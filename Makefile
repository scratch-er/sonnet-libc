LIBC_BUILD_DIR ?= build
DESTDIR ?= $(LIBC_BUILD_DIR)/sysroot

ifneq ($(TOOLCHAIN_FILE),)
	include $(TOOLCHAIN_FILE)
endif

CFLAGS += -isystem include

LIBC_SRCS := src/ctype.c src/stdio.c src/printf.c src/stdlib.c src/string.c
LIBC_OBJS := $(patsubst src/%.c,$(LIBC_BUILD_DIR)/objs/%.o,$(LIBC_SRCS))

all: $(LIBC_BUILD_DIR)/libc.a

install: $(LIBC_BUILD_DIR)/libc.a
	mkdir -p $(DESTDIR)/lib
	cp -f $(LIBC_BUILD_DIR)/libc.a $(DESTDIR)/lib/
	cp -rf include $(DESTDIR)/

clean:
	rm -rf $(LIBC_BUILD_DIR)/libc.a $(LIBC_BUILD_DIR)/objs

$(LIBC_BUILD_DIR)/objs/%.o: src/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c -o $@ $<

$(LIBC_BUILD_DIR)/libc.a: $(LIBC_OBJS)
	@rm -f $@
	$(AR) rcs $@ $(LIBC_OBJS)

.PHONY: all install clean
