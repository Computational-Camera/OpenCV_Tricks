int64 pre_timer, cur_timer;
pre_timer = cur_timer; 
cur_timer = cv::getTickCount();
cout<<"Time spent "<<(cur_timer-pre_timer)/ cv::getTickFrequency()<<" Seconds  "<<endl;  
