#include <stdio.h>
#include <omp.h>

double f(double x)
{
    return (2+x)/(1+x*x);
}

double integr(double a, double b, int n, double (*g)(double), int j)
{
  int i;
  double s, h;

  s = 0.;
  h = (b-a)/n;
  #pragma omp parallel for reduction(+:s)
  for(i = 0; i < n; i ++){
    s += (g(a+i*h)+g(a+(i+1)*h))/2;
  }
  return s*h;
}

int main()
{
  double A, B, r;
  int n;
  A = 1.0;
  B = 2.0;
  n = 100000000;
  for (int num_threads = 1; num_threads <= 4; num_threads++){
    double timein = omp_get_wtime();
    omp_set_num_threads(num_threads);
    r = integr(A, B, n, f, num_threads);
    printf("%i: Work took %f sec. time.\n", num_threads, omp_get_wtime()-timein);
    printf("%lf\n", r);
  }
}
