#===face detection===
const std::string caffeConfigFile = "./data/deploy.prototxt"; 
const std::string caffeWeightFile = "./data/res10_300x300_ssd_iter_140000_fp16.caffemodel";
cv::dnn::Net net  = cv::dnn::readNetFromCaffe(caffeConfigFile, caffeWeightFile);
const cv::Scalar meanVal(104.0, 177.0, 123.0);
const size_t inWidth = 300;
const size_t inHeight = 300;
const double inScaleFactor = 1.0;
const float confidenceThreshold = 0.7;
cv::Mat inputBlob = cv::dnn::blobFromImage(img, inScaleFactor, cv::Size(inWidth, inHeight), meanVal, false, false);

net.setInput(inputBlob, "data");
cv::Mat detection = net.forward("detection_out");

#===mat2tensor===
cv::cvtColor(img_crop, img_crop, cv::COLOR_BGR2RGB);//brg to rgb
img_crop.convertTo(img_crop, CV_32FC3);  
img_crop = img_crop/255.0 - cv::Scalar(0.3387, 0.2865, 0.2553); 
  
float* tmp = new float[img_crop.cols * img_crop.rows * 3];
float* tmp2 = tmp;
for (int k=0; k<3; k++){
  for (int j=0; j<img_crop.rows; j++){
        for (int i=0; i<img_crop.cols; i++){
            *tmp2++ = img_crop.at<cv::Vec3f>(j,i).val[k];     
        }
    }
  }
at::Tensor tensor_image = torch::from_blob(tmp, {1, 3, img_crop.rows, img_crop.cols}, at::kFloat).to(at::kCUDA);
