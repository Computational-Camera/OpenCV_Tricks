int64 pre_timer, cur_timer;
pre_timer = cur_timer; 
cur_timer = cv::getTickCount();
cout<<"Time spent "<<(cur_timer-pre_timer)/ cv::getTickFrequency()<<" Seconds  "<<endl;  

//random number
#include <time.h>
srand((unsigned)time(NULL));
float jitter = float(rand()%100)/100 -0.5 ; //RAND_MAX is a constant defined in <cstdlib>. -0.5 to 0.5

