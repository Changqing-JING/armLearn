sourceDir=src
buildDir=build
assembly:
	aarch64-linux-gnu-gcc -S -o ${sourceDir}/main.s ${sourceDir}/main.c

bin:
	aarch64-linux-gnu-gcc -g -o ${buildDir}/main.bin ${sourceDir}/main.s

obj:
	aarch64-linux-gnu-as -c -g -o ${buildDir}/asmVsMachinecode.o ${sourceDir}/asmVsMachinecode.s

dump: obj
	aarch64-linux-gnu-objdump -D -j .text  ${buildDir}/asmVsMachinecode.o
disassembly:
	aarch64-linux-gnu-objdump -D -j .text  ${buildDir}/main.bin

GDBServerAssembly:
	gdbserver 127.0.0.1:5000 build/main.bin
gdbserverC:
	gdbserver 127.0.0.1:5000 build/armLearn