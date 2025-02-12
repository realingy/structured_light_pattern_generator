#include "head.h"

// PhaseShifting生成
class CEncoder_Phase {
 private:
  int m_numMat;      // Mat数目
  int m_pixPeriod;   // 每周期的pix数目
  cv::Mat *m_PSMat;  // phaseshifting对应的图像

  int m_resRow;      // 图像的行分辨率
  int m_resLine;     // 图像的列分辨率
  bool m_lineBased;  // 是否按照列来绘制

  std::string m_filePath;  // 存储路径名
  std::string m_matName;   // 图像名
  std::string m_matEnd;    // 图像后缀名

  bool DrawMat();    // 根据PS内容，绘制图像
  bool WriteData();  // 输出到文件

 public:
  CEncoder_Phase();
  ~CEncoder_Phase();
  bool Encode(int pixPeriod, bool lineBased);
  bool SetMatFileName(std::string filePath, std::string matName,
                      std::string matEnd);
  void Visualization();
};