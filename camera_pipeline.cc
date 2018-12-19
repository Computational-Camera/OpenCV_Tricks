#denoise CV3 BOOK page 327
fastNlMeansDenoising (src, dst, h, window_size, search_window_size) //3,7,21 , color weight decay
#first convert to LAB and hColor decay apply to luminance channel
fastNlMeansDenoisingColor (src, dst, h, hColor, window_size, search_window_size) //3,3,7,21
#burst
fastNlMeansDenoisingMulti()
fastNlMeansDenoisingColoredMulti()
