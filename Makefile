LIBC_BUILD_DIR ?= build
DESTDIR ?= $(LIBC_BUILD_DIR)/sysroot

ifneq ($(TOOLCHAIN_FILE),)
	include $(TOOLCHAIN_FILE)
endif

CFLAGS += -std=c23 -ffreestanding -isystem include -fno-stack-protector

LIBC_SRCS := src/ctype.c src/stdio.c src/printf.c src/stdlib.c src/string.c
LIBC_OBJS := $(patsubst src/%.c,$(LIBC_BUILD_DIR)/objs/%.o,$(LIBC_SRCS))

all: $(LIBC_BUILD_DIR)/libc.a

install: $(LIBC_BUILD_DIR)/libc.a
	mkdir -p $(DESTDIR)/lib
	cp -f $(LIBC_BUILD_DIR)/libc.a $(DESTDIR)/lib/
	cp -rf include $(DESTDIR)/

clean:
	rm -rf $(LIBC_BUILD_DIR)/libc.a $(LIBC_BUILD_DIR)/objs $(LIBC_BUILD_DIR)/test

TEST_SRCS := test/test_memory.c test/test_string.c test/test_stdlib.c
TEST_BINS := $(patsubst test/test_%.c,$(LIBC_BUILD_DIR)/test/test_%,$(TEST_SRCS))

$(LIBC_BUILD_DIR)/objs/%.o: src/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c -o $@ $<

$(LIBC_BUILD_DIR)/libc.a: $(LIBC_OBJS)
	@rm -f $@
	$(AR) rcs $@ $(LIBC_OBJS)

$(LIBC_BUILD_DIR)/test/test_%: $(LIBC_BUILD_DIR)/test/test_%.o $(LIBC_BUILD_DIR)/test/test.o $(LIBC_BUILD_DIR)/libc.a
	$(CC) $(CFLAGS) -nostdlib -static -o $@ $^

$(LIBC_BUILD_DIR)/test/test_%.o: test/test_%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c -o $@ $<

$(LIBC_BUILD_DIR)/test/test.o: test/test.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c -o $@ $<

test: $(TEST_BINS)
	@for t in $(TEST_BINS); do $$t ; done

.PHONY: all install clean test
