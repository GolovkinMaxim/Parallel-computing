#include <stdio.h>
#include <cstdlib>
#include <omp.h>

int main(){
    int n, num_threads;
    n = 1000;
    num_threads = 4;
    omp_set_num_threads(num_threads);
    double **A = new double*[n];
    double *b = new double[n];
    double *x = new double[n];
    double timein = omp_get_wtime();
	#pragma omp parallel for shared (A, b)
    	for(int i = 0; i < n; i++){
    		A[i] = new double[n];
    		b[i] = (double)rand()/ RAND_MAX*10;
        	for(int j = 0; j < n; j++){
        		A[i][j] = (double)rand()/ RAND_MAX*10;
			}
        }

	#pragma omp parallel
	for (int k = 0; k < n; k++){
		#pragma omp for
		for (int j = k + 1; j < n; j++){
			double d = A[j][k] / A[k][k];
			for (int i = k; i < n; i++){
				A[j][i] = A[j][i] - d * A[k][i];
			}
			b[j] = b[j] - d * b[k];
		}
	}
	#pragma omp parallel
	for (int k = n-1; k >= 0; k--){
		double d = 0;
		#pragma omp for
		for (int j = k + 1; j < n; j++){
			double s = A[k][j] * x[j]; 
			d = d + s;
		}
		x[k] = (b[k] - d) / A[k][k];
	}
    printf("%i: Work took %f sec. time.\n", num_threads, omp_get_wtime()-timein);
	/*for(int i = 0; i < n; i++){
		printf("%f\n", x[i]);
    	}*/
}
