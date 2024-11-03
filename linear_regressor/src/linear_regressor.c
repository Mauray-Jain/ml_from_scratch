#include "linear_regressor.h"
#include "csv.h"

static void gradient_descent(LinearRegressor* lr, DataFrame* df, double eeta) {
	double* x = df->data[0], *y = df->data[1];
	double w = lr->w, b = lr->b;
	double diff_w = 0, diff_b = 0;

	for (size_t i = 0; i < df->len_rows; i++) {
		diff_w += x[i] * (y[i] - (w * x[i] + b));
	}
	diff_w *= (double) -2 / df->len_rows;

	for (size_t i = 0; i < df->len_rows; i++) {
		diff_b += y[i] - (w * x[i] + b);
	}
	diff_b *= (double) -2 / df->len_rows;

	lr->w = w - eeta * diff_w;
	lr->b = b - eeta * diff_b;
}

static void set_cost(LinearRegressor* lr, DataFrame* df) {
	double mae = 0, mse = 0;
	double* x = df->data[0], *y = df->data[1];
	double w = lr->w, b = lr->b;
	
	for (size_t i = 0; i < df->len_rows; i++) {
		double temp = y[i] - (w * x[i] + b);
		mae += ABS(temp);
		mse += temp * temp;
	}

	lr->mae = mae / df->len_rows;
	lr->mse = mse / df->len_rows;
}

void linear_regressor_fit(LinearRegressor* lr, DataFrame* df, double eeta, int epochs) {
	double* x = df->data[0];
	double* y = df->data[1];
	for (int i = 0; i < epochs; i++) {
		gradient_descent(lr, df, eeta);
	}
	set_cost(lr, df);
}

void linear_regressor_predict(LinearRegressor* lr, arr_double* x, arr_double* result) {
	result->len = x->len;
	for (size_t i = 0; i < x->len; i++) {
		result->ptr[i] = lr->w * x->ptr[i] + lr->b;
	}
}
