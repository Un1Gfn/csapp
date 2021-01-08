.PHONY: clean

clean:
	@rm -fv \
		*.c.s \
		*.o \
		*.out

p41_hello.c.s:
%.c.s: %.c ; $(CC) -S $(CFLAGS_NDEBUG) -o $@ $<

p54_overwrite_other_proc_var.out:
p54_virtual_address_space.out:
p68_float_overflow.out:
p69_float_not_associative.out:
%.out: %.c ; $(CC) $(CFLAGS) -o $@ $<

p76_prog_32.out: M:=-m32
p76_prog_64.out: M:=-m64
p76_prog_%.out: p76_prog.c ; $(CC) $(CFLAGS) $(M) $(shell pkg-config --libs libelf,libbsd) -o $@ $<
# p76_prog_%.out: p76_prog.c ; $(CC) $(CFLAGS) $(M) -ldw -lelf -o $@ $<
p76_prog_run: p76_prog_32.out p76_prog_64.out
	@echo
	@./p76_prog_32.out
	@echo
	@./p76_prog_64.out
	@echo

include Makefile.defs
