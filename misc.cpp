int64 pre_timer, cur_timer;
pre_timer = cur_timer; 
cur_timer = cv::getTickCount();
cout<<"Time spent "<<1000000000/(cur_timer-pre_timer)<<" Seconds  "<<endl;  
