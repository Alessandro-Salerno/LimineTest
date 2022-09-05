x86_64-linux-gnu-gcc module.c -c -o module.o
x86_64-linux-gnu-ld module.o -T module.ld -o module.elf