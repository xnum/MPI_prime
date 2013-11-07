#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include <string.h>
#include "mpi.h"
#define LOW(id,procs,nums) ( (nums/procs) * id + id )
#define HIGH(id,procs,nums) ( (nums/procs) *(id+1) + id )
int prime(int rank , int size , unsigned int num)
{
    unsigned int i,j,k=0;

    double time = MPI_Wtime();
    int *table = (int*)calloc(46341 , sizeof(int))
      , *t     = (int*)calloc( 4792 , sizeof(int));

    table[0] = 1;
    table[1] = 1;

    int c = 0;
    for(i=2 ; i<46341 ; ++i)
        if( 0==table[i] )
        {
            t[c++] = i;
            for(k = 46340/i,j=i*k;k>=i;--k,j-=i)
                if(table[k] == 0)
                    table[j] = 1;
        }
    
    int block = num / 32 / size + 1 ;
    unsigned int *pr = (unsigned int*)calloc(block,sizeof(unsigned int));

    if(rank == 0){
        pr[1>>5] |= (1 << (1&0x1f)) ;
        pr[0>>5] |= (1 << (0&0x1f)) ;
    }
  
    int bound = 0;
    int high_bound = HIGH(rank,size,num);
    int low_bound = LOW(rank,size,num);
    if(rank+1 == size)
        bound = num - low_bound;
    else
        bound = high_bound - low_bound;

    time = MPI_Wtime()-time;
    printf("time after build table= %lf s\n",time);
    time = MPI_Wtime();

    for( j=1 ; t[j]*t[j] < num ; j++ ) 
    {
        i = t[j] * t[j];
        if( i < low_bound ){
            if( low_bound % t[j] != 0 )
                i = (low_bound/t[j]+1)*t[j] - low_bound;
            else
                i = 0;
        }
        else
            i -= low_bound;
        if( (i+low_bound) % 2 == 0 )i += t[j];
        while( i <= bound ){
            pr[i>>5] |= (1 << (i&0x1f)) ;
            i += (t[j]<<1);
        }
    }

    time = MPI_Wtime()-time;
    printf("time after calu= %lf s\n",time);
    time = MPI_Wtime();
    int o=0;

    /*
    for( i = 0 ; i <= ((bound-low_bound)>>5)-1 ; ++i )
    {
        unsigned int tmp = ~pr[i];
        while(tmp)
        {
            if(tmp&1)
                o++;
            tmp = tmp >> 1;
        }
    }
    */

    //for( i= (((bound-low_bound)>>5)<<5) ; i <= bound-low_bound ; ++i )
    for( i = !(low_bound&1) ; i <= bound ; i += 2 )
    {
        if( (pr[i>>5]&(1 << (i&0x1f)))  ==0){
            o++;
            //printf("%d=0\n",i+low_bound);
        }
    }


    int result = 0;

    MPI_Reduce( &o , &result , 1 , MPI_INT , MPI_SUM , 0 , MPI_COMM_WORLD );
    if(rank == 0)
        printf("prime: %d\n",result+1);
    
    MPI_Barrier(MPI_COMM_WORLD);
    
    time = MPI_Wtime()-time;
    printf("time after barrier= %lf s\n",time);
    free(pr);
    free(table);
    free(t);
    return o;
}


int main(int argc, char *argv[])
{

    int rank = 0;
    int size = 1;

    unsigned int n = 2100000000;

    MPI_Init(&argc,&argv);
    MPI_Comm_size(MPI_COMM_WORLD,&size);
    MPI_Comm_rank(MPI_COMM_WORLD,&rank);

    double time = MPI_Wtime();
    int result = prime(rank,size,n);

    time = MPI_Wtime()-time;
    printf("time= %lf s\n",time);

    MPI_Finalize();
    return 0;
}
