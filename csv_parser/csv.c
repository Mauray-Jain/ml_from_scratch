#include "../str.h"
#include <bits/posix2_lim.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int parse(FILE* src, DataFrame* df) {
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
	return 0;
}

int main(void) {
	FILE* src = fopen("test.csv", "r");
	DataFrame df = {0};
	parse(src, &df);
	/*printf("%lu\n", df.len_cols);*/
	printf("%lu, %lu\n", df.len_cols, df.len_rows);
	for (size_t i = 0; i < df.len_cols; i++) {
		/*printf(",%s,\n", df.col_names[i].ptr);*/
		for (size_t j = 0; j < df.len_rows; j++) {
			printf("[%lu][%lu] = %lf\n", i, j, df.data[i][j]);
		}
		free(df.col_names[i].ptr);
		free(df.data[i]);
	}
	free(df.col_names);
	free(df.data);
	fclose(src);
	return 0;
}
