#include "Slave/LibSlave.h"
#include <iostream>
#include <opencv2/highgui.hpp>
#include <opencv2/opencv_modules.hpp>
#include "Cam/LibCam.h"
#include "Cam/DxImageProc.h"
#include "Socket/Socket.h"
#include "Base/Bash64.h"
#include <string.h>
#include <cjson/cJSON.h>


#include<opencv2/imgcodecs.hpp>
//#include<opencv2/imgcodecs/imgcodecs_c.h>


enum CMDS { IMG=1000,WIFI };
//char* response(int code,char* data);
char* response(int code,const char* msg,cJSON* data);


using namespace std;
using namespace cv;

char* m_rgb_image=NULL;

int x =0;
//图像回调处理函数
static void GX_STDC OnFrameCallbackFun(GX_FRAME_CALLBACK_PARAM* pFrame)
{

//	cout <<"回调函数触发"<<endl;
	if (pFrame->status == GX_FRAME_STATUS_SUCCESS)
	{
	//对图像进行某些操作
//	cout << "收到一帧图像!" << endl;
	int height =pFrame->nHeight;
	int width =pFrame->nWidth;

	Mat src;
	//申请rgb空间
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

	printf("测试点2\n");

//	namedWindow("src",0);
//	imshow("src",src);

	//waitKey(100);
	
	
  char charray[imgdata.size()];
 
    //vector全部转到数组
  memcpy(charray, &imgdata[0], imgdata.size() * sizeof(imgdata[0]));

	//imgdata.size();
	if(getconnum()>=1 ){
   		
	string imgBase64 = base64_encode(charray,sizeof(charray));
	

		cJSON* cjson_test = NULL;
		/* 创建一个JSON数据对象(链表头结点) */
   		cjson_test = cJSON_CreateObject();
//
    		/* 添加一条字符串类型的JSON数据(添加一个链表节点) */
    		cJSON_AddStringToObject(cjson_test, "name", &imgBase64[0]);
		char* str = NULL;
   		str = cJSON_Print(cjson_test);

//		printf("%s\n", str);
   		printf("测试点1\n");
		printf("图像size：%lu imencode size: %lu \n",strlen(str),imgdata.size());

		char* msg = "server send test : data 222222222";
		socketSend(msg , strlen(msg));
		if(x ==0){

			socketSend(str,strlen(str));
			x++;
		}
		delete[] str;
		cJSON_Delete(cjson_test);

	}
	//namedWindow("t    str = cJSON_Print(cjson_test);

	//imshow("test",src);
	//释放rgb空间
	delete[] m_rgb_image;
	}
	return;
}




int main()
{
//char* x = "{\"a\":\"i\"}";
//recvStrData(x);
//cJSON *obj = cJSON_CreateObject();
//cJSON_AddStringToObject(obj,"day","1");
//char* tmp = response(-1,"asfasdf",obj);
//printf("%s\n",tmp);
//return 0;
	//链接下位机
	dev_conn("/dev/ttyS0", 115200, 'N', 8, 1);
	dev_LED(1);
	dev_close();

	printf("open Caming...\n");
	Cam_Open();
	printf("registerCapture...");
	Cam_RegisterCapture(OnFrameCallbackFun);
	printf("SetAcquisition...\n");
	Cam_SetAcquisition(true);
	printf("socket init...\n");

	socket_Init();
	printf("Socket Start...\n");
	Socket_Start();
//
//
//	Cam_SetExposure(2000000000000.0);
	
//	double mExposureTime =0;
//	Cam_GetExposure(&mExposureTime);
//	cout<<"曝光:"<<mExposureTime<<endl;




	char inCh;
	for(;;)
	{
		inCh=getchar();
		if(inCh=='q'){
				
			Cam_SetAcquisition(false);
			Cam_Close();
			break;

		}
	}

	return 0;

}
char* recvStrData(char* jsonstr)
{
	cJSON* jsonObj = cJSON_Parse(jsonstr);
	if(jsonObj==NULL)
	{
		printf("Error before: [%s]\n", cJSON_GetErrorPtr());
		return response(-1,cJSON_GetErrorPtr(),NULL);
	}
	cJSON* cmd = cJSON_GetObjectItem(jsonObj, "cmd");
	if(cmd == NULL)
	{
		printf("Error before: [%s]\n", cJSON_GetErrorPtr());
		return response(-1,cJSON_GetErrorPtr(),NULL);
	}
	int command = cmd->valueint;
//	cJSON_Print(cmd);
	int code =0;//0 成功 
	char *msg =NULL;
	cJSON* data = NULL;
	switch(command)
	{
		case WIFI:
			printf("cmd is wifi: %d\n",WIFI);
			msg = "set wifi ok";
			break;
		case IMG:
			printf("cmd is IMG: %d\n",IMG);
			msg = "set img ok";
			break;
		default :
			printf("cmd is default: %d\n",IMG);
			msg = "set defaut ok";
			
	}
	cJSON_Delete(jsonObj);
	return response(code,msg,data);
} 

char* response(int code,const char* msg,cJSON* data)
{
	cJSON *ret = cJSON_CreateObject();
	cJSON_AddNumberToObject(ret,"code",code);
	cJSON_AddStringToObject(ret,"msg",msg);
	cJSON_AddItemToObject(ret,"data",data);

	char *json_data = cJSON_Print(ret);
	cJSON_Delete(ret);
	cJSON_Delete(data);
	return json_data;
}
