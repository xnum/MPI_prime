#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include "mpi.h"
#define LOW(id,procs,nums) ( (nums/procs) * id + id )
#define HIGH(id,procs,nums) ( (nums/procs) *(id+1) + id )
int prime(int rank , int size , unsigned int num)
{
    unsigned int i,j,k=0;

    //
    //build prime table 2~46340
    //
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
                
    unsigned int *pr = (unsigned int*)malloc(sizeof(unsigned int) * ( num / 32 / size + 1 ) );

    // compute value
    //

    for( i = 0 ; i <= num / 32 / size +1 ; ++i )
        pr[i] = 0;
    if(rank == 0){
        pr[1>>5] |= (1 << (1&0x1f)) ;
        pr[0>>5] |= (1 << (0&0x1f)) ;
    }
  
    int bound = 0;
    if(rank+1 == size)
        bound = num;
    else
        bound = HIGH(rank,size,num);


    for( j=0 ; t[j]*t[j] < num ; j++ ) 
    {
        i = t[j] * t[j];
        if( i < LOW(rank,size,num) ){
            if( LOW(rank,size,num) % t[j] != 0 )
                i = (LOW(rank,size,num)/t[j]+1)*t[j];
            else
                i = LOW(rank,size,num);
        }
        while( i <= bound ){
            int v = i - LOW(rank,size,num);
                pr[v>>5] |= (1 << (v&0x1f)) ;
            i += t[j];
        }
    }

    printf("rank=%d i=%d LOW=%d HIGH=%d\n",rank,i,LOW(rank,size,num),bound);
    int o=0;

    for( i=0 ; i + LOW(rank,size,num) <= bound ; ++i )
    {
        if( (pr[i>>5]&(1 << (i&0x1f)))  ==0){
            o++;
            //printf("%d=0\n",i + LOW(rank,size,num));
        }
    }

    printf("PP%d %d 0=%d\n",rank ,size,o);
    int result = 0;
    MPI_Reduce( &o , &result , 1 , MPI_INT , MPI_SUM , 0 , MPI_COMM_WORLD );

    if(rank == 0)
        printf("prime: %d\n",result);
    MPI_Barrier(MPI_COMM_WORLD);
    free(pr);
    free(table);
    free(t);
    return o;
}


int main(int argc, char *argv[])
{

    int rank = 0;
    int size = 1;

    unsigned int n = 800000000;

    MPI_Init(&argc,&argv);
    MPI_Comm_size(MPI_COMM_WORLD,&size);
    MPI_Comm_rank(MPI_COMM_WORLD,&rank);

    double time = MPI_Wtime();
    int result = prime(rank,size,n);

    time = MPI_Wtime()-time;
    printf("time= %lf s\n",time);

    return 0;
}
