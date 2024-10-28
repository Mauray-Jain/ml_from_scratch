#include "../str.h"
#include <bits/posix2_lim.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int parse(FILE* src, DataFrame* df) {
	char buf[LINE_MAX] = {0};
	fgets(buf, LINE_MAX, src);
	size_t commas = 0;
	size_t j = 0, k = 0;
	for (size_t i = 0; buf[i] != '\0'; i++) {
		if (buf[i] == ',' || buf[i] == '\n') { // df,df\n\0
			char* tempstr = malloc((sizeof(char)) * (i - j + 1));
			strncpy(tempstr, &buf[j], i-j);
			tempstr[i-j] = '\0';
			printf("%lu\n", commas);
			df->col_names[k].len = i-j+1;
			df->col_names[k].ptr = tempstr;
			j = i;
			k++;
			commas++;
		}
	}
	df->len_cols = commas+1;
	
	return 0;
}

int main(void) {
	FILE* src = fopen("test.csv", "r");
	DataFrame df = {0};
	parse(src, &df);
	printf("%lu\n", df.len_cols);
	for (size_t i = 0; i < df.len_cols; i++) {
		printf("%*s", (int)df.col_names[i].len, df.col_names[i].ptr);
		free(df.col_names[i].ptr);
	}
	fclose(src);
	return 0;
}
