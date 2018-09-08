#include <glib.h>
#include <string>
#include <luna-service2/lunaservice.h>
#include <wiringPi.h>
#include <wiringPiI2C.h>
#include <wiringPiSPI.h>
#include <json-c/json.h>
#include "./include/logging.h"
#include "./include/gpioBase.h"

static bool cbHello(LSHandle *sh, LSMessage* message, void* ctx)
{
    int i = 0;
    LSError lserror;
    json_object *pinObj;
    json_object *pinNum;
    int num=0;
   
    GPIO_LOG_DEBUG("test");
    pinObj = json_tokener_parse(LSMessageGetPayload(message));

    pinNum = json_object_object_get(pinObj,"pinNum");
    num = json_object_get_int(pinNum);
    
    if(!num) {
	LSMessageReplyErrorBadJson(sh,message);
    }


    if(wiringPiSetup() == -1) {
	LSMessageReplySetupFail(sh, message);
	return false;
    }

    pinMode(num, OUTPUT);
    
    for(i=0; i<10; i++) {
	digitalWrite(num, 1);
	delay(1000);
	digitalWrite(num, 0);
	delay(1000);
    }

    LSMessageReplySuccess(sh, message);
    return true;
}

static bool setWiringPi(LSHandle *sh, LSMessage* message, void *ctx)
{
   LSError lserror;

   if(wiringPiSetup() == -1)
   {
	LSMessageReplySetupFail(sh, message);
	return false;
   }
  
   LSMessageReplySuccess(sh, message);
   return true;
}

static bool setWiringPiPhys(LSHandle *sh, LSMessage* message, void *ctx) 
{
   LSError lserror;

   if(wiringPiSetupPhys() == -1)
   {
	LSMessageReplySetupFail(sh, message);
	return false;
   }
   
   LSMessageReplySuccess(sh, message);
   return true;

}

static bool setWiringPiSys(LSHandle *sh, LSMessage* message, void *ctx) 
{
   LSError lserror;

   if(wiringPiSetupSys() == -1)
   {
	LSMessageReplySetupFail(sh, message);
	return false;
   }
 
   LSMessageReplySuccess(sh, message);
   return true;

}
static LSMethod serviceMethods[] = {
    { "gpio", cbHello }, {"setWiringPi", setWiringPi},{"setWiringPiPhys", setWiringPiPhys}, {"setWiringPiSys", setWiringPiSys}
};
 
int main(int argc, char* argv[])
{
    g_print("Start com.gpiotest.service.native\n");
 
    LSError lserror;
    LSErrorInit(&lserror);
 
    GMainLoop* mainLoop = g_main_loop_new(nullptr, false);
    LSHandle *m_handle = nullptr;   
 
    if(!LSRegister("com.gpiotest.service.native", &m_handle, &lserror))
    {
        g_print("Unable to register to luna-bus\n");
        LSErrorPrint(&lserror, stdout);
 
        return false;
    }
 
    if (!LSRegisterCategory(m_handle, "/", serviceMethods, NULL, NULL, &lserror))
    {
        g_print("Unable to register category and method\n");
        LSErrorPrint(&lserror, stdout);
 
        return false;
    }
 
    if(!LSGmainAttach(m_handle, mainLoop, &lserror))
    {
        g_print("Unable to attach service\n");
        LSErrorPrint(&lserror, stdout);
 
        return false;
    }
 
    g_main_loop_run(mainLoop);
 
    if(!LSUnregister(m_handle, &lserror))
    {
        g_print("Unable to unregister service\n");
        LSErrorPrint(&lserror, stdout);
 
        return false;
    }
 
    g_main_loop_unref(mainLoop);
    mainLoop = nullptr;
 
    return 0;
}
