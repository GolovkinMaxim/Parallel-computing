#include <string.h>
#include <stdio.h>
#include <cstdlib>
#include <omp.h>

int main(){
    int n, num_threads;
    n = 500;
    num_threads = 4;
    double **A, **B, **C;
    double timein = omp_get_wtime();
    omp_set_num_threads(num_threads);
    A = new double*[n];
    B = new double*[n];
    C = new double*[n];
	#pragma omp parallel for shared (A, B, C)
    	for(int i = 0; i < n; i++){
    		A[i] = new double[n];
    		B[i] = new double[n];
    		C[i] = new double[n];
        	for(int j = 0; j < n; j++){
        		A[i][j] = (double)rand()/ RAND_MAX*10;
        		B[i][j] = (double)rand()/ RAND_MAX*10;
        		C[i][j] = 0;
			}
        }
	#pragma omp parallel for shared (A, B, C)
    	for(int i = 0; i < n; i++){
        	for(int j = 0; j < n; j++){
        		for(int k = 0; k < n; k++){
        			C[i][j] += A[i][k] * B[k][j];
				}
			}	
        }
    printf("%i: Work took %f sec. time.\n", num_threads, omp_get_wtime() - timein);
    /*printf("\n");
	for(int i = 0; i < n; i++){
        for(int j = 0; j < n; j++){
		printf("%f ", C[i][j]);
    	}
    	printf("\n");
    }*/
}
