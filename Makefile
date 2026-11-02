# assay — build, test, and clean with one command each.
#
#   make              build build/assay
#   make test         build, then run unit + golden tests
#   make test SANITIZE=1   same, with AddressSanitizer + UBSan (catches memory bugs)
#   make clean
#
# New to Makefiles? Each rule is:   target: dependencies
#                                   <TAB>commands
# make only rebuilds a target when a dependency is newer than it.

NAME    := assay
CC      ?= cc
CFLAGS  := -std=c11 -Wall -Wextra -Wpedantic -Wshadow -g -O1
LDFLAGS :=

ifeq ($(SANITIZE),1)
  CFLAGS  += -fsanitize=address,undefined -fno-omit-frame-pointer
  LDFLAGS += -fsanitize=address,undefined
endif

BUILD   := build
SRCS    := $(wildcard src/*.c)
OBJS    := $(SRCS:src/%.c=$(BUILD)/%.o)
# Everything except main.o, so unit tests can link against the real code.
LIBOBJS := $(filter-out $(BUILD)/main.o,$(OBJS))

.PHONY: all test test-unit test-golden clean

all: $(BUILD)/$(NAME)

$(BUILD)/$(NAME): $(OBJS)
	$(CC) $(LDFLAGS) -o $@ $^

# -MMD -MP writes .d files so editing a header rebuilds what includes it.
$(BUILD)/%.o: src/%.c | $(BUILD)
	$(CC) $(CFLAGS) -MMD -MP -c $< -o $@

$(BUILD)/test_scanner: tests/unit/test_scanner.c tests/unit/test.h $(LIBOBJS)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ tests/unit/test_scanner.c $(LIBOBJS)

$(BUILD):
	mkdir -p $(BUILD)

# Run both suites even if the first fails, then report overall failure.
test: all $(BUILD)/test_scanner
	@status=0; \
	echo "== unit tests =="; ./$(BUILD)/test_scanner || status=1; \
	echo; echo "== golden tests =="; python3 tests/run_tests.py || status=1; \
	exit $$status

test-unit: $(BUILD)/test_scanner
	./$(BUILD)/test_scanner

test-golden: all
	python3 tests/run_tests.py -v

clean:
	rm -rf $(BUILD)

-include $(OBJS:.o=.d)
