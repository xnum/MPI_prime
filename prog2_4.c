#include <stdlib.h>
#include <time.h>
#include <stdio.h>
//  #include "mpi.h"

int prime(int rank , int size , unsigned int num)
{
    unsigned int i,j,k=0;

    int *table = (int*)malloc(46341 * sizeof(int)),*t= (int*)malloc(4792*sizeof(int));

    table[0] = 1;
    table[1] = 1;

    for( i=2 ; i<46341 ; ++i )
        table[i] = 0;
    

    int c = 0;
    for(i=2 ; i<46341 ; ++i)
        if( 0==table[i] )
        {
            t[c++] = i;
            for(k = 46340/i,j=i*k;k>=i;--k,j-=i)
                if(table[k] == 0)
                    table[j] = 1;
        }
                
    unsigned int *pr = (unsigned int*)malloc(sizeof(unsigned int) * (1<<27) );

    for( i = 0 ; i <= 1<<27 ; ++i )
        pr[i] = 0;
   
    for( j=0 ; j<c ; ++j )
        for(i=t[j]*t[j] ; i<=num; i += t[j] )
                pr[i>>5] |= (1 << (i&0x1f)) ;

    int o=0;

    for( i=2 ; i <= num ; ++i )
    {
        if( (pr[i>>5]&(1 << (i&0x1f)))  ==0)
            o++;
    }

    printf("prime=%d \n",o);

    free(pr);
    free(table);
    free(t);
    return o;
}


int main(int argc, char *argv[])
{

    int rank = 0;
    int size = 1;

    unsigned int n = 2147483647;
    puts("INPUT:");
    while( scanf(" %u",&n) != EOF ){
        clock_t start , end;
        start = clock();
        int result = prime(rank,size,n);
        end = clock();
        float ti =(float) (end-start) / CLOCKS_PER_SEC;
        printf("prime=%d time= %f s\n",result,ti);
    }
    return 0;
}
