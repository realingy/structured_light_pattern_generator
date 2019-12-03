#include "head.h"

using namespace cv;

int main()
{
	// 格雷码进行编码
	CEncoder_Gray EGray_v, EGray_h;
	EGray_v.SetCodeFileName("vGrayCode", ".txt");
	EGray_v.SetMatFileName("Projector/", "vGray", ".bmp");
	if (!EGray_v.Encode(6, true))
		printf("vGray Encode failed.\n");
	EGray_v.Visualization();

	EGray_h.SetCodeFileName("hGrayCode", ".txt");
	EGray_h.SetMatFileName("Projector/", "hGray", ".bmp");
	if (!EGray_h.Encode(5, false))
		printf("hGray Encode failed.\n");
	EGray_h.Visualization();
	
	// PhaseShifting进行编码
	CEncoder_Phase EPS_v, EPS_h;
	EPS_v.SetMatFileName("Projector/", "vPhase", ".bmp");
	if (!EPS_v.Encode(PROJECTOR_RESLINE / 32, true)) // 32个竖条纹
		printf("vPhase Encode failed.\n");
	EPS_v.Visualization();

	EPS_h.SetMatFileName("Projector/", "hPhase", ".bmp");
	if (!EPS_h.Encode(PROJECTOR_RESROW / 16, false)) // 16个行条纹
		printf("hPhase Encode failed.\n");
	EPS_h.Visualization();

	system("PAUSE");
	return 0;
}