// C++���
#include <iostream>
#include <fstream>
#include <strstream>
#include <cmath>

// OpenCV
#include <opencv2/highgui.hpp>

using namespace std;
using namespace cv;

//#define VISUAL

//ͶӰ�Ƿֱ���
static int PROJECTOR_RESLINE = 1280;
static int PROJECTOR_RESROW = 720;

// ���������ɡ�
// ����λ�����Ϊ16λ����ʹ��short���ʹ洢������
class CEncoder_Gray
{
private:
	int numDigit;			// λ��
	int grayCodeSize;		// �ܹ��ĸ�������Ŀ
	short *grayCode;		// ������
	Mat *grayCodeMat;		// �������Ӧ��ͼ��
	int resRow;				// ͼ����зֱ���
	int resLine;			// ͼ����зֱ���
	bool lineBased;			// �Ƿ�����������

	string m_filePath;		// �洢·����
	string m_codeName;		// �������ļ���
	string m_codeEnd;		// �������׺��
	string m_matName;		// ͼ����
	string m_matEnd;		// ͼ���׺��

	bool GetBit(short Code, int n);		// ��ȡCode�е������nλ��n��1��ʼ��
	bool EncodeGray();					// ����numDigitλ��GrayCode���洢��
	bool DrawMat();						// ���ݸ��������ݣ����Ƹ�����ͼ��
	bool WriteData();					// ������ļ�

public:
	CEncoder_Gray();										// ���캯����
	~CEncoder_Gray();										// ����������ɾ���������ؿռ䡣
	bool Encode(int numDigit, bool lineBased);				// ��ʼ����GrayCode����Ҫ��������λ����
	bool SetCodeFileName(string codeName, string codeEnd);	// �趨�洢�ļ���
	bool SetMatFileName(string filePath, string matName, string matEnd);
	void Visualization();
};

// PhaseShifting����
class CEncoder_Phase
{
private:
	int m_numMat;			// Mat��Ŀ
	int m_pixPeriod;		// ÿ���ڵ�pix��Ŀ
	cv::Mat * m_PSMat;		// phaseshifting��Ӧ��ͼ��

	int m_resRow;			// ͼ����зֱ���
	int m_resLine;			// ͼ����зֱ���
	bool m_lineBased;		// �Ƿ�����������

	string m_filePath;		// �洢·����
	string m_matName;		// ͼ����
	string m_matEnd;		// ͼ���׺��

	bool DrawMat();			// ����PS���ݣ�����ͼ��
	bool WriteData();		// ������ļ�

public:
	CEncoder_Phase();
	~CEncoder_Phase();
	bool Encode(int pixPeriod, bool lineBased);
	bool SetMatFileName(string filePath, string matName, string matEnd);
	void Visualization();
};
