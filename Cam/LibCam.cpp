
#include"LibCam.h"
#include "../Base/Bash64.h"



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
	if (status != GX_STATUS_SUCCESS)
	{
		return 0;
	}

	return 1;
}

//相机初始化
/*
int Cam_Inti()
{
}
*/
char* ImgBuild(PGX_FRAME_BUFFER pFrame )
{
	char* m_rgb_image=NULL;
	int x =0;
	string imgBase64 = NULL;
	if (pFrame->nStatus == GX_FRAME_STATUS_SUCCESS)
	{
		int height =pFrame->nHeight;
		int width =pFrame->nWidth;

		Mat src;
		m_rgb_image=new char[width*height*3];

		//创建MAT
		src.create(height,width,CV_8UC3);
		//去除const`
		void* imgBuf = const_cast<void*>(pFrame->pImgBuf);

		//图像raw8转RGB格式
		DxRaw8toRGB24(imgBuf,m_rgb_image,width, height,RAW2RGB_NEIGHBOUR,DX_PIXEL_COLOR_FILTER(BAYERBG),false);
		//copy 图像内存
		memcpy(src.data,m_rgb_image,width*height*3);

		vector<uchar> imgdata;
		imencode(".jpg",src,imgdata);
		char charray[imgdata.size()];
		//vector全部转到数组
		memcpy(charray, &imgdata[0], imgdata.size() * sizeof(imgdata[0]));
		imgBase64 = base64_encode(charray,sizeof(charray));
		delete[] m_rgb_image;
	}
	return &imgBase64[0];
}

//拍摄
int Cam_Snap(char* img)
{
	PGX_FRAME_BUFFER pFrameBuffer;
	status = GXStreamOn(hDevice);
	if (status == GX_STATUS_SUCCESS) {
		img = ImgBuild(pFrameBuffer);
		GXDQBuf(hDevice, &pFrameBuffer, 1000);
		GXStreamOff(hDevice);
	}
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

int Cam_SetAutoWhite()
{
	//status = GXSetEnum(hDevice,GX_ENUM_AWB_LAMP_HOUSE, GX_AWB_LAMP_HOUSE_FLUORESCENCE);
	//设置连续自动白平衡
	status = GXSetEnum(hDevice,GX_ENUM_BALANCE_WHITE_AUTO, GX_BALANCE_WHITE_AUTO_CONTINUOUS);
	if (status != GX_STATUS_SUCCESS)
	{
		return 0;

	}
	return 1;
}



int Cam_GetBalanceRatio_R(double *dGain )
{
	status = GXSetEnum(hDevice,GX_ENUM_BALANCE_RATIO_SELECTOR, GX_BALANCE_RATIO_SELECTOR_RED);
	if (status != GX_STATUS_SUCCESS)
	{
		return 0;
	}
	status = GXGetFloat(hDevice, GX_FLOAT_GAIN, dGain);
	if (status != GX_STATUS_SUCCESS)
	{
		return 0;
	}

	return  1;
}


int Cam_SetBalanceRatio_R(double *dGain )
{
	status = GXSetEnum(hDevice,GX_ENUM_BALANCE_RATIO_SELECTOR, GX_BALANCE_RATIO_SELECTOR_RED);
	if (status != GX_STATUS_SUCCESS)
	{
		return 0;
	}

	status = GXSetFloat(hDevice, GX_FLOAT_GAIN, *dGain);
	if (status != GX_STATUS_SUCCESS)
	{
		return 0;
	}

	return  1;
}
int Cam_GetBalanceRatio_G(double *dGain )
{
	status = GXSetEnum(hDevice,GX_ENUM_BALANCE_RATIO_SELECTOR, GX_BALANCE_RATIO_SELECTOR_GREEN);
	if (status != GX_STATUS_SUCCESS)
	{
		return 0;
	}
	status = GXGetFloat(hDevice, GX_FLOAT_GAIN, dGain);
	if (status != GX_STATUS_SUCCESS)
	{
		return 0;
	}

	return  1;
}


int Cam_SetBalanceRatio_G(double *dGain )
{
	status = GXSetEnum(hDevice,GX_ENUM_BALANCE_RATIO_SELECTOR, GX_BALANCE_RATIO_SELECTOR_GREEN);
	if (status != GX_STATUS_SUCCESS)
	{
		return 0;
	}

	status = GXSetFloat(hDevice, GX_FLOAT_GAIN, *dGain);
	if (status != GX_STATUS_SUCCESS)
	{
		return 0;
	}

	return  1;
}
int Cam_GetBalanceRatio_B(double *dGain )
{
	status = GXSetEnum(hDevice,GX_ENUM_BALANCE_RATIO_SELECTOR, GX_BALANCE_RATIO_SELECTOR_BLUE);
	if (status != GX_STATUS_SUCCESS)
	{
		return 0;
	}
	status = GXGetFloat(hDevice, GX_FLOAT_GAIN, dGain);
	if (status != GX_STATUS_SUCCESS)
	{
		return 0;
	}

	return  1;
}


int Cam_SetBalanceRatio_B(double *dGain )
{
	status = GXSetEnum(hDevice,GX_ENUM_BALANCE_RATIO_SELECTOR, GX_BALANCE_RATIO_SELECTOR_BLUE);
	if (status != GX_STATUS_SUCCESS)
	{
		return 0;
	}

	status = GXSetFloat(hDevice, GX_FLOAT_GAIN, *dGain);
	if (status != GX_STATUS_SUCCESS)
	{
		return 0;
	}

	return  1;
}

int Cam_GetAll(double *dGain[5])
{
//Cam_GetExposure
	int ret= Cam_GetExposure(*dGain);
	if(ret == 0 )
	{
		return 0;
	}
	ret = Cam_GetGain(*dGain+1);
	if(ret == 0 )
	{
		return 0;
	}
	ret = Cam_GetBalanceRatio_R(*dGain+2);
	if(ret == 0 )
	{
		return 0;
	}
	ret = Cam_GetBalanceRatio_G(*dGain+3);
	if(ret == 0 )
	{
		return 0;
	}
	ret = Cam_GetBalanceRatio_B(*dGain+4);
	if(ret == 0 )
	{
		return 0;
	}
	return 1;
}

int Cam_SetAll(double *dGain[5])
{
	int ret = Cam_SetExposure(*dGain[0]);
	if(ret == 0 )
	{
		return 0;
	}
	ret=Cam_SetGain(*dGain[1]);
	if(ret == 0 )
	{
		return 0;
	}
	ret=Cam_SetBalanceRatio_R(*dGain+2);
	if(ret == 0 )
	{
		return 0;
	}
	ret=Cam_SetBalanceRatio_G(*dGain+3);
	if(ret == 0 )
	{
		return 0;
	}
	ret=Cam_SetBalanceRatio_B(*dGain+4);
	if(ret == 0 )
	{
		return 0;
	}
	return 1;
}
int Cam_Reset()
{
	status = GXSendCommand(hDevice, GX_COMMAND_DEVICE_RESET);
	if (status != GX_STATUS_SUCCESS)
	{
		return 0;
	}
	return 1;
}
struct Cam_Method arr[] = {{"Cam_GetExposure",NULL,Cam_GetExposure}
			   ,{"Cam_SetExposure",Cam_SetExposure}
			   ,{"Cam_Trigger",NULL,NULL,Cam_Trigger}
			   ,{"Cam_Snap",NULL,NULL,NULL,Cam_Snap}
			   ,{"Cam_GetGain",NULL,Cam_GetGain}
			   ,{"Cam_SetGain",Cam_SetGain}
			   ,{"Cam_SetAutoWhite",NULL,NULL,NULL,NULL,Cam_SetAutoWhite}
			   ,{"Cam_GetBalanceRatio_R",NULL,Cam_GetBalanceRatio_R}
			   ,{"Cam_GetBalanceRatio_G",NULL,Cam_GetBalanceRatio_G}
			   ,{"Cam_SetBalanceRatio_B",NULL,Cam_GetBalanceRatio_B}
			   ,{"Cam_SetBalanceRatio_R",NULL,Cam_SetBalanceRatio_R}
			   ,{"Cam_SetBalanceRatio_R",NULL,Cam_SetBalanceRatio_G}
			   ,{"Cam_SetBalanceRatio_R",NULL,Cam_SetBalanceRatio_B}
			   ,{"Cam_GetAll",NULL,NULL,NULL,NULL,NULL,Cam_GetAll}
			   ,{"Cam_SetAll",NULL,NULL,NULL,NULL,NULL,Cam_SetAll}
			   ,{"Cam_Reset",NULL,NULL,NULL,NULL,Cam_Reset}
			  };


Cam_Method* getMethod(char* method_name)
{
	for(int i=0;i<sizeof(arr)/sizeof(Cam_Method);i++)
	{
		char* tmp = arr[i].method_name;
		if(strcmp(method_name,tmp)==0)
		{
			return &arr[i];
		}
	}
	return NULL;
}

