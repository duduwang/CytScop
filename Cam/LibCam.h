

#ifndef _LIB_CAM
#define _LIB_CAM


#include <iostream>
#include "GxIAPI.h"
#include <opencv2/highgui.hpp>
#include <opencv2/opencv_modules.hpp>
#include <opencv2/imgcodecs.hpp>

#include "DxImageProc.h"

using namespace std;
using namespace cv;



//打开相机
int Cam_Open();
//相机初始化
//int Cam_Inti();
//关闭相机
int Cam_Close();
//回调函数注册
int Cam_RegisterCapture(GXCaptureCallBack callBackFun);
//设置采集状态
//true 开启
//false 关闭
int Cam_SetAcquisition(bool isAcquisition);


//拍摄
int Cam_Snap(char* img);


//触发模式开关
//TRUE 打开触发模式
//FALSE 关闭触发模式
int Cam_Trigger(bool isTrigger);

//获取曝光时间
int Cam_GetExposure(double *dExposureTime );

//设置曝光时间
int Cam_SetExposure(double dExposureTime);

//获取增益
int Cam_GetGain(double *dGain);

//设置增益
int Cam_SetGain(double dGain);


int Cam_SetAutoWhite();

int Cam_GetBalanceRatio_R(double *dGain );
int Cam_SetBalanceRatio_R(double dGain );

int Cam_GetBalanceRatio_G(double *dGain );
int Cam_SetBalanceRatio_G(double dGain );

int Cam_GetBalanceRatio_B(double *dGain );
int Cam_SetBalanceRatio_B(double dGain );

int Cam_GetAll(double* dGain[]);
int Cam_SetAll(double dGain[]);
int Cam_Reset();

struct Cam_Method
{
	char const * method_name;
	int(*dmethod)(double);
	int(*dpmethod)(double*);
	int(*bmethod)(bool);
	int(*cmethod)(char*);
	int(*nmethod)();
	int(*damethod)(double[]);
	int(*dapmethod)(double*[]);
};


Cam_Method* getMethod(char* method_name);

char*  get_Err();
#endif






