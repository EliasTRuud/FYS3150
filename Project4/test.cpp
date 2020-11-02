#include <cstdio>
#include <omp.h>

int main(int argc, char const *argv[]) {

  #pragma omp parallel
  {
    printf("Hello World... from thread = %d\n", omp_get_thread_num());
  }
  return 0;
}
