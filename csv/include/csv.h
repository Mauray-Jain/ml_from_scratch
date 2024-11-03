#ifndef ML_SCRATCH_CSV_PARSER_H
#define ML_SCRATCH_CSV_PARSER_H

#include <stddef.h>

typedef struct str str;
struct str {
	size_t len;
	char* ptr;
};

typedef struct arr_double arr_double;
struct arr_double {
	size_t len;
	double* ptr;
};

typedef struct DataFrame DataFrame;
struct DataFrame {
	size_t len_rows;
	size_t len_cols;
	str* col_names;
	double** data;
};

void parse(const char* filename, DataFrame* df);
void free_dataframe(DataFrame* df);

#endif /* end of include guard: ML_SCRATCH_CSV_PARSER_H */
