obj-m += watch-irq.o
watch-irq-objs := main-test.o stats.o gpio-ops.o normal-signal.o interrupt-handler.o normal-poll.o debugfs-utils.o
 
#change to your linux source file
KDIR = 
ARCH = arm64
CROSS_COMPILE = aarch64-linux-gnu-

all:
	make ARCH=$(ARCH) CROSS_COMPILE=$(CROSS_COMPILE) -C $(KDIR)  M=$(shell pwd) modules
 
clean:
	make ARCH=$(ARCH) CROSS_COMPILE=$(CROSS_COMPILE) -C $(KDIR)  M=$(shell pwd) clean
