CC := gcc
CFLAGS := -Wall -Wpedantic -Wextra -Wno-unused-parameter -Wconversion -Wdouble-promotion
DIRS := csv linear_regressor dt_hund dt_gini dt_entropy
LIBRARY_DIR := lib
BUILD_DIR := build
TARGETS := $(patsubst %,$(LIBRARY_DIR)/lib%.a,$(DIRS))
TEST_TARGETS := $(patsubst %,test_%,$(DIRS))
PREREQS := $(patsubst %,$(BUILD_DIR)/%.o,$(DIRS))

all: $(TARGETS)

# making .o
$(BUILD_DIR)/csv.o: csv/src/csv.c csv/include/csv.h
	@ mkdir -p $(dir $@)
	@ $(CC) $(CFLAGS) -c -Icsv/include $< -o $@

$(BUILD_DIR)/%.o: linear_regressor/src/%.c linear_regressor/include/%.h
	@ mkdir -p $(dir $@)
	@ $(CC) $(CFLAGS) -c -Icsv/include -Ilinear_regressor/include $< -o $@

# making static libraries
$(LIBRARY_DIR)/libcsv.a: $(BUILD_DIR)/csv.o
	@ mkdir -p $(dir $@)
	@ ar rc $@ $^
	@ ranlib $@

$(LIBRARY_DIR)/liblinear_regressor.a: $(BUILD_DIR)/linear_regressor.o $(BUILD_DIR)/csv.o
	@ mkdir -p $(dir $@)
	@ ar rc $@ $^
	@ ranlib $@

$(LIBRARY_DIR)/libdt_hund.a: $(BUILD_DIR)/dt_hund.o $(BUILD_DIR)/csv.o
	@ mkdir -p $(dir $@)
	@ ar rc $@ $^
	@ ranlib $@

$(LIBRARY_DIR)/libdt_entropy.a: $(BUILD_DIR)/dt_entropy.o $(BUILD_DIR)/csv.o
	@ mkdir -p $(dir $@)
	@ ar rc $@ $^
	@ ranlib $@

$(LIBRARY_DIR)/libdt_gini.a: $(BUILD_DIR)/dt_gini.o $(BUILD_DIR)/csv.o
	@ mkdir -p $(dir $@)
	@ ar rc $@ $^
	@ ranlib $@

# Tests
test_csv: $(LIBRARY_DIR)/libcsv.a
	@ mkdir -p $(BUILD_DIR)
	@ cp csv/test/test_csv.csv $(BUILD_DIR)
	@ $(CC) $(CFLAGS) csv/test/test.c -Icsv/include \
		-L$(LIBRARY_DIR) -lcsv -o $(BUILD_DIR)/csv_test && \
		cd $(BUILD_DIR) && ./csv_test

test_linear_regressor: $(LIBRARY_DIR)/liblinear_regressor.a $(LIBRARY_DIR)/libcsv.a
	@ mkdir -p $(BUILD_DIR)
	@ cp csv/test/test_linear_regressor.csv $(BUILD_DIR)
	@ $(CC) $(CFLAGS) linear_regressor/test/test.c -Icsv/include -Ilinear_regressor/include \
		-L$(LIBRARY_DIR) -lcsv -llinear_regressor -o $(BUILD_DIR)/linear_regressor_test && \
		cd $(BUILD_DIR) && ./linear_regressor_test

test_dt_hund: $(LIBRARY_DIR)/libdt_hund.a $(LIBRARY_DIR)/libcsv.a
	@ mkdir -p $(BUILD_DIR)
	@ echo l$(patsubst %.a,%,$^)

test_dt_entropy: $(LIBRARY_DIR)/libdt_entropy.a $(LIBRARY_DIR)/libcsv.a
	@ mkdir -p $(BUILD_DIR)
	@ echo l$(patsubst %.a,%,$^)

test_dt_gini: $(LIBRARY_DIR)/libdt_gini.a $(LIBRARY_DIR)/libcsv.a
	@ mkdir -p $(BUILD_DIR)
	@ echo l$(patsubst %.a,%,$^)

# $(BUILD_DIR)/%.o: $(SOURCE_DIR)/%.c $(HEADERS)
# 	@ mkdir -p $(dir $@)
# 	@ $(CC) $(CFLAGS) -c -o $@ $<

clean:
	@ rm -rf $(BUILD_DIR)
	@ rm -rf $(LIBRARY_DIR)

.PHONY: all clean $(TEST_TARGETS)
