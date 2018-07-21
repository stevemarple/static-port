/*
 * Copyright (C) 2018 Steve Marple
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public License
 * as published by the Free Software Foundation; either version 2.1 of
 * the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.

 * Based on bind.c with revisions to modify port instead of address.
 * Copyright (C) 2000  Daniel Ryde
 * Small amendment by Daniel Lange, 2010
 */

/*
 * LD_PRELOAD library to make bind() use a predefined port specified
 * by the STATIC_PORT enviroment variable.
 *
 * Compile on Linux with:
 * gcc -nostartfiles -fpic -shared static-port.c -o static-port.so -ldl -D_GNU_SOURCE
 *
 * Example in bash to make listening connections use port 25565.
 *
 * STATIC_PORT="25565" LD_PRELOAD=./static-port.so java
 *
 * This can be used to fix Minecraft so that the "Open to LAN" option
 * uses a pre-defined port.
 *
 * This program written by Steve Marple based on similar work by
 * Daniel Ryde and Daniel Lange.

 * Twitter: @stevemarple
 * Github: https://github.com/stevemarple/static-port
 */

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <dlfcn.h>
#include <errno.h>


int (*real_bind)(int, const struct sockaddr *, socklen_t);
unsigned short port = 0;

char *logfilename = "/tmp/static-port.log";
FILE *logfile = NULL;

void _init(void)
{
	const char *err;

	if (logfilename != NULL && (logfile = fopen(logfilename, "w")) != NULL) {
		fprintf(logfile, "_init() called\n");
		fflush(logfile);
	}

	if (getenv("STATIC_PORT")) {
		port = (unsigned short)atoi(getenv("STATIC_PORT"));
	}

	if (logfile) {
		fprintf(logfile, "Static port: %d\n", (int)port);
		fflush(logfile);
	}

	real_bind = dlsym(RTLD_NEXT, "bind");
	if ((err = dlerror()) != NULL) {
		fprintf(stderr, "ERROR: dlsym(bind): %s\n", err);
		if (logfile) {
			fprintf(logfile, "ERROR: dlsym(bind): %s\n", err);
			fflush(logfile);
		}
	}
}


int bind (int fd, const struct sockaddr *sk, socklen_t sl)
{
	//static struct sockaddr_in *lsk_in;
	struct sockaddr_in *lsk_in;

	if (logfile) {
		fprintf(logfile, "bind() called\n");
		fflush(logfile);
	}


	lsk_in = (struct sockaddr_in *)sk;
	if ((lsk_in->sin_family == AF_INET || lsk_in->sin_family == AF_INET6) && lsk_in->sin_port == 0) {
		lsk_in->sin_port = htons(port);
		if (logfile) {
			fprintf(logfile, "Mapped bind() to use port %d\n", port);
			fflush(logfile);
		}
	}

	return real_bind(fd, sk, sl);
}
