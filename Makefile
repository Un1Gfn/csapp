.PHONY: clean

clean:
	@rm -fv \
		*.c.s \
		*.o \
		*.out

p41_hello.c.s:
%.c.s: %.c ; gcc -S $(CFLAGS_NDEBUG) -o $@ $<

p54_overwrite_other_proc_var.out:
p54_virtual_address_space.out:
p68_float_overflow.out:
p69_float_not_associative.out:

p76_prog_32.out: M:=-m32
p76_prog_64.out: M:=-m64
p76_prog_%.out: p76_prog.c; gcc $(CFLAGS) $(M) $(shell pkg-config --libs libelf,libbsd) -o $@ $<
# p76_prog_%.out: p76_prog.c ; gcc $(CFLAGS) $(M) -ldw -lelf -o $@ $<
p76_prog_run: p76_prog_32.out p76_prog_64.out
	@echo
	@./p76_prog_32.out
	@echo
	@./p76_prog_64.out
	@echo

# ~/buildroot-2020.11.1-m68k/output/host/usr/share/man/man1/m68k-buildroot-linux-uclibc-gcc.1
# ~/buildroot-2020.11.1-powerpc/output/host/usr/share/man/man1/powerpc-buildroot-linux-gnu-gcc.1
p81_show_bytes_i386.out: CC:=gcc -m32
p81_show_bytes_amd64.out: CC:=gcc -m64
p81_show_bytes_m68k.out: CC:=~/buildroot-2020.11.1-m68k/output/host/bin/m68k-buildroot-linux-uclibc-gcc
p81_show_bytes_powerpc.out: CC:=~/buildroot-2020.11.1-powerpc/output/host/bin/powerpc-buildroot-linux-gnu-gcc
p81_show_bytes_%.out: p81_show_bytes.c; $(CC) $(CFLAGS) -static -o $@ $<
p81_show_bytes_run: $(addsuffix .out, $(addprefix p81_show_bytes_, i386 amd64 m68k powerpc ) )
	@echo
	@echo -n "move this window to an empty workspace then press enter "; read -r
	@echo
	@sleep 0.3; alacritty --title i386    --command sh -c "qemu-i386   ./p81_show_bytes_i386.out    12345; read -r" &
	@sleep 0.3; alacritty --title amd64   --command sh -c "qemu-x86_64 ./p81_show_bytes_amd64.out   12345; read -r" &
	@sleep 0.3; alacritty --title m68k    --command sh -c "qemu-m68k   ./p81_show_bytes_m68k.out    12345; read -r" &
	@sleep 0.3; alacritty --title powerpc --command sh -c "qemu-ppc    ./p81_show_bytes_powerpc.out 12345; read -r" &
# 	@sleep 0.3; alacritty --title i386    --command sh -c "qemu-i386   -L /                                            ./p81_show_bytes_i386.out; read -r" &
# 	@sleep 0.3; alacritty --title amd64   --command sh -c "qemu-x86_64 -L /                                            ./p81_show_bytes_amd64.out; read -r" &
# 	@sleep 0.3; alacritty --title m68k    --command sh -c "qemu-m68k   -L ~/buildroot-2020.11.1-m68k/output/target/    ./p81_show_bytes_m68k.out; read -r" &
# 	@sleep 0.3; alacritty --title powerpc --command sh -c "qemu-ppc    -L ~/buildroot-2020.11.1-powerpc/output/target/ ./p81_show_bytes_powerpc.out; read -r" &

p83_longest_common_substring.out:

# test:
# 	@echo $(addprefix a, b c)

%.out: %.c ; gcc $(CFLAGS) -o $@ $<

include Makefile.defs
