#!/bin/bash

export OMP_NUM_THREADS=8

time ./dgemm_cpu.bin
time ./dgemm_gpu.bin
