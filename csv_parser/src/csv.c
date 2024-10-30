#include "csv.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define LINE_MAX 2048

void parse(const char* filename, DataFrame* df) {
	FILE* src = fopen(filename, "rb");
	char buf[LINE_MAX] = {0};
	fgets(buf, LINE_MAX, src);

	// Count columns
	size_t commas = 0;
	for (size_t i = 0; buf[i] != '\0'; i++) {
		if (buf[i] == ',') commas++;
	}
	df->len_cols = commas+1;

	// Fill column names in df
	df->col_names = malloc((sizeof(str)) * df->len_cols);
	size_t j = 0, k = 0;
	for (size_t i = 0; buf[i] != '\0'; i++) {
		if (buf[i] == ',' || buf[i] == '\n') {
			const size_t len = i - j + 1;
			char* tempstr = malloc(sizeof(char) * len);
			strncpy(tempstr, &buf[j], len-1);
			tempstr[len-1] = '\0';
			df->col_names[k].len = len;
			df->col_names[k].ptr = tempstr;
			i++; j = i; k++;
		}
	}

	// Count number of rows
	long pos = ftell(src);
	size_t num_rows = 0;
	while (fgets(buf, LINE_MAX, src)) num_rows++;
	fseek(src, pos, SEEK_SET);
	df->len_rows = num_rows;

	// Allocate data
	df->data = malloc(sizeof(double*) * df->len_cols);
	for (size_t i = 0; i < df->len_cols; i++) {
		df->data[i] = malloc(sizeof(double) * df->len_rows);
	}

	// Fill data
	size_t row = 0, col = 0;
	while (fgets(buf, LINE_MAX, src)) {
		char *end, *p = buf;
		for (double f = strtod(p, &end); p != end; f = strtod(p, &end)) {
			df->data[col][row] = f;
			col++; p = end; p++;
		}
		col = 0; row++;
	}
	fclose(src);
}

void free_dataframe(DataFrame* df) {
	for (size_t i = 0; i < df->len_cols; i++) {
		free(df->col_names[i].ptr);
		free(df->data[i]);
		df->col_names[i].ptr = NULL;
		df->col_names[i].len = 0;
		df->data[i] = NULL;
	}
	free(df->col_names);
	free(df->data);
	df->col_names = NULL;
	df->data = NULL;
	df->len_rows = 0;
	df->len_cols = 0;
}
