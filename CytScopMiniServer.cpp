#include "Slave/LibSlave.h"
#include <iostream>
#include <opencv2/highgui.hpp>
#include <opencv2/opencv_modules.hpp>
#include "Cam/LibCam.h"
#include "Cam/DxImageProc.h"
#include "Socket/Socket.h"
#include "Base/ExecuteCommand.h"
#include "Base/Bash64.h"
#include <string.h>
#include <cjson/cJSON.h>


#include<opencv2/imgcodecs.hpp>
//#include<opencv2/imgcodecs/imgcodecs_c.h>


enum DEVICE { CAM=1,SYS,STM };
char* response(int code,const char* msg,cJSON* data);


using namespace std;
using namespace cv;


//图像回调处理函数
static void GX_STDC OnFrameCallbackFun(GX_FRAME_CALLBACK_PARAM* pFrame)
{
	char* m_rgb_image=NULL;
	int x =0;
	if (pFrame->status == GX_FRAME_STATUS_SUCCESS)
	{
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

		//printf("测试点2\n");

		//	namedWindow("src",0);
		//	imshow("src",src);

		//waitKey(100);


		char charray[imgdata.size()];

		//vector全部转到数组
		memcpy(charray, &imgdata[0], imgdata.size() * sizeof(imgdata[0]));

		//imgdata.size();
		if(getconnum()>=1 ){

			string imgBase64 = base64_encode(charray,sizeof(charray));


			cJSON* cjson_test = cJSON_CreateObject();
			/* 创建一个JSON数据对象(链表头结点) */
			//   		cjson_test = cJSON_CreateObject();
			//
			/* 添加一条字符串类型的JSON数据(添加一个链表节点) */

			cJSON_AddStringToObject(cjson_test, "name", &imgBase64[0]);
			char* str = NULL;
			str = cJSON_Print(cjson_test);

			//		printf("%s\n", str);
			//printf("测试点1\n");
			//printf("图像size：%lu imencode size: %lu \n",strlen(str),imgdata.size());

			//char* msg = "server send test : data 222222222";
			//socketSend(msg , strlen(msg));
			if(x ==0){

				socketSend(str,strlen(str));
				x++;
			}
			delete[] str;
			cJSON_Delete(cjson_test);

		}
		delete[] m_rgb_image;
	}
	return;
}


int system_option(const cJSON* json,char *result)
{

	char* cmd = cJSON_GetObjectItem(json, "cmd")->valuestring;
	char* execute_cmd = cmd;
	return execute_command(execute_cmd,result);
}

int modbus_option(const cJSON* json,uint16_t* val)
{
	char* cmd = cJSON_GetObjectItem(json, "cmd")->valuestring;
	const cJSON* data = cJSON_GetObjectItem(json, "data");
	int register_offset = cJSON_GetObjectItem(data,"register_offset")->valueint;
	int ret_val=0;
	//uint16_t* val ;
	if(strcmp(cmd,"Set")==0)
	{	
		uint16_t tmp;
		char* p  = cJSON_GetObjectItem(cJSON_GetObjectItem(json, "data"),"val")->valuestring;
		sscanf(p,"%X",tmp);
		val[0] =tmp;
		printf("val %X\n",val);
		ret_val = set(register_offset,val);
		return ret_val > -1?1:-1;
	}
	ret_val= get(register_offset,val);
	return ret_val > -1?1:-1;
}

int cam_option(const cJSON* json,cJSON* outdata,char* method) 
{

	Cam_Method* sMethod = getMethod(method);
	if(sMethod == NULL)
	{
		printf("sMethod is null");
		return -1;
	}
	if(sMethod->nmethod != NULL)
	{
		return (sMethod->nmethod)();
	}
	if(sMethod->dmethod != NULL)
	{
		cJSON* data = cJSON_GetObjectItem(json, "data");
		double val  = cJSON_GetObjectItem(data, "val")->valuedouble;
		return (sMethod->dmethod)(val);
	}

	if(sMethod->dpmethod != NULL)
	{
		double val ; 
		int x = (sMethod->dpmethod)(&val);
		if(x != 0)
		{
			cJSON_AddNumberToObject(outdata,"val",val);
		}
printf("err : %s\n",get_Err());
		return  x;
	}
	if(sMethod->bmethod != NULL)
	{
		cJSON* data = cJSON_GetObjectItem(json, "data");
		bool val  = cJSON_GetObjectItem(data, "val")->valueint;
		return (sMethod->bmethod)(val);
	}
	if(sMethod->cmethod != NULL)
	{
		char*  val[2048] ; 
		int x = (sMethod->cmethod)(*val);
		if(x != 0)
		{
			cJSON_AddStringToObject(outdata,"val",*val);
		}
		return  x;
	}
	return -1;
}


int main()
{
	cJSON* jsonObj = cJSON_CreateObject();
	cJSON* data= cJSON_CreateObject();
	int ret =cam_option(jsonObj,data,"Cam_SetAutoWhite");
printf("err : %s\n",get_Err());
printf("ret is %d\n",ret);
	return 0;
/*
	cJSON* jsonObj = cJSON_CreateObject();
	cJSON_AddStringToObject(jsonObj,"cmd","ls");
	char ret[1024*4];
	int code =system_option(jsonObj,ret);
	printf("out is %s code is %d\n",ret,code);
	return 0;
*/
	//链接下位机
	dev_conn("/dev/ttyS0", 115200, 'N', 8, 1);

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
	dev_close();

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
	cJSON* deviceId = cJSON_GetObjectItem(jsonObj, "id");
	if(deviceId  == NULL)
	{
		printf("Error before: [%s]\n", cJSON_GetErrorPtr());
		return response(-1,cJSON_GetErrorPtr(),NULL);
	}
	int id = deviceId->valueint;
	int code =0;//0 成功 
	char *msg =NULL;
	cJSON* data = cJSON_CreateObject();
	switch(id)
	{
		case SYS:
			{
				char ret[1024*4];
				code= system_option(jsonObj,ret);
				msg=ret;
			}
			break;
		case STM:{
				 uint16_t* val = NULL;	
				 code = modbus_option(jsonObj,val);
				 const char* p= modbus_strerror(errno);
				 if(sizeof(&p)>0)
				 {
					 code = -1;
					 msg = (char*)malloc(sizeof(char)*sizeof(&p));
					 strcpy(msg,modbus_strerror(errno));
				 }
				 p = NULL;
				 char tmp[2];
				 sprintf(tmp,"%x",val);
				 cJSON_AddStringToObject(data,"val",tmp);
			 }
			 break;
		case CAM:
			 {
				 char* tmp = cJSON_GetObjectItem(jsonObj, "cmd")->valuestring;
				 code = cam_option(jsonObj,data,tmp);
				 msg = "ok";
			 }
			 break;
		default :
			 code = -1;
			 msg = "deviceId is err\n";
	}
	cJSON_Delete(jsonObj);
	char* ret = response(code,msg,data);
	//free(msg);
	return ret;
} 

char* response(int code,const char* msg,cJSON* data)
{
	cJSON *ret = cJSON_CreateObject();
	cJSON_AddNumberToObject(ret,"code",code);
	if(msg==NULL){
		cJSON_AddStringToObject(ret,"msg","sucess");
	}else{
		cJSON_AddStringToObject(ret,"msg",msg);
	}
	cJSON_AddItemToObject(ret,"data",data);

	char *json_data = cJSON_Print(ret);
	cJSON_Delete(ret);
	//cJSON_Delete(data);
	return json_data;
}


