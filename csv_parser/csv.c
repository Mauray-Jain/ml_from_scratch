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

	while (!fgets(buf, LINE_MAX, src)) num_rows++;
	fseek(src, pos, SEEK_SET);

	df->len_rows = num_rows;
	printf("%lu\n", df->len_rows);
	df->data = malloc(sizeof(double*) * df->len_rows);

	for (size_t i = 0; i < df->len_rows; i++) {
		fgets(buf, LINE_MAX, src);
		df->data[i] = malloc(sizeof(double) * df->len_cols);
		char *end, *p;
		for (double f = strtod(p, &end); p != end; f = strtod(p, &end)) {
			df->data[i][j] = f;
			p = end;
			printf("%lf\n", f);
		}
	}
	return 0;
}

int main(void) {
	FILE* src = fopen("test.csv", "r");
	DataFrame df = {0};
	parse(src, &df);
	/*printf("%lu\n", df.len_cols);*/
	for (size_t i = 0; i < df.len_cols; i++) {
		/*printf(",%s,\n", df.col_names[i].ptr);*/
		free(df.col_names[i].ptr);
	}
	fclose(src);
	return 0;
}
