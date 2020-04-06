#include "head.h"

using namespace std;

// ���캯��
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

// ����������ɾ���������ؿռ�
CEncoder_Gray::~CEncoder_Gray()
{
	// ɾ��GrayCode
	if (this->grayCode != NULL)
	{
		delete[](this->grayCode);
		this->grayCode = NULL;
	}
	
	// ɾ��GrayCodeMat
	if (this->grayCodeMat != NULL)
	{
		delete[](this->grayCodeMat);
		this->grayCodeMat = NULL;
	}
}

// ��ȡCode�е������nλ��n��1��ʼ
bool CEncoder_Gray::GetBit(short Code, int n)
{
	bool res = (Code >> (n - 1)) & 0x0001;
	return res;
}

// ����numDigitλ��GrayCode���洢
bool CEncoder_Gray::EncodeGray()
{
	// ����������ռ䲢��ʼ��
	this->grayCode = new short[this->grayCodeSize]; //64

	// ��ʼѭ�����
	this->grayCode[0] = 0x0000;		// �Ե�һ�����ȫ0
	this->grayCode[1] = 0x0001;		// �ı����Ҷ˵�ֵ
	for (int i = 2; i < this->grayCodeSize; i += 2)
	{
		// ��GrayCode[i]: ���ȣ��ҵ��ұߵ�һ��Ϊ1����
		int k = 1;
		while (true)
		{
			if (this->GetBit(this->grayCode[i - 1], k))
				break;
			k++;
		}
		if (k >= 16)	return false;
		// ��GrayCode[i]: Ȼ�󣬸ı���������һλ��ֵ
		short Cover = 0x0001 << k;
		grayCode[i] = grayCode[i - 1] ^ Cover;

		// ��GrayCode[i+1]���ı����Ҷ˵�ֵ
		grayCode[i + 1] = grayCode[i] ^ 0x0001;
	}

	// �����ʾ������
	/*
	for (int i = 0; i < this->grayCodeSize; i++)
	{
		std::cout << "==> " << i << " :: " << grayCode[i] << std::endl;
	}
	*/

	return true;
}

// ���ݸ��������ݣ����Ƹ�����ͼ��
bool CEncoder_Gray::DrawMat()
{
	// ����numDigit*2��ͼ��
	this->grayCodeMat = new cv::Mat[this->numDigit*2];

	// ����ÿһ��ͼ��picIdx������Ƶ��Ǹ�����ĵ�picIdx / 2λ��Ҳ����ڼ�������
	for (int picIdx = 0; picIdx < this->numDigit * 2; picIdx += 2)
	{
		// ����ͼ��
		this->grayCodeMat[picIdx].create(this->resRow, this->resLine, CV_8UC1);
		this->grayCodeMat[picIdx + 1].create(this->resRow, this->resLine, CV_8UC1);

		// ������
		if (lineBased)	// ����ǻ��ư��е�ͼ��
		{
			int space = this->resLine / grayCodeSize;
			for (int i = 0; i < grayCodeSize; i++)
			{
				// �жϻ�����ɫ
				char color = 0;
				if (this->GetBit(grayCode[i], picIdx / 2 + 1))
					color = 0xFF;
				else
					color = 0;

				// ����
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
				// �жϻ�����ɫ
				char color = 0;
				if (this->GetBit(grayCode[i], picIdx / 2 + 1))
					color = 0xFF;
				else
					color = 0;

				// ����
				for (int r = 0; r < space; r++)
				{
					for (int l = 0; l < this->resLine; l++)
					{
						this->grayCodeMat[picIdx].at<char>(r + i*space, l) = color;
					}
				}
			}
		}

		// ������ɫͼ��
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

// ������ļ�
bool CEncoder_Gray::WriteData()
{
	// ����·��
	string tempPath = this->m_filePath;
	for (int i = 0; i < tempPath.length(); i++)
	{
		if (tempPath[i] == '/')
			tempPath[i] = '\\';
	}
	system((string("mkdir ") + tempPath).c_str());

	// ������ɵ�GrayCode��txt
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

	// ������ɵ�GrayCodeMat��bmp
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

// ��ʼ����GrayCode����Ҫ��������λ��
bool CEncoder_Gray::Encode(int numDigit, bool lineBased)
{
	// ȷ�������Ϸ�
	if ((numDigit <= 0) || (numDigit > 16))
		return false;
	this->numDigit = numDigit;
	this->grayCodeSize = (int)1 << (this->numDigit); // 1 << 6 = 64  1 << 5 = 32
	this->lineBased = lineBased; // true: ������ false��������

	// ��ʼ����
	if (!this->EncodeGray())
		return false;
	if (!this->DrawMat())
		return false;

	// ������ļ�
	if (!this->WriteData())
		return false;

	return true;
}

// �趨�洢���������ݵ��ļ���
bool CEncoder_Gray::SetCodeFileName(string codeName, string codeEnd)
{
	this->m_codeName = codeName;
	this->m_codeEnd = codeEnd;
	return true;
}

// �趨�洢������ͼ�����ļ���
bool CEncoder_Gray::SetMatFileName(string filePath, string matName, string matEnd)
{
	this->m_filePath = filePath;
	this->m_matName = matName;
	this->m_matEnd = matEnd;
	return true;
}

// ���ӻ�
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


