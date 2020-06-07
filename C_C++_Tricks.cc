//====Number to String====
char str[8];//enough large
sprintf(str, "%d", t);

//====Variable File Name====
ostringstream fn;
fn<<"./output/xxx_"<<k<<".JPG";
(fn.str()).c_str()
  
//=====Time Performance=====
struct timespec start, end;
clock_gettime(CLOCK_MONOTONIC_RAW, &start);
//------------------
clock_gettime(CLOCK_MONOTONIC_RAW, &end);
uint64_t delta_us = (end.tv_nsec - start.tv_nsec) / 1000;

//======Random Number========
#include <time.h>
srand((unsigned)time(NULL));
float jitter = float(rand()%100)/100 -0.5 ; //RAND_MAX is a constant defined in <cstdlib>. -0.5 to 0.5

//=====3D array declearation


//====new array with zeros
new float[10]();

//====output to console or file
ostringstream file_name;
freopen((file_name.str()).c_str(), "a", stderr);//a: append, w: new and write
fprintf(stdout, "hello\n");
fprintf(stderr, "hello\n");
