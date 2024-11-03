#ifndef ML_SCRATCH_LINEAR_REGRESSOR_H
#define ML_SCRATCH_LINEAR_REGRESSOR_H

#include "csv.h"

#define ABS(x) (((x) > 0)? (x) : -(x))

typedef struct LinearRegressor LinearRegressor;
struct LinearRegressor {
	double w;
	double b;
	double mae;
	double mse;
};

void linear_regressor_fit(LinearRegressor* lr, DataFrame* df, double eeta, int epochs);
void linear_regressor_predict(LinearRegressor* lr, arr_double* x, arr_double* result);

#endif /* end of include guard: ML_SCRATCH_LINEAR_REGRESSOR_H */
