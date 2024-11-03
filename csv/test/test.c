#include "csv.h"
#include <stdio.h>

int main(void) {
	DataFrame df = {0};
	parse("test_csv.csv", &df);
	printf("%lu\n", df.len_cols);
	printf("%lu, %lu\n", df.len_cols, df.len_rows);
	for (size_t i = 0; i < df.len_cols; i++) {
		printf(",%s,\n", df.col_names[i].ptr);
		for (size_t j = 0; j < df.len_rows; j++) {
			printf("[%lu][%lu] = %lf\n", i, j, df.data[i][j]);
		}
	}
	free_dataframe(&df);
	return 0;
}

