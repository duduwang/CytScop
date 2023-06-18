#include <iostream>
#include "../Cam/LibCam.h"
#include "../Cam/DxImageProc.h"
#include <string.h>
#include <cjson/cJSON.h>






int cam_option(const cJSON* json,cJSON* outdata,const char* method) 
{

	Cam_Method* sMethod = getMethod((char*)method);
	if(sMethod == NULL)
	{
		printf("sMethod is null");
		return -1;
	}
	printf("call %s \n" ,method);
	if(sMethod->nmethod != NULL)
	{
		int ret = (sMethod->nmethod)();
		if(ret!= 0){
			return 0;
		}
		return -1;
	}
	if(sMethod->dmethod != NULL)
	{
		cJSON* data = cJSON_GetObjectItem(json, "data");
		if(data == NULL){
			return -1;
		}
		double val  = cJSON_GetObjectItem(data, "val")->valuedouble;
		int ret = (sMethod->dmethod)(val);
		if(ret!= 0){
			return 0;
		}
		return -1;
	}

	if(sMethod->dpmethod != NULL)
	{
		double val ; 
		int x = (sMethod->dpmethod)(&val);
		if(x != 0)
		{
			cJSON_AddNumberToObject(outdata,"val",val);
			return 0;
		}
		return  -1;
	}
	if(sMethod->bmethod != NULL)
	{
		cJSON* data = cJSON_GetObjectItem(json, "data");
		if(data == NULL){
			return -1;
		}
		bool val  = cJSON_GetObjectItem(data, "val")->valueint;
		int ret = (sMethod->bmethod)(val);
		if(ret!= 0){
			return 0;
		}
		return -1;
	}
	if(sMethod->cmethod != NULL)
	{
		char*  val; 
		int x = (sMethod->cmethod)(val);
		if(x != 0)
		{
			cJSON_AddStringToObject(outdata,"val",val);
			return 0;
		}
		return  -1;
	}
	if(sMethod->damethod != NULL)
	{
		double  val[5]; 
		cJSON* data = cJSON_GetObjectItem(json, "data");
		if(data == NULL){
			return -1;
		}
		cJSON* array =  cJSON_GetObjectItem(data,"val");
		val[0]  = cJSON_GetArrayItem(array, 0)->valuedouble;
		val[1]  = cJSON_GetArrayItem(array, 1)->valuedouble;
		val[2]  = cJSON_GetArrayItem(array, 2)->valuedouble;
		val[3]  = cJSON_GetArrayItem(array, 3)->valuedouble;
		val[4]  = cJSON_GetArrayItem(array, 4)->valuedouble;
		int x = (sMethod->damethod)(val);
		if(x != 0){
			return 0;
		}
		return  -1;
	}
	if(sMethod->dapmethod != NULL)
	{
		double  val[5]; 
		int x = (sMethod->damethod)(val);
		if(x != 0)
		{
			cJSON* array=cJSON_CreateArray();
			//cJSON_AddItemToArray(,arrayobj);
			cJSON_AddItemToArray(array, cJSON_CreateNumber(val[0]));
			cJSON_AddItemToArray(array, cJSON_CreateNumber(val[1]));
			cJSON_AddItemToArray(array, cJSON_CreateNumber(val[2]));
			cJSON_AddItemToArray(array, cJSON_CreateNumber(val[3]));
			cJSON_AddItemToArray(array, cJSON_CreateNumber(val[4]));
			cJSON_AddItemToObject(outdata,"val",array);
			return 0;
		}
		return  -1;
	}
	return -1;
}


int main()
{
/*
	cJSON* jsonObj = cJSON_CreateObject();
	cJSON_AddStringToObject(jsonObj,"cmd","ls");
	char ret[1024*4];
	int code =system_option(jsonObj,ret);
	printf("out is %s code is %d\n",ret,code);
	return 0;
*/

	printf("open Caming...\n");
	Cam_Open();
	//printf("registerCapture...");
	//printf("SetAcquisition...\n");
	string methods[] = {"Cam_GetExposure","Cam_SetExposure","Cam_GetGain","Cam_SetGain"
,"Cam_SetAutoWhite","Cam_GetBalanceRatio_R","Cam_GetBalanceRatio_G","Cam_GetBalanceRatio_B","Cam_SetBalanceRatio_R"
,"Cam_SetBalanceRatio_G","Cam_SetBalanceRatio_B","Cam_Reset"};
	for(int i=0;i<sizeof(methods)/sizeof(string);i++)
	{
		cJSON* json = cJSON_CreateObject();
		cJSON_AddNumberToObject(json, "val", 0.2f);
		cJSON* data = cJSON_CreateObject();
		cam_option(json,data,methods[i].c_str());
printf("err msg %s\n",get_Err());
		printf("out data %s\n",cJSON_Print(data));
	}
}



