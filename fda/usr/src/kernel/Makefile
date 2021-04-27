# Makefile for kernel

# Directories
u = /usr
i = $u/include
l = $u/lib
s = system

# Programs, flags, etc.
CC =	exec cc
CPP =	$l/cpp
LD =	$(CC) -.o
CFLAGS = -I$i 
LDFLAGS = -i 

HEAD =	mpx.o
OBJS =	start.o protect.o klib.o table.o kprintf.o main.o proc.o \
	i8259.o exception.o system.o clock.o utility.o debug.o \
	e9_car.o e9_str.o e9_raw.o e9_printf.o vsprintf.o \
	u9_car.o u9_str.o sys_call_e9.o \
	sys_call_plot.o plotear.o 
SYSTEM = system.a
LIBS = -ltimers 


# What to make.
all: build 
kernel build install: $(HEAD) $(OBJS) 
	cd system && $(MAKE) -$(MAKEFLAGS) $@
	$(LD) $(CFLAGS) $(LDFLAGS) -o kernel \
	$(HEAD) $(OBJS) \
	$(SYSTEM) $(LIBS)
	install -S 0 kernel

clean:
	cd system && $(MAKE) -$(MAKEFLAGS) $@
	rm -f *.a *.o *~ *.bak kernel

depend: 
	cd system && $(MAKE) -$(MAKEFLAGS) $@
	/usr/bin/mkdep "$(CC) -E $(CPPFLAGS)" *.c *.s > .depend

# How to build it
.s.o:
	$(CC) $(CFLAGS) -c -o $@ $<

.c.o:
	$(CC) $(CFLAGS) -c -o $@ $<

# Include generated dependencies.
klib.o:	klib386.s klib88.s
mpx.o:	mpx386.s mpx88.s
include .depend

