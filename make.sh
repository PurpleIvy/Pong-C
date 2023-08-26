#!/bin/sh

gcc -Wall -o Pong main.c `pkg-config --libs --cflags sdl3`
