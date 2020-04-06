#include "head.h"

using namespace std;

// 构造函数
CEncoder_Gray::CEncoder_Gray()
{
	this->numDigit = 0;
	this->grayCode = NULL;
	this->grayCodeMat = NULL;
	this->resRow = PROJECTOR_RESROW;
	this->resLine = PROJECTOR_RESLINE;
	this->lineBased = true;

	this->m_filePath = "/";
	this->m_codeName = "GrayCode";
	this->m_codeEnd = ".txt";
	this->m_matName = "Gray";
	this->m_matEnd = ".bmp";
}

// 析构函数，删除分配的相关空间
CEncoder_Gray::~CEncoder_Gray()
{
	// 删除GrayCode
	if (this->grayCode != NULL)
	{
		delete[](this->grayCode);
		this->grayCode = NULL;
	}
	
	// 删除GrayCodeMat
	if (this->grayCodeMat != NULL)
	{
		delete[](this->grayCodeMat);
		this->grayCodeMat = NULL;
	}
}

// 获取Code中的右起第n位，n从1开始
bool CEncoder_Gray::GetBit(short Code, int n)
{
	bool res = (Code >> (n - 1)) & 0x0001;
	return res;
}

// 构建numDigit位的GrayCode并存储
bool CEncoder_Gray::EncodeGray()
{
	// 创建格雷码空间并初始化
	this->grayCode = new short[this->grayCodeSize]; //64

	// 开始循环填充
	this->grayCode[0] = 0x0000;		// 对第一个填充全0
	this->grayCode[1] = 0x0001;		// 改变最右端的值
	for (int i = 2; i < this->grayCodeSize; i += 2)
	{
		// 对GrayCode[i]: 首先，找到右边第一个为1的数
		int k = 1;
		while (true)
		{
			if (this->GetBit(this->grayCode[i - 1], k))
				break;
			k++;
		}
		if (k >= 16)	return false;
		// 对GrayCode[i]: 然后，改变这个数左边一位的值
		short Cover = 0x0001 << k;
		grayCode[i] = grayCode[i - 1] ^ Cover;

		// 对GrayCode[i+1]：改变最右端的值
		grayCode[i + 1] = grayCode[i] ^ 0x0001;
	}

	// 逐个显示格雷码
	/*
	for (int i = 0; i < this->grayCodeSize; i++)
	{
		std::cout << "==> " << i << " :: " << grayCode[i] << std::endl;
	}
	*/

	return true;
}

// 根据格雷码内容，绘制格雷码图像
bool CEncoder_Gray::DrawMat()
{
	// 创建numDigit*2个图像
	this->grayCodeMat = new cv::Mat[this->numDigit*2];

	// 绘制每一个图像。picIdx代表绘制的是格雷码的第picIdx / 2位。也代表第几幅矩阵
	for (int picIdx = 0; picIdx < this->numDigit * 2; picIdx += 2)
	{
		// 创建图像
		this->grayCodeMat[picIdx].create(this->resRow, this->resLine, CV_8UC1);
		this->grayCodeMat[picIdx + 1].create(this->resRow, this->resLine, CV_8UC1);

		// 计算间距
		if (lineBased)	// 如果是绘制按列的图像
		{
			int space = this->resLine / grayCodeSize;
			for (int i = 0; i < grayCodeSize; i++)
			{
				// 判断绘制颜色
				char color = 0;
				if (this->GetBit(grayCode[i], picIdx / 2 + 1))
					color = 0xFF;
				else
					color = 0;

				// 绘制
				for (int l = 0; l < space; l++)
				{
					for (int r = 0; r < this->resRow; r++)
					{
						this->grayCodeMat[picIdx].at<char>(r, l + i*space) = color;
					}
				}
			}
		}
		else
		{
			int space = this->resRow / grayCodeSize;
			for (int i = 0; i < grayCodeSize; i++)
			{
				// 判断绘制颜色
				char color = 0;
				if (this->GetBit(grayCode[i], picIdx / 2 + 1))
					color = 0xFF;
				else
					color = 0;

				// 绘制
				for (int r = 0; r < space; r++)
				{
					for (int l = 0; l < this->resLine; l++)
					{
						this->grayCodeMat[picIdx].at<char>(r + i*space, l) = color;
					}
				}
			}
		}

		// 创建反色图像
		for (int r = 0; r < this->resRow; r++)
		{
			for (int l = 0; l < this->resLine; l++)
			{
				uchar value = this->grayCodeMat[picIdx].at<uchar>(r, l);
				if (value == 0xFF)
					this->grayCodeMat[picIdx + 1].at<uchar>(r, l) = 0;
				else
					this->grayCodeMat[picIdx + 1].at<uchar>(r, l) = 0xFF;
			}
		}
	}

	return true;
}

// 输出到文件
bool CEncoder_Gray::WriteData()
{
	// 创建路径
	string tempPath = this->m_filePath;
	for (int i = 0; i < tempPath.length(); i++)
	{
		if (tempPath[i] == '/')
			tempPath[i] = '\\';
	}
	system((string("mkdir ") + tempPath).c_str());

	// 输出生成的GrayCode到txt
	std::fstream txtFile;
	txtFile.open((this->m_filePath + this->m_codeName + this->m_codeEnd).c_str(), ios::out);
	if (!txtFile)
		return false;
	for (int i = 0; i < this->grayCodeSize; i++)
	{
		//txtFile << i << '\t' << this->grayCode[i] << std::endl ;
		txtFile << i << '\t';
		for (int d = this->numDigit; d > 0; d--)
		{
			txtFile << this->GetBit(this->grayCode[i], d);
		}
		txtFile << std::endl;
	}
	txtFile.close();

	// 输出生成的GrayCodeMat到bmp
	for (int i = 0; i < this->numDigit * 2; i++)
	{
		std::string tempNum;
		std::strstream ss;
		ss << numDigit*2 - i - 1;
		ss >> tempNum;
		cv::imwrite(this->m_filePath + this->m_matName + tempNum + this->m_matEnd, this->grayCodeMat[i]);
	}

	return true;
}

// 开始构建GrayCode。需要传入具体的位数
bool CEncoder_Gray::Encode(int numDigit, bool lineBased)
{
	// 确保参数合法
	if ((numDigit <= 0) || (numDigit > 16))
		return false;
	this->numDigit = numDigit;
	this->grayCodeSize = (int)1 << (this->numDigit); // 1 << 6 = 64  1 << 5 = 32
	this->lineBased = lineBased; // true: 列条纹 false：行条纹

	// 开始运算
	if (!this->EncodeGray())
		return false;
	if (!this->DrawMat())
		return false;

	// 输出到文件
	if (!this->WriteData())
		return false;

	return true;
}

// 设定存储格雷码数据的文件名
bool CEncoder_Gray::SetCodeFileName(string codeName, string codeEnd)
{
	this->m_codeName = codeName;
	this->m_codeEnd = codeEnd;
	return true;
}

// 设定存储格雷码图案的文件名
bool CEncoder_Gray::SetMatFileName(string filePath, string matName, string matEnd)
{
	this->m_filePath = filePath;
	this->m_matName = matName;
	this->m_matEnd = matEnd;
	return true;
}

// 可视化
void CEncoder_Gray::Visualization()
{
	for (int i = 0; i < this->grayCodeSize; i++)
	{
		for (int d = this->numDigit; d > 0; d--)
		{
			std::cout << this->GetBit(this->grayCode[i], d);
		}
		std::cout << std::endl;
	}

#ifdef VISUAL
	cv::namedWindow(this->m_matName);
	for (int i = 0; i < this->numDigit * 2; i++)
	{
		std::cout << "Now present: " << i << std::endl;
		imshow(this->m_matName, this->grayCodeMat[i]);
		cv::waitKey(300);
	}
	cv::destroyWindow(this->m_matName);
#endif
}


