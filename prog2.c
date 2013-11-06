#include <time.h>
#include <stdio.h>
//  #include "mpi.h"

int prime(int rank , int size , int num)
{
    int count = 2,i,j,lo,hi;

    for( i = 6 ; i-1 <= num ; i += 6 )
    {   
        hi = 1;
        if(i+1 <= num)
        for( j = 6 ; (j-1)*(j-1) <= i+1 ; j += 6 )
        {
            if( (i+1) % (j-1) == 0 || (i+1) % (j+1) == 0 )
            {
                hi = 0;
                break;
            }
        }

        lo = 1;
        for( j = 6 ; (j-1)*(j-1) <= i-1 ; j += 6 )
        {
            if( (i-1) % (j-1) == 0 || (i-1) % (j+1) == 0 )
            {
                lo = 0;
                break;
            }
        }
        count += lo + hi;
        if( (i/6)%10000 == 0 )printf("%d,%d\n",i,count);
    }

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
