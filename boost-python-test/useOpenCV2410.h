#ifndef __USE_OPENCV2410_H__
#define __USE_OPENCV2410_H__

#pragma once

//#include "opencv2/opencv.hpp"

#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
// #include "opencv2/core/core_c.h"
//#include "opencv2/highgui/highgui_c.h"
//#include "opencv2/imgproc/imgproc_c.h"
//#include "opencv2/video/video.hpp"
//#include "opencv2/videostab/videostab.hpp"
//#include "opencv2/features2d/features2d.hpp"
#include "opencv2/objdetect/objdetect.hpp"
#include "opencv2/nonfree/nonfree.hpp"

//#include "opencv2/flann/miniflann.hpp"
//#include "opencv2/flann/flann.hpp"
//#include "opencv2/photo/photo.hpp"
//#include "opencv2/calib3d/calib3d.hpp"
//#include "opencv2/ml/ml.hpp"
//#include "opencv2/contrib/contrib.hpp"
//#include "opencv2/ts/ts.hpp"
//#include "opencv2/stitching/stitcher.hpp"
//#include "opencv2/legacy/legacy.hpp"

#ifdef _DEBUG
#pragma comment(lib,"opencv_core2410d.lib")
#pragma comment(lib,"opencv_highgui2410d.lib")
#pragma comment(lib,"opencv_imgproc2410d.lib")
//	#pragma comment(lib,"opencv_video248d.lib")
#pragma comment(lib,"opencv_nonfree2410d.lib")
//	#pragma comment(lib,"opencv_videostab248d.lib")
#pragma comment(lib,"opencv_features2d2410d.lib")
 #pragma comment(lib,"opencv_objdetect2410d.lib")
//	#pragma comment(lib,"opencv_flann248d.lib")
//	#pragma comment(lib,"opencv_photo248d.lib")
//#pragma comment(lib,"opencv_calib3d248d.lib")
//	#pragma comment(lib,"opencv_ml248d.lib")
//	#pragma comment(lib,"opencv_contrib248d.lib")
//	#pragma comment(lib,"opencv_ts248d.lib")
//	#pragma comment(lib,"opencv_stitching248d.lib")
//	#pragma comment(lib,"opencv_legacy248d.lib")
#else
#pragma comment(lib,"opencv_core2410.lib")
#pragma comment(lib,"opencv_highgui2410.lib")
#pragma comment(lib,"opencv_imgproc2410.lib")
//	#pragma comment(lib,"opencv_video248.lib")
#pragma comment(lib,"opencv_nonfree2410.lib")
//	#pragma comment(lib,"opencv_videostab248.lib")
#pragma comment(lib,"opencv_features2d2410.lib")
  #pragma comment(lib,"opencv_objdetect2410.lib")
//	#pragma comment(lib,"opencv_flann248.lib")
//	#pragma comment(lib,"opencv_photo248.lib")
//#pragma comment(lib,"opencv_calib3d248.lib")
//	#pragma comment(lib,"opencv_ml248.lib")
//	#pragma comment(lib,"opencv_contrib248.lib")
//	#pragma comment(lib,"opencv_ts248.lib")
//	#pragma comment(lib,"opencv_stitching248.lib")
//	#pragma comment(lib,"opencv_legacy248.lib")
#endif

using namespace cv;
using namespace std;

#endif //__USE_OPENCV248_H__