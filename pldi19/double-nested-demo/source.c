
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

/* --- FUNCTIONS --- */
/* ------------------------------------------ */
/* 
 * reads :
 * writes:
 */

int f(int n, int** a) {
  for(int i = 0; i < n; ++i) {
    for(int j = 0; j < n; ++j) {
      a[i][j] = i+j;
    }
  } 
}


/* ---------------------------------------- */
int main (int argc, char* argv[])
{
    return 0;
}

