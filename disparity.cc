      	int32_t width  = im11c.cols;
      	int32_t height = im11c.rows;

	Mat disparity;
	timespec ts1,ts2;
      	clock_gettime(CLOCK_REALTIME, &ts1); // Works on Linux	
	
	int wsize = 5;
      	Mat filtered_disp, left_disp, right_disp;
	    Ptr<StereoSGBM> left_matcher  = StereoSGBM::create(-16,64,wsize);
      	left_matcher->setP1(16*wsize*wsize);
      	left_matcher->setP2(128*wsize*wsize);
      	left_matcher->setPreFilterCap(16);
      	left_matcher->setMode(StereoSGBM::MODE_SGBM);
      	Ptr<StereoMatcher> right_matcher = createRightMatcher(left_matcher);

      	left_matcher-> compute(im11c, im22c, left_disp);
      	right_matcher->compute(im22c, im11c, right_disp);
        
     	Ptr<DisparityWLSFilter> wls_filter;
      	wls_filter = createDisparityWLSFilter(left_matcher); 	   
      	wls_filter->setLambda(2000.0);
      	wls_filter->setSigmaColor(2);
      	wls_filter->filter(left_disp,im11c,filtered_disp,right_disp);
        
	clock_gettime(CLOCK_REALTIME, &ts2); // Works on Linux
	float diff_time = (ts2.tv_nsec - ts1.tv_nsec);
	diff_time = (diff_time>0) ? diff_time/1000000000.f   + (ts2.tv_sec - ts1.tv_sec): 
                                  diff_time/1000000000.f+1 + (ts2.tv_sec - ts1.tv_sec)-1;

     	cout<< "Disparity spent "<<diff_time<<"s"<<endl; //lscpu | grep MHz

      	Mat temp = Mat::zeros(im11c.rows, im11c.cols, CV_8U);
      	Rect ROI = Rect(48,48, im11c.cols-96, im11c.rows-96);
      	filtered_disp(ROI).copyTo(temp(ROI));

 	// Apply the colormap:
        applyColorMap(img0, cm_img0, COLORMAP_BONE);//COLORMAP_JET
