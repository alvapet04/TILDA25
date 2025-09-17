/*
 Skrivare: Hanna Wike
*/

#include <stdio.h>
#include <stdlib.h>

#define COLUMNS 6



void print_number(int n, int *p){
  *p = 1 + *p;
  printf("%10d ", n);
  if (*p%COLUMNS == 0){
    printf("\n");
  }
}

void print_sieves(int n){
    int a = 0;
    int * p;
    p = &a;
    char prim_array[n];
    for(int i=2; i<n; i++){
        prim_array[i]= 1;
    }
    for (int i=2;i*i<=n;i++){
        if (prim_array[i] == 1){
            for(int j=i*i;j<n;j+=i){
                prim_array[j]=0;
            }
        }
    }

    for(int i=2; i<n; i++){
        if (prim_array[i] == 1){
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
        print_sieves(atoi(argv[1]));
    }
  else
    printf("Please state an integer number.\n");
  return 0;
}