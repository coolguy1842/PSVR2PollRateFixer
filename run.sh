#!/usr/bin/env bash

meson setup build --reconfigure
cd build

meson compile -j12

cd ..

# use this one for more detailed stack traces
env ./build/PSVR2PollRateFixer ${@,2}