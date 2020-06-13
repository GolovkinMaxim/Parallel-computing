#include <pthread.h>
#include <stdio.h>
#include <cstdlib>
#include <omp.h>

double a1, a2, b1, b2, h1, h2, *mins, minimum;
int *nums, num_threads, n, rc;

double f(double x, double y)
{
  return x*x + (y-2) * (y-2);
}

void* worker(void *p)
{
  int my = *(int*)p;
  mins[my] = f(a1,a2);
  for(int i = my; i <= n; i += num_threads){
        	for(int j = 0; j <= n; j++){
        		double t = f(a1+i*h1,a2+j*h2);
            	if (t < mins[my]) {
					mins[my] = t;
				}
            }
        }
  return NULL;
}

int main(int arc, char* argv[])
{
    pthread_t *threads;
    double timein = omp_get_wtime();
    a1 = 0.0;
    b1 = 4.0;
    a2 = -1.0;
    b2 = 1.0;
    n = 20000;
	num_threads = 4;
    h1 = (b1-a1)/n;
    h2 = (b2-a2)/n;
    threads = (pthread_t*)malloc(num_threads * sizeof(pthread_t));
    nums = (int*)malloc(num_threads * sizeof(int));
    mins = (double*)malloc(num_threads * sizeof(double));
  
    for(int i = 0; i < num_threads; i ++) {
    	nums[i] = i;
        rc = pthread_create(threads + i, NULL, worker, nums + i);
        if(rc != 0) {
            fprintf(stderr, "pthread_create: error code %d\n", rc);
            exit(-1);
        }
    }
    for(int i = 0; i < num_threads; i ++) { 
        rc = pthread_join(threads[i], NULL);
        if(rc != 0) {
            fprintf(stderr, "pthread_join: error code %d\n", rc);
            exit(-1);
        }
    }

    double minimum = mins[0];
    for(int i = 0; i < num_threads; i ++){
        if (mins[i] < minimum){
        minimum = mins[i];
        }
    }
  printf("%lf\n", minimum);
  printf("%i: Work took %f sec. time.\n", num_threads, omp_get_wtime() - timein);
}
