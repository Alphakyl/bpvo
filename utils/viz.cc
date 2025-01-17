#include "bpvo/utils.h"
#include "utils/viz.h"

#include <opencv2/opencv.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>

namespace bpvo {

void colorizeDisparity(const cv::Mat& src, cv::Mat& dst, double min_d, double num_d)
{
  CV_Assert(src.type() == CV_32F);

  double scale = 0.0;
  if(num_d > 0) {
    scale = 255.0 / num_d;
  } else {
    double max_val = 0;
    cv::minMaxLoc(src, nullptr, &max_val);
    scale = 255.0 / max_val;
  }

  src.convertTo(dst, CV_8U, scale);
  cv::applyColorMap(dst, dst, cv::COLORMAP_JET);

  for(int y = 0; y < src.rows; ++y)
    for(int x = 0; x < src.cols; ++x)
      if(src.at<float>(y,x) <= min_d)
        dst.at<cv::Vec3b>(y,x) = cv::Vec3b(0,0,0);
}

void overlayDisparity(const cv::Mat& I, const cv::Mat& D, cv::Mat& dst,
                      double alpha, double min_d, double num_d)
{
  cv::Mat image;
  switch( I.type() )
  {
    case CV_8UC1:
      cv::cvtColor(I, image, cv::COLOR_GRAY2BGR);
      break;
    case CV_8UC3:
      image = I;
      break;
    case CV_8UC4:
      cv::cvtColor(I, image, cv::COLOR_BGRA2BGR);
      break;
    default:
      throw std::runtime_error("Unsupported image type");
  }

  cv::addWeighted(image, alpha, colorizeDisparity(D, min_d, num_d), 1.0-alpha, 0.0, dst);
}

} // bpvo
