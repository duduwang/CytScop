
#include"LibCam.h"




GX_STATUS status = GX_STATUS_SUCCESS;
GX_DEV_HANDLE hDevice = NULL;

//打开相机
int Cam_Open()
{

    uint32_t nDeviceNum = 0;
	GX_OPEN_PARAM stOpenParam;
    //初始化库
	status = GXInitLib();
	if (status!= GX_STATUS_SUCCESS)
	{
		cout << "初始化相机失败" << endl;
		return 0;
	}

	//枚举设备列表
	status = GXUpdateDeviceList(&nDeviceNum, 1000);
	if ((status!= GX_STATUS_SUCCESS)||(nDeviceNum<= 0))
	{
		cout << "获取相机失败" << endl;

		return 0;
	}


	cout << "相机初始话完成" << endl;


	//打开设备
	stOpenParam.accessMode = GX_ACCESS_EXCLUSIVE;
	stOpenParam.openMode = GX_OPEN_INDEX;
	stOpenParam.pszContent = "1";
	status = GXOpenDevice(&stOpenParam, &hDevice);
	if (status != GX_STATUS_SUCCESS)
	{
		return 0;

	}

	return 1;

}
//回调函数注册
int Cam_RegisterCapture(GXCaptureCallBack callBackFun)
{

	//注册图像处理回调函数
	status = GXRegisterCaptureCallback(hDevice, NULL,callBackFun);
	if (status != GX_STATUS_SUCCESS)
	{
		return 0;

	}

	return 1;

}
//设置采集状态
//true 开启
//false 关闭
int Cam_SetAcquisition(bool isAcquisition)
{


	//发送开采命令
	status = GXSendCommand(hDevice, isAcquisition? GX_COMMAND_ACQUISITION_START :GX_COMMAND_ACQUISITION_STOP);
		//发送停采命令
	//status = GXSendCommand(hDevice, GX_COMMAND_ACQUISITION_STOP);

	if (status != GX_STATUS_SUCCESS)
	{
		return 0;

	}

	return 1;


}

//相机初始化
int Cam_Inti()
{
	//TODO
}


//拍摄
int Cam_Snap()
{
	//TODO	
}



//触发模式开关
//TRUE 打开触发模式
//FALSE 关闭触发模式
int Cam_Trigger(bool isTrigger)
{
	status = GXSetEnum(hDevice,GX_ENUM_TRIGGER_MODE,isTrigger?GX_TRIGGER_MODE_ON:GX_TRIGGER_MODE_OFF);
	if (status != GX_STATUS_SUCCESS)
	{
		return 0;

	}

	return 1;


}


//关闭相机
int Cam_Close()
{
	//注销采集回调
	status = GXUnregisterCaptureCallback(hDevice);
	//关闭相机
	status = GXCloseDevice(hDevice);
	status = GXCloseLib();


	if (status != GX_STATUS_SUCCESS)
	{
		return 0;

	}

	return 1;


}

//获取曝光时间
int Cam_GetExposure(double *dExposureTime )
{


	
	status = GXGetFloat(hDevice, GX_FLOAT_EXPOSURE_TIME, dExposureTime);
	if (status != GX_STATUS_SUCCESS)
	{
		return 0;

	}

	return  1;


}


//设置曝光时间
int Cam_SetExposure(double dExposureTime)
{

	GX_FLOAT_RANGE stFloatRange;

    	// Get the range 
    	status = GXGetFloatRange(hDevice, GX_FLOAT_EXPOSURE_TIME, &stFloatRange);
	if(dExposureTime > stFloatRange.dMax)
	{
		dExposureTime = stFloatRange.dMax;
	}
	if(dExposureTime < stFloatRange.dMin)
	{
		dExposureTime = stFloatRange.dMin;
	}

	status = GXSetFloat(hDevice, GX_FLOAT_EXPOSURE_TIME, dExposureTime);
	if (status != GX_STATUS_SUCCESS)
	{
		return 0;

	}

	return 1;


}





//获取增益
int Cam_GetGain(double *dGain )
{


	
	status = GXGetFloat(hDevice, GX_FLOAT_GAIN, dGain);
	if (status != GX_STATUS_SUCCESS)
	{
		return 0;

	}

	return  1;


}


//设置增益
int Cam_SetGain(double dGain)
{

	GX_FLOAT_RANGE stFloatRange;

    	// Get the range 
    	status = GXGetFloatRange(hDevice, GX_FLOAT_GAIN, &stFloatRange);
	if(dGain > stFloatRange.dMax)
	{
		dGain = stFloatRange.dMax;
	}
	if(dGain < stFloatRange.dMin)
	{
		dGain = stFloatRange.dMin;
	}

	status = GXSetFloat(hDevice, GX_FLOAT_GAIN, dGain);
	if (status != GX_STATUS_SUCCESS)
	{
		return 0;

	}

	return 1;
}







