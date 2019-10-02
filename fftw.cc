#include <stdio.h>
#include <fftw3.h>

// Print 1D complex array
void print_complex_1d(fftw_complex* arr, int size, int normalize){
	int i;
	for (i = 0; i < size; i++){
		double a = arr[i][0] / (normalize ? size : 1);
		double b = arr[i][1] / (normalize ? size : 1);
		printf(" {%2.1f, %2.1f} \n", a, b);
	}
	printf("\n");
}

// Print 2D complex array
void print_complex_2d(fftw_complex* arr, int rows, int cols, int normalize){
	int i, j, k, size = rows*cols;
	for (i = 0, k = 0; i < rows; i++){
		for (j = 0; j < cols; j++, k++){
			double a = arr[k][0] / (normalize ? size : 1);
			double b = arr[k][1] / (normalize ? size : 1);
			printf(" {%2.1f, %2.1f} ", a, b);
		}
		printf("\n");
	}
	printf("\n");
}

// Print 1D real array
void print_real_1d(double* arr, int size, int normalize){
	int i;
	for (i = 0; i < size; i++)
		printf(" %2.1f ", arr[i] / (normalize ? size : 1));
	printf("\n\n");
}

// Print 2D real array
void print_real_2d(double* arr, int rows, int cols, int normalize){
	int i, j, k, size = rows*cols;
	for (i = 0, k = 0; i < rows; i++){
		for (j = 0; j < cols; j++, k++)
			printf(" %2.1f ", arr[k] / (normalize ? size : 1));
		printf("\n");
	}
	printf("\n\n");
}

#define WW 4
#define HH 4
#define WH 16

bool fft_inverse_img(){
	fftw_complex* b;
	fftw_plan			plan_f, plan_b;

	double *a = fft_data.ptr<double>(0);
	double *c = ifft_data.ptr<double>(0);	
	//double				a[WH], c[WH];
	int 					i, j, k;

	/* This is the 2D real array, suppose WW is odd number*/
	double in[HH][WW] = {   { 0.0, 0.0, 4.0, 0.0},
		                    { 0.0, 1.0, 0.0, 0.0},
			                { 0.0, 0.0, 0.0, 0.0},	                  
			                { 2.0, 0.0, 0.0, 0.0}};

	/* Allocate array for DFT result */
	b = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * HH * (WW/2+1)) ; ///(2+1)

	/* Create plans */
	plan_f = fftw_plan_dft_r2c_2d(WW, HH, a, b, FFTW_ESTIMATE);
	plan_b = fftw_plan_dft_c2r_2d(WW, HH, b, c, FFTW_ESTIMATE);

	/* Populate the input data in row-major order */
	for (i = 0, k = 0; i < HH; i++)
		for (j = 0; j < WW; j++, k++)
			a[k] = in[i][j];

	/* Compute forward & inverse DFT */
	fftw_execute(plan_f);

	/* Print results */
	printf("Input:\n"); print_real_2d(&in[0][0], HH, WW, 0);
	printf("DFT:\n");   print_complex_2d(b, HH, WW/2 +1, 0); //due to symmetry
	fftw_execute(plan_b);	
	printf("IDFT:\n");  print_real_2d(c, WW, HH, 1);

	/* Free memory */
	fftw_destroy_plan(plan_f);
	fftw_destroy_plan(plan_b);
	fftw_free(b);
