#include <stdlib.h>
#include <time.h>
#include <stdio.h>

int prime(int rank , int size , unsigned int num)
{
    unsigned int i,j,k=0;

    int *table = (int*)calloc(46341 , sizeof(int))
       ,*t     = (int*)calloc( 4792 , sizeof(int));

    int c = 0;
    for(i=2 ; i<46341 ; ++i)
        if( 0==table[i] )
        {
            t[c++] = i;
            for(k = 46340/i,j=i*k;k>=i;--k,j-=i)
                if(table[k] == 0)
                    table[j] = 1;
        }
                
    unsigned int *pr = (unsigned int*)calloc(num / 32 + 1 , sizeof(unsigned int));

    for( j=0 ; j<c ; ++j )
        for(i=t[j]*t[j] ; i<=num; i += (t[j]<<1) )
                pr[i>>5] |= (1 << (i&0x1f)) ;

    int o=0;

    for( i=3 ; i <= num ; i += 2 )
        if( (pr[i>>5]&(1 << (i&0x1f)))  == 0 )
            o++;

    free(pr);
    free(table);
    free(t);
    return o+1;
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
