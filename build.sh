#!/bin/bash

gcc -o screenshot src/screenshot.c src/wlr-screencopy.c -l wayland-client -I include
