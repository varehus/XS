#! /usr/bin/env sh

rm -rf buildscan
mkdir buildscan
scan-build meson setup buildscan
cd buildscan
scan-build --view ninja
