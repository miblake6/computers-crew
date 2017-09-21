all: cpu.bin gpu.bin

cpu.bin: cpuversion.c
	gcc -O3 -o cpu.bin cpuversion.c -lm

gpu.bin: gpuversion.c
	gcc -O3 -o gpu.bin gpuversion.c -lOpenCL -lm

clean:
	rm -f *.bin
