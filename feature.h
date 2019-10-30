

  SimpleBlobDetector::Params params;
	// Change thresholds
	params.minThreshold = 64;
	params.maxThreshold = 168;
	params.filterByArea = true;
	params.minArea = 512;
	params.filterByCircularity = false;
	params.minCircularity = 0.5;
	params.filterByConvexity = true;
	params.minConvexity = 0.5;
	params.filterByInertia = true;
	params.minInertiaRatio = 0.1;
	
  vector<KeyPoint> keypoints;
	Ptr<SimpleBlobDetector> detector = SimpleBlobDetector::create(params);
	detector->detect(img, keypoints);
