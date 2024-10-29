#include "csv.h"
#include <bits/posix2_lim.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

void parse(const char* filename, DataFrame* df) {
	FILE* src = fopen(filename, "rb");
	char buf[LINE_MAX] = {0};
	fgets(buf, LINE_MAX, src);
	size_t commas = 0;
	for (size_t i = 0; buf[i] != '\0'; i++) {
		if (buf[i] == ',') commas++;
	}

	size_t j = 0, k = 0;
	df->len_cols = commas+1;
	df->col_names = malloc((sizeof(str)) * df->len_cols);

	for (size_t i = 0; buf[i] != '\0'; i++) {
		if (buf[i] == ',' || buf[i] == '\n') {
			char* tempstr = malloc((sizeof(char)) * (i - j + 1));
			strncpy(tempstr, &buf[j], i-j);
			tempstr[i-j] = '\0';
			df->col_names[k].len = i-j+1;
			df->col_names[k].ptr = tempstr;
			i++;
			j = i;
			k++;
		}
	}

	long pos = ftell(src);
	size_t num_rows = 0;

	while (fgets(buf, LINE_MAX, src)) num_rows++;
	fseek(src, pos, SEEK_SET);

	df->len_rows = num_rows;
	df->data = malloc(sizeof(double*) * df->len_cols);
	for (size_t i = 0; i < df->len_cols; i++) {
		df->data[i] = malloc(sizeof(double) * df->len_rows);
	}

	size_t row = 0, col = 0;
	while (fgets(buf, LINE_MAX, src)) {
		char *end, *p = buf;
		for (double f = strtod(p, &end); p != end; f = strtod(p, &end)) {
			df->data[col][row] = f;
			col++;
			p = end;
			p++;
		}
		col = 0;
		row++;
	}
	fclose(src);
}

void free_dataframe(DataFrame* df) {
	for (size_t i = 0; i < df->len_cols; i++) {
		free(df->col_names[i].ptr);
		df->col_names[i].ptr = NULL;
		df->col_names[i].len = 0;
		free(df->data[i]);
		df->data[i] = NULL;
	}
	free(df->col_names);
	df->col_names = NULL;
	free(df->data);
	df->data = NULL;
	df->len_rows = 0;
	df->len_cols = 0;
}

static int test(void) {
	DataFrame df = {0};
	parse("test.csv", &df);
	/*printf("%lu\n", df.len_cols);*/
	printf("%lu, %lu\n", df.len_cols, df.len_rows);
	for (size_t i = 0; i < df.len_cols; i++) {
		/*printf(",%s,\n", df.col_names[i].ptr);*/
		for (size_t j = 0; j < df.len_rows; j++) {
			printf("[%lu][%lu] = %lf\n", i, j, df.data[i][j]);
		}
	}
	free_dataframe(&df);
	return 0;
}

