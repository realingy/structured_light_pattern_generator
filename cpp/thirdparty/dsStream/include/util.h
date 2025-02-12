#ifndef REAL_FRAMEWORK_UTIL_H_
#define REAL_FRAMEWORK_UTIL_H_

// #include <ds_rx_camera.h>
#include <QImage>
#include <QMap>
#include <opencv2/core.hpp>
#include <string>

QString getCWD(const char* aSuffix);
void sleep_(int aMs);

template <typename K, typename T>
T* tryFind(QMap<K, T>* aMap, const K& aKey) {
  auto ret = aMap->find(aKey);
  if (ret == aMap->end()) {
    aMap->insert(aKey, T());
    ret = aMap->find(aKey);
  }
  return &(*ret);
}

namespace scv {

QImage cvMat2QImage(const cv::Mat& mat);
cv::Mat QImage2cvMat(QImage image);

}  // namespace scv

#endif
