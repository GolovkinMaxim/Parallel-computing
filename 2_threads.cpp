#include <pthread.h>
#include <stdio.h>
#include <omp.h>

double a = 1.0, b = 2.0, h, *r;
int *nums, numt, n;

double f(double x)
{
  return (2+x)/(1+x*x);
}

void* worker(void* p)
{
  int my, i;
  double s; 

  my = *(int*)p;
  s = 0.0;
  for(i = my; i < n; i += numt)
    s += f(a+(i+0.5)*h);
  r[my] = s; 
  return NULL;
}

int main(int arc, char* argv[])
{
  double S;
  pthread_t *threads;
  int i, rc;
  double timein = omp_get_wtime();
  numt = 4;
  n = 100000000;
  threads = (pthread_t*)malloc(numt * sizeof(pthread_t));
  nums = (int*)malloc(numt * sizeof(int));
  r = (double*)malloc(numt * sizeof(double));
  h = (b - a) / n;

  for(i = 0; i < numt; i ++) {
    nums[i] = i;

    rc = pthread_create(threads + i, NULL, worker, nums + i);
    if(rc != 0) {
      fprintf(stderr, "pthread_create: error code %d\n", rc);
      exit(-1);
    }
  }
  for(i = 0; i < numt; i ++) { 
    rc = pthread_join(threads[i], NULL);
    if(rc != 0) {
      fprintf(stderr, "pthread_join: error code %d\n", rc);
      exit(-1);
    }
  }

  S = 0;
  for(i = 0; i < numt; i ++) 
    S += r[i];
  printf("Answer = %lf\n", S * h);
  printf("%i: Work took %f sec. time.\n", numt, omp_get_wtime() - timein);
}
