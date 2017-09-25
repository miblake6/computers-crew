all: sgemm_cpu.bin sgemm_gpu.bin dgemm_cpu.bin dgemm_gpu.bin dgemm_hybrid.bin

sgemm_cpu.bin: sgemm_cpu.c
	gcc -g -O3 -Wall -fopenmp -o sgemm_cpu.bin sgemm_cpu.c -lm 

sgemm_gpu.bin: sgemm_gpu.c
	gcc -g -O3 -Wall -o sgemm_gpu.bin sgemm_gpu.c -lOpenCL -lm

dgemm_cpu.bin: dgemm_cpu.c
	gcc -g -O3 -Wall -fopenmp -o dgemm_cpu.bin dgemm_cpu.c -lm 

dgemm_gpu.bin: dgemm_gpu.c
	gcc -g -O3 -Wall -o dgemm_gpu.bin dgemm_gpu.c -lOpenCL -lm

sgemm_hybrid.bin: sgemm_hybrid.c
	gcc -g -O3 -Wall -fopenmp -o sgemm_hybrid.bin sgemm_hybrid.c -lm 

dgemm_hybrid.bin: dgemm_hybrid.c
	gcc -g -O3 -Wall -fopenmp -o dgemm_hybrid.bin dgemm_hybrid.c -lOpenCL -lm

clean:
	rm -f *.bin out.*
