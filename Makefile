CC := gcc
CFLAGS := -Wall -Wpedantic -Wextra -Wno-unused-parameter -Wconversion -Wdouble-promotion
DIRS := csv linear_regressor dt_hund dt_gini dt_entropy
LIBRARY_DIR := lib
BUILD_DIR := build
TARGETS := $(patsubst %,$(LIBRARY_DIR)/%.a,$(DIRS))
TEST_TARGETS := $(patsubst %,test_%,$(DIRS))
PREREQS := $(patsubst %,$(BUILD_DIR)/%.o,$(DIRS))

all: $(TARGETS)

# making static libraries
$(LIBRARY_DIR)/csv.a: $(BUILD_DIR)/csv.o
	@ mkdir -p $(dir $@)
	@ ar rc $@ $^
	@ ranlib $@

$(LIBRARY_DIR)/linear_regressor.a: $(BUILD_DIR)/linear_regressor.o $(BUILD_DIR)/csv.o
	@ mkdir -p $(dir $@)
	@ ar rc $@ $^
	@ ranlib $@

$(LIBRARY_DIR)/dt_hund.a: $(BUILD_DIR)/dt_hund.o $(BUILD_DIR)/csv.o
	@ mkdir -p $(dir $@)
	@ ar rc $@ $^
	@ ranlib $@

$(LIBRARY_DIR)/dt_entropy.a: $(BUILD_DIR)/dt_entropy.o $(BUILD_DIR)/csv.o
	@ mkdir -p $(dir $@)
	@ ar rc $@ $^
	@ ranlib $@

$(LIBRARY_DIR)/dt_gini.a: $(BUILD_DIR)/dt_gini.o $(BUILD_DIR)/csv.o
	@ mkdir -p $(dir $@)
	@ ar rc $@ $^
	@ ranlib $@

# Tests
test_csv: $(LIBRARY_DIR)/csv.a
	@ mkdir -p $(BUILD_DIR)
	@ cp csv/test/test_csv.csv $(BUILD_DIR)
	@ $(CC) $(CFLAGS) csv/test/test.c -Icsv/include -L$(LIBRARY_DIR) -lcsv -o $(BUILD_DIR)/csv_test && cd $(BUILD_DIR) && ./csv_test

test_linear_regressor: $(LIBRARY_DIR)/linear_regressor.a $(LIBRARY_DIR)/csv.a
	@ mkdir -p $(BUILD_DIR)
	@ $(CC) $(foreach dep,$^,-l$(dep))

test_dt_hund: $(LIBRARY_DIR)/dt_hund.a $(LIBRARY_DIR)/csv.a
	@ mkdir -p $(BUILD_DIR)
	@ echo l$(patsubst %.a,%,$^)

test_dt_entropy: $(LIBRARY_DIR)/dt_entropy.a $(LIBRARY_DIR)/csv.a
	@ mkdir -p $(BUILD_DIR)
	@ echo l$(patsubst %.a,%,$^)

test_dt_gini: $(LIBRARY_DIR)/dt_gini.a $(LIBRARY_DIR)/csv.a
	@ mkdir -p $(BUILD_DIR)
	@ echo l$(patsubst %.a,%,$^)

# $(BUILD_DIR)/%.o: $(SOURCE_DIR)/%.c $(HEADERS)
# 	@ mkdir -p $(dir $@)
# 	@ $(CC) $(CFLAGS) -c -o $@ $<

clean:
	@ rm -rf $(BUILD_DIR)
	@ rm -rf $(LIBRARY_DIR)

.PHONY: all clean $(TEST_TARGETS)
