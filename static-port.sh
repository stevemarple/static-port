#!/bin/sh
export STATIC_PORT="25565"

STATIC_PORT="25565" LD_PRELOAD=/usr/local/bin/static-port.so java "$@"
