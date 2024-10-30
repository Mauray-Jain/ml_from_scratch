CC := gcc
CFLAGS := -Wall -Wpedantic -Wextra -Wno-unused-parameter -Wconversion -Wdouble-promotion
DIRS := csv_parser linear_regressor dt_hund dt_gini dt_entropy
LIBRARY_DIR := lib
BUILD_DIR := build
TARGETS := $(patsubst %,$(LIBRARY_DIR)/%.a,$(DIRS))
PREREQS := $(patsubst %,$(BUILD_DIR)/%.o,$(DIRS))

all: $(TARGETS)

# making static libraries
$(LIBRARY_DIR)/csv_parser.a: $(BUILD_DIR)/csv_parser.o
	@ mkdir -p $(dir $@)
	@ ar rc $@ $^

$(LIBRARY_DIR)/linear_regressor.a: $(BUILD_DIR)/linear_regressor.o $(BUILD_DIR)/csv_parser.o
	@ mkdir -p $(dir $@)
	@ ar rc $@ $^

$(LIBRARY_DIR)/dt_hund.a: $(BUILD_DIR)/dt_hund.o $(BUILD_DIR)/csv_parser.o
	@ mkdir -p $(dir $@)
	@ ar rc $@ $^

$(LIBRARY_DIR)/dt_entropy.a: $(BUILD_DIR)/dt_entropy.o $(BUILD_DIR)/csv_parser.o
	@ mkdir -p $(dir $@)
	@ ar rc $@ $^

$(LIBRARY_DIR)/dt_gini.a: $(BUILD_DIR)/dt_gini.o $(BUILD_DIR)/csv_parser.o
	@ mkdir -p $(dir $@)
	@ ar rc $@ $^

test: $(TARGETS)


# $(BUILD_DIR)/%.o: $(SOURCE_DIR)/%.c $(HEADERS)
# 	@ mkdir -p $(dir $@)
# 	@ $(CC) $(CFLAGS) -c -o $@ $<

clean:
	@ rm -rf $(BUILD_DIR)

.PHONY: all clean test
