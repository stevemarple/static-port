static-port.so: static-port.c
	gcc -nostartfiles -fpic -shared static-port.c -o static-port.so -ldl -D_GNU_SOURCE

