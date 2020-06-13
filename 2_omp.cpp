#include <stdio.h>
#include <omp.h>

double func(double x)
{
    return (2+x)/(1+x*x);
}

double integr(double a, double b, int n, double (*g)(double), int j)
{
  int i;
  double s, h;

  s = 0.;
  h = (b - a) / n;
  #pragma omp parallel for reduction(+:s) num_threads(j)
  for(i = 0; i < n; i ++){
    s += g(a + (i+0.5) * h);
  }
  return s * h;
}

int main()
{
  double A, B, v;
  int j, N;

  A = 1.0;
  B = 2.0;
  N = 100000000; 
  for (j = 1; j <= 4; j++){
    double timein = omp_get_wtime();
    v = integr(A, B, N, func, j);
    printf("%i: Work took %f sec. time.\n", j, omp_get_wtime()-timein);
    printf("%lf\n", v);
  }
}
