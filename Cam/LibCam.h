

#ifndef _LIB_CAM
#define _LIB_CAM


#include <iostream>
#include "GxIAPI.h"

using namespace std;



//打开相机
int Cam_Open();
//相机初始化
int Cam_Inti();
//关闭相机
int Cam_Close();
//回调函数注册
int Cam_RegisterCapture(GXCaptureCallBack callBackFun);
//设置采集状态
//true 开启
//false 关闭
int Cam_SetAcquisition(bool isAcquisition);


//拍摄
int Cam_Snap();


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

#endif








