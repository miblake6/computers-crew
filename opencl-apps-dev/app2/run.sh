#!/bin/bash

export OMP_NUM_THREADS=8

time ./cpu.bin
time ./gpu.bin
