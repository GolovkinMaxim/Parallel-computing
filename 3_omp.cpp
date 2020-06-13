#include <stdio.h>
#include <omp.h>
#include <cstdlib>

double f (double x, double y)
{
  return x*x + (y-2) * (y-2);
}

int main(){
    double a1, a2, b1, b2, h1, h2, *mins, minimum;
    int n, num_threads;
    time_t start, end;
    a1 = 0.0;
    b1 = 4.0;
    a2 = -1.0;
    b2 = 1.0;
    n = 20000;
	num_threads = 4;
    h1 = (b1-a1)/n;
    h2 = (b2-a2)/n;
    double timein = omp_get_wtime();
    omp_set_num_threads(num_threads);
    mins = (double *) malloc(sizeof(double) * num_threads);
    for(int i = 0; i < num_threads; i++){
		mins[i] = f(a1,a2);
	}
	#pragma omp parallel for
    	for(int i = 0; i <= n; i++){
        	for(int j = 0; j <= n; j++){
        		int th_num = omp_get_thread_num();
        		double t = f(a1+i*h1,a2+j*h2);
            	if (t < mins[th_num]) {
					mins[th_num] = t;
				}
            }
        }
	minimum = mins[0];
	for (int i = 1; i < num_threads; i++)
		if (mins[i] < minimum)
			minimum = mins[i];
    printf("%lf\n", minimum);
    printf("%i: Work took %f sec. time.\n", num_threads, omp_get_wtime() - timein);
}
