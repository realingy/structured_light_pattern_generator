#include "head.h"

// 构造函数。
CEncoder_Phase::CEncoder_Phase()
{
	this->m_pixPeriod = 0;
	this->m_numMat = 0;
	this->m_PSMat = NULL;

	this->m_resRow = PROJECTOR_RESROW;
	this->m_resLine = PROJECTOR_RESLINE;
	this->m_lineBased = true;
	
	this->m_filePath = "/";
	this->m_matName = "Phase";
	this->m_matEnd = ".bmp";
}

// 析构函数。删除分配的相关空间。
CEncoder_Phase::~CEncoder_Phase()
{
	// 删除m_PSMat
	if (this->m_PSMat != NULL)
	{
		delete[](this->m_PSMat);
		this->m_PSMat = NULL;
	}
}

// 根据PS内容，绘制图像
bool CEncoder_Phase::DrawMat()
{
	using namespace cv;
	
	// 创建图像
	this->m_numMat = 4;
	this->m_PSMat = new Mat[this->m_numMat];
	for (int i = 0; i < this->m_numMat; i++)
	{
		this->m_PSMat[i].create(this->m_resRow, this->m_resLine, CV_8UC1);
	}
	
	// 绘制图像
	if (this->m_lineBased)
	{
		// 计算总周期数，并按周期绘制
		int numPeriod = this->m_resLine / this->m_pixPeriod;
		for (int T = 0; T < numPeriod; T++)
		{
			for (int pix = 0; pix < this->m_pixPeriod; pix++)
			{
				// 将pix映射到 [0, 2PI)
				double x = (double)pix / (double)(this->m_pixPeriod) * 2 * CV_PI;
				// 填充为sin(x), sin(x+pi/3), sin(x-pi/3);
				// 正弦波，每次相位移位pi/2
				for (int r = 0; r < this->m_resRow; r++)
				{
					this->m_PSMat[0].at<char>(r, pix + T*this->m_pixPeriod) = (sin(x) + 1) * 127;
					this->m_PSMat[1].at<char>(r, pix + T*this->m_pixPeriod) = (sin(x + CV_PI / 2) + 1) * 127;
					this->m_PSMat[2].at<char>(r, pix + T*this->m_pixPeriod) = (sin(x + CV_PI) + 1) * 127;
					this->m_PSMat[3].at<char>(r, pix + T*this->m_pixPeriod) = (sin(x + 3 * CV_PI / 2) + 1) * 127;
				}
			}
		}
	}
	else
	{
		// 计算总周期数，并按周期绘制
		int numPeriod = this->m_resRow / this->m_pixPeriod;
		for (int T = 0; T < numPeriod; T++)
		{
			for (int pix = 0; pix < this->m_pixPeriod; pix++)
			{
				// 将pix映射到（0, 2PI）
				double x = (double)pix / (double)(this->m_pixPeriod) * 2 * CV_PI;
				// 填充为sin(x), sin(x+pi/3), sin(x-pi/3);
				for (int l = 0; l < this->m_resLine; l++)
				{
					this->m_PSMat[0].at<char>(pix + T*this->m_pixPeriod, l) = (sin(x) + 1) * 127;
					this->m_PSMat[1].at<char>(pix + T*this->m_pixPeriod, l) = (sin(x + CV_PI / 2) + 1) * 127;
					this->m_PSMat[2].at<char>(pix + T*this->m_pixPeriod, l) = (sin(x + CV_PI) + 1) * 127;
					this->m_PSMat[3].at<char>(pix + T*this->m_pixPeriod, l) = (sin(x + 3 * CV_PI / 2) + 1) * 127;
				}
			}
		}
	}

	return true;
}

// 输出到文件
bool CEncoder_Phase::WriteData()
{
	using namespace cv;
	
	for (int i = 0; i < this->m_numMat; i++)
	{
		std::string tempNum;
		std::strstream ss;
		ss << m_numMat - i;
		ss >> tempNum;

		string tempPath = this->m_filePath;
		for (int i = 0; i < tempPath.length(); i++)
		{
			if (tempPath[i] == '/')
				tempPath[i] = '\\';
		}
		system((string("mkdir ") + tempPath).c_str());

		cv::imwrite(this->m_filePath + this->m_matName + tempNum + this->m_matEnd, this->m_PSMat[i]);
	}

	return true;
}

// 开始构建PhaseShifting。需要传入pixel周期，以及绘制方向
bool CEncoder_Phase::Encode(int pixPeriod, bool lineBased)
{
	// 判断参数是否合法，并传参。
	if (pixPeriod <= 0)
		return false;
	this->m_pixPeriod = pixPeriod;
	this->m_lineBased = lineBased; //true：列条纹 false：行条纹

	// 绘制
	if (!this->DrawMat())
		return false;

	// 保存
	if (!this->WriteData())
		return false;

	return true;
}

// 设定存储格雷码图案的文件名
bool CEncoder_Phase::SetMatFileName(string filePath, string matName, string matEnd)
{
	this->m_filePath = filePath;
	this->m_matName = matName;
	this->m_matEnd = matEnd;
	return true;
}

// 可视化
void CEncoder_Phase::Visualization()
{
	using namespace cv;

#ifdef VISUAL
	namedWindow(this->m_matName);
	for (int i = 0; i < this->m_numMat; i++)
	{
		std::cout << "Now present: " << i << std::endl;
		imshow(this->m_matName, this->m_PSMat[i]);
		cv::waitKey(400);
	}
	destroyWindow(this->m_matName);
#endif
}