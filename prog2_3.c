#include <stdlib.h>
#include <time.h>
#include <stdio.h>
//  #include "mpi.h"

int prime(int rank , int size , int num)
{
    int count = 2,i,j,lo,hi;

    int *table = (int*)malloc(46341 * sizeof(int)),*t= (int*)malloc(4792*sizeof(int));

    table[0] = 1;
    table[1] = 1;

    for( i=2 ; i<46341 ; ++i )
        table[i] = 0;
    

    for(i=2 ; i<46341 ; ++i)
    {
        if( 0==table[i] )
        {
            for(lo = 46340/i,j=i*lo;lo>=i;--lo,j-=i)
                if(table[lo] == 0)
                    table[j] = 1;
        }
    
    }
    
    int c = 0;
    for( i=0 ; i<46340 ; ++i )
    {
        if(table[i] == 0){
            t[c++] = i;
        }
    }
//    printf("%d\n",c);
    
    for( i=0 ; i<50 ; ++i )
    {
//        printf("%d\n",t[i]);
    }
    

    for( i = 6 ; i-1 <= num ; i += 6 )
    {   
       lo = 1;
       hi = 1;

        for( j = 0 ; j<c && t[j]*t[j]<=i-1 ; ++j )
        {
            if( (i-1) % t[j] == 0 )
            {
                lo = 0;
                break;
            }
        }

        for( j = 0 ; j<c && t[j]*t[j]<=i+1; ++j )
        {
            if( (i+1) % t[j] == 0 )
            {
                hi = 0;
                break;
            }
        }
        count += lo + hi;
        if( (i/6)%1000000 == 0 )printf("%d,%d\n",i,count);
    }
        free(table);
        free(t);
        return count;
}


int main(int argc, char *argv[])
{

    int rank = 0;
    int size = 1;

    int n = 2147483647;

    while( scanf(" %d",&n) != EOF ){
        clock_t start , end;
        start = clock();
        int result = prime(rank,size,n);
        end = clock();
        float ti =(float) (end-start) / CLOCKS_PER_SEC;
        printf("%d %f\n",result,ti);
    }
    return 0;
}
