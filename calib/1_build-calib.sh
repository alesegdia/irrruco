
#!/usr/bin/env bash

g++ `pkg-config --cflags opencv` -g -o calib calibration.cpp `pkg-config --libs opencv`
