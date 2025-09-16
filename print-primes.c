/*
 print-primes.c
 By David Broman.
 Last modified: 2015-09-15
 This file is in the public domain.
*/


#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define COLUMNS 6



void print_number(int n, int *p){
  *p = 1 + *p;
  printf("%10d ", n);
  if (*p%COLUMNS == 0){
    printf("\n");
  }
}

int is_prime(int n){
    int i;
    for (i=2; i*i<= n; i++){
        if (n%i == 0)
          return 0;
    }
    return 1;
}

void print_primes(int n){
  int a = 0;
  int * p;
  p = &a;
  for (int i =2; i<=n; i++){
    int primtal = is_prime(i);
    if (primtal == 1){
      print_number(i, p);
    }
  }
  printf("\n");
}
  
// 'argc' contains the number of program arguments, and
// 'argv' is an array of char pointers, where each
// char pointer points to a null-terminated string.
int main(int argc, char *argv[]){
    if(argc == 2)
    {
        print_primes(atoi(argv[1]));
    }
  else
    printf("Please state an integer number.\n");
  return 0;
}