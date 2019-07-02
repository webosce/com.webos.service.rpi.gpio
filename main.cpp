#include <glib.h>
#include <string>
#include <luna-service2/lunaservice.h>
#include <wiringPi.h>
#include <wiringSerial.h>
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
   int ret;
   if(ret=wiringPiSetup() == -1)
   {
	LSMessageReplySetupFail(sh, message);
	return false;
   }
  
   retLSMessageReplySuccess(sh, message,ret);
   return true;
}

static bool setWiringPiPhys(LSHandle *sh, LSMessage* message, void *ctx) 
{
   LSError lserror;
   int ret;
   if(ret=wiringPiSetupPhys() == -1)
   {
	LSMessageReplySetupFail(sh, message);
	return false;
   }
   
   retLSMessageReplySuccess(sh, message,ret);
   return true;

}

static bool setWiringPiSys(LSHandle *sh, LSMessage* message, void *ctx) 
{
   LSError lserror;
   int ret;
   if(ret=wiringPiSetupSys() == -1)
   {
	LSMessageReplySetupFail(sh, message);
	return false;
   }
 
   LSMessageReplySuccess(sh, message);
   return true;

}

static bool setPinMode(LSHandle *sh, LSMessage* message, void *ctx)
{
   LSError lserror;
   int num, mode;
   json_object *pinObj;
   json_object *pinNum;
   json_object *pinmode;
   
   pinObj = json_tokener_parse(LSMessageGetPayload(message));

   pinNum = json_object_object_get(pinObj,"pinNum");
   pinmode = json_object_object_get(pinObj,"pinMode");
   num = json_object_get_int(pinNum);
   mode = json_object_get_int(pinmode);
    
   if((!num) || (!mode)) {
      LSMessageReplyErrorBadJson(sh,message);
      return false;
   }

   pinMode(num, mode);
   LSMessageReplySuccess(sh, message);
   
   return true;
}

static bool digitWrite(LSHandle *sh, LSMessage* message, void *ctx)
{
   LSError lserror;
   int num, value;
   json_object *pinObj;
   json_object *pinNum;
   json_object *pinValue;
   
   pinObj = json_tokener_parse(LSMessageGetPayload(message));

   pinNum = json_object_object_get(pinObj,"pinNum");
   pinValue = json_object_object_get(pinObj,"pinValue");
   num = json_object_get_int(pinNum);
   value = json_object_get_int(pinValue);
    
   if((!num) || (!value)) {
      LSMessageReplyErrorBadJson(sh,message);
      return false;
   }

   digitalWrite(num, value);
   LSMessageReplySuccess(sh, message);
   
   return true;
}

static bool setPullUpDnControl(LSHandle *sh, LSMessage* message, void *ctx)
{
   LSError lserror;
   int num, pud;
   json_object *pinObj;
   json_object *pinNum;
   json_object *pinPud;
   
   pinObj = json_tokener_parse(LSMessageGetPayload(message));

   pinNum = json_object_object_get(pinObj,"pinNum");
   pinPud = json_object_object_get(pinObj,"pinPud");
   num = json_object_get_int(pinNum);
   pud = json_object_get_int(pinPud);
    
   if((!num) || (!pud)) {
      LSMessageReplyErrorBadJson(sh,message);
      return false;
   }

   pullUpDnControl(num,pud);
   LSMessageReplySuccess(sh, message);
   
   return true;
}

static bool writePwm(LSHandle *sh, LSMessage* message, void *ctx)
{
   LSError lserror;
   int num, pwm;
   json_object *pinObj;
   json_object *pinNum;
   json_object *pinPwm;
   
   pinObj = json_tokener_parse(LSMessageGetPayload(message));

   pinNum = json_object_object_get(pinObj,"pinNum");
   pinPwm = json_object_object_get(pinObj,"pinPwm");
   num = json_object_get_int(pinNum);
   pwm = json_object_get_int(pinPwm);
    
   if((!num) || (!pwm)) {
      LSMessageReplyErrorBadJson(sh,message);
      return false;
   }

   pwmWrite(num, pwm);
   LSMessageReplySuccess(sh, message);
   
   return true;
}
static bool digitRead(LSHandle *sh, LSMessage* message, void *ctx)
{
   LSError lserror;
   int num;
   json_object *pinObj;
   json_object *pinNum;
   int ret;
 
   pinObj = json_tokener_parse(LSMessageGetPayload(message));

   pinNum = json_object_object_get(pinObj,"pinNum");
   num = json_object_get_int(pinNum);
    
   if(!num) {
      LSMessageReplyErrorBadJson(sh,message);
      return false;
   }

   ret = digitalRead(num);
   retLSMessageReplySuccess(sh, message,ret);
   
   return true;
}
static bool analRead(LSHandle *sh, LSMessage* message, void *ctx)
{
   LSError lserror;
   int num;
   json_object *pinObj;
   json_object *pinNum;
   int ret;  
   pinObj = json_tokener_parse(LSMessageGetPayload(message));

   pinNum = json_object_object_get(pinObj,"pinNum");
   num = json_object_get_int(pinNum);
    
   if(!num) {
      LSMessageReplyErrorBadJson(sh,message);
      return false;
   }

   ret=analogRead(num);
   retLSMessageReplySuccess(sh, message,ret);
   
   return true;
}
static bool analWrite(LSHandle *sh, LSMessage* message, void *ctx)
{
   LSError lserror;
   int num, value;
   json_object *pinObj;
   json_object *pinNum;
   json_object *pinValue;
   
   pinObj = json_tokener_parse(LSMessageGetPayload(message));

   pinNum = json_object_object_get(pinObj,"pinNum");
   pinValue = json_object_object_get(pinObj,"pinValue");
   num = json_object_get_int(pinNum);
   value = json_object_get_int(pinValue);
    
   if((!num) || (!value)) {
      LSMessageReplyErrorBadJson(sh,message);
      return false;
   }

   analogWrite(num, value);
   LSMessageReplySuccess(sh, message);
   
   return true;
}

static bool setWiringPiSPI(LSHandle *sh, LSMessage* message, void *ctx)
{
   
   LSError lserror;
   int channel, speed;
   json_object *pinObj;
   json_object *chan;
   json_object *spd;
   int ret;  
 
   pinObj = json_tokener_parse(LSMessageGetPayload(message));

   chan = json_object_object_get(pinObj,"channel");
   spd = json_object_object_get(pinObj,"speed");
   channel = json_object_get_int(chan);
   speed = json_object_get_int(spd);
    
   if((!channel) || (!speed)) {
      LSMessageReplyErrorBadJson(sh,message);
      return false;
   }

   ret = wiringPiSPISetup(channel, speed);
   retLSMessageReplySuccess(sh, message,ret);
   
   return true;
}

static bool wiringPiSPIRWData(LSHandle *sh, LSMessage* message, void *ctx)
{
   
   int channel, lenth;
   unsigned char *dat = NULL;
   LSError lserror;
   json_object *pinObj;
   json_object *chan;
   json_object *len;
   json_object *data;
   int ret;

   pinObj = json_tokener_parse(LSMessageGetPayload(message));

   chan = json_object_object_get(pinObj,"channel");
   data = json_object_object_get(pinObj,"data");
   len = json_object_object_get(pinObj, "len");

   channel = json_object_get_int(chan);
   lenth = json_object_get_int(len);
   dat = (unsigned char*)json_object_get_string(data);

   if((!channel) || (!lenth) || dat == NULL) {
      LSMessageReplyErrorBadJson(sh,message);
      return false;
   }

   ret = wiringPiSPIDataRW(channel, dat, lenth);
   retLSMessageReplySuccess(sh, message,ret);
   return true;
}
static bool piI2CSetup(LSHandle *sh, LSMessage *message, void *ctx)
{
   int ret;
   LSError lserror;
   json_object *pinObj;
   json_object *dev;
   int devId;
   
   pinObj=json_tokener_parse(LSMessageGetPayload(message));
   dev = json_object_object_get(pinObj,"devId");
   devId = json_object_get_int(dev);

   if((!devId)) {
      LSMessageReplyErrorBadJson(sh,message);
      return false;
   }
   ret = wiringPiI2CSetup(devId);
   retLSMessageReplySuccess(sh,message,ret);
   return true;
}

static bool piI2CRead(LSHandle *sh, LSMessage *message, void *ctx) 
{
   int ret;
   LSError lserror;
   json_object *pinObj;
   json_object *fd;
   int file;
   pinObj=json_tokener_parse(LSMessageGetPayload(message));
   fd = json_object_object_get(pinObj,"fd");
   file = json_object_get_int(fd);
   
   if(!fd) {
      LSMessageReplyErrorBadJson(sh,message);
      return false;
   }
   ret = wiringPiI2CRead(file);
   retLSMessageReplySuccess(sh,message,ret);
   return true;
}

static bool piI2CWrite(LSHandle *sh, LSMessage *message, void *ctx)
{
   int ret;
   LSError lserror;
   json_object *pinObj;
   json_object *fd;
   json_object *data;
   int dat;
   int file;
   

   pinObj=json_tokener_parse(LSMessageGetPayload(message));
   fd=json_object_object_get(pinObj,"fd");
   data=json_object_object_get(pinObj,"data");
   file=json_object_get_int(fd);
   dat=json_object_get_int(data);

   if((!file)||(!dat)) {
      LSMessageReplyErrorBadJson(sh,message);
      return false;
   }
   ret = wiringPiI2CWrite(file, dat);
   retLSMessageReplySuccess(sh,message,ret);
   return true;
}

static bool piI2CWriteReg8(LSHandle *sh, LSMessage *message, void *ctx)
{
   int ret;
   LSError lserror;
   json_object *pinObj;
   json_object *fd;
   json_object *data;
   json_object *reg;
   int dat;
   int file;
   int regi;

   pinObj=json_tokener_parse(LSMessageGetPayload(message));
   fd=json_object_object_get(pinObj,"fd");
   data=json_object_object_get(pinObj,"data");
   reg=json_object_object_get(pinObj,"reg");
   file=json_object_get_int(fd);
   dat=json_object_get_int(data);
   regi=json_object_get_int(reg);
   

   if(!dat || !regi || !file) {
      LSMessageReplyErrorBadJson(sh,message);
      return false;
   }
   ret=wiringPiI2CWriteReg8(file,regi,dat);
   retLSMessageReplySuccess(sh,message,ret);
   return true;
}  

static bool piI2CWriteReg16(LSHandle *sh, LSMessage *message, void *ctx)
{
   int ret;
   LSError lserror;
   json_object *pinObj;
   json_object *fd;
   json_object *data;
   json_object *reg;
   int dat;
   int file;
   int regi;

   pinObj=json_tokener_parse(LSMessageGetPayload(message));
   fd=json_object_object_get(pinObj,"fd");
   data=json_object_object_get(pinObj,"data");
   reg=json_object_object_get(pinObj,"reg");
   file=json_object_get_int(fd);
   dat=json_object_get_int(data);
   regi=json_object_get_int(reg);
   

   if(!dat || !regi || !file) {
      LSMessageReplyErrorBadJson(sh,message);
      return false;
   }
   ret=wiringPiI2CWriteReg16(file,regi,dat);
   retLSMessageReplySuccess(sh,message,ret);
   return true;
}

static bool piI2CReadReg8(LSHandle *sh, LSMessage *message,void *ctx)
{
   int ret;
   LSError lserror;
   json_object *pinObj;
   json_object *fd;
   json_object *reg;
   int file;
   int regi;

   pinObj=json_tokener_parse(LSMessageGetPayload(message));
   fd=json_object_object_get(pinObj,"fd");
   reg=json_object_object_get(pinObj,"reg");
   file=json_object_get_int(fd);
   regi=json_object_get_int(reg);
   

   if(!regi || !file) {
      LSMessageReplyErrorBadJson(sh,message);
      return false;
   }
   ret=wiringPiI2CReadReg8(file,regi);
   retLSMessageReplySuccess(sh,message,ret);
   return true;
}

static bool piI2CReadReg16(LSHandle *sh, LSMessage *message, void *ctx)
{
   int ret;
   LSError lserror;
   json_object *pinObj;
   json_object *fd;
   json_object *reg;
   int file;
   int regi;

   pinObj=json_tokener_parse(LSMessageGetPayload(message));
   fd=json_object_object_get(pinObj,"fd");
   reg=json_object_object_get(pinObj,"reg");
   file=json_object_get_int(fd);
   regi=json_object_get_int(reg);
   

   if(!regi || !file) {
      LSMessageReplyErrorBadJson(sh,message);
      return false;
   }
   ret=wiringPiI2CReadReg16(file,regi);
   retLSMessageReplySuccess(sh,message,ret);
   return true;
}

static bool openSerial(LSHandle *sh, LSMessage *message, void *ctx)
{
   int ret;
   LSError lserror;
   json_object *pinObj;
   json_object *device;
   json_object *baud;
   int num_baud;
   char *ch_device=NULL;
   

   pinObj=json_tokener_parse(LSMessageGetPayload(message));
   device=json_object_object_get(pinObj,"device");
   baud=json_object_object_get(pinObj, "baud");
   num_baud=json_object_get_int(baud);
   ch_device=(char *)json_object_get_string(baud);

   if(!num_baud || !ch_device) {
      LSMessageReplyErrorBadJson(sh,message);
   }
   ret=serialOpen(ch_device, num_baud);
   retLSMessageReplySuccess(sh, message, ret);
   return true;
}

static bool closeSerial(LSHandle *sh, LSMessage *message, void *ctx) 
{
   LSError lserror;
   json_object *pinObj;
   json_object *fd;
   int num_fd;
   
   pinObj=json_tokener_parse(LSMessageGetPayload(message));
   fd=json_object_object_get(pinObj, "fd");
   num_fd=json_object_get_int(fd);

   if(!fd) {
      LSMessageReplyErrorBadJson(sh,message);
   }
   serialClose(num_fd);
   LSMessageReplySuccess(sh, message);
   return true;
}

static bool putcharSerial(LSHandle *sh, LSMessage *message, void *ctx)
{
   int ret;
   LSError lserror;
   json_object *pinObj;
   json_object *c;
   json_object *fd;
   int num_fd;
   unsigned char ch_c=NULL;
   unsigned char *s=NULL;
 
   pinObj=json_tokener_parse(LSMessageGetPayload(message));
   c=json_object_object_get(pinObj,"c");
   fd=json_object_object_get(pinObj, "fd");
   num_fd=json_object_get_int(fd);
   s=(unsigned char*)json_object_get_string(c);
   ch_c = s[0];

   if(!num_fd || !ch_c) {
      LSMessageReplyErrorBadJson(sh,message);
   }

   serialPutchar(num_fd, ch_c);
   LSMessageReplySuccess(sh, message);
   return true;
}

static bool putsSerial(LSHandle *sh, LSMessage *message, void *ctx)
{
   LSError lserror;
   json_object *pinObj;
   json_object *s;
   json_object *fd;
   char *ch_s=NULL;
   int num_fd;
   
   pinObj=json_tokener_parse(LSMessageGetPayload(message));
   s=json_object_object_get(pinObj, "s");
   fd=json_object_object_get(pinObj,"fd");
   
   num_fd=json_object_get_int(fd);
   ch_s=(char*)json_object_get_string(s);
   
   if(!num_fd || !ch_s) {
      LSMessageReplyErrorBadJson(sh,message);
   }

   serialPuts(num_fd, ch_s);
   LSMessageReplySuccess(sh,message);
   return true;
}

static bool printfSerial(LSHandle *sh, LSMessage *message, void *ctx)
{
   LSError lserror;
   json_object *pinObj;
   json_object *fd;
   json_object *pi_message;
   char *ch_message=NULL;
   int num_fd;

   pinObj=json_tokener_parse(LSMessageGetPayload(message));
   fd=json_object_object_get(pinObj,"fd");
   pi_message=json_object_object_get(pinObj, "message");
   
   num_fd=json_object_get_int(fd);
   ch_message=(char *)json_object_get_string(pi_message);
   if(!num_fd || !ch_message) {
      LSMessageReplyErrorBadJson(sh, message);
   }
   
   serialPrintf(num_fd, ch_message);
   LSMessageReplySuccess(sh,message);
   return true;
}

static bool dataAvailSerial(LSHandle *sh, LSMessage *message, void *ctx)
{
   int ret;
   LSError lserror;
   json_object *pinObj;
   json_object *fd;
   int num_fd;

   json_tokener_parse(LSMessageGetPayload(message));
   fd=json_object_object_get(pinObj,"fd");
   num_fd=json_object_get_int(fd);

   if(!num_fd) {
      LSMessageReplyErrorBadJson(sh,message);
   }
  
   ret=serialDataAvail(num_fd);
   retLSMessageReplySuccess(sh,message,ret);

   return true;
}

static bool getcharSerial(LSHandle *sh, LSMessage *message, void *ctx)
{
   int ret;
   LSError lserror;
   json_object *pinObj;
   json_object *fd;
   int num_fd;

   json_tokener_parse(LSMessageGetPayload(message));
   fd=json_object_object_get(pinObj,"fd");
   num_fd=json_object_get_int(fd);

   if(!num_fd) {
      LSMessageReplyErrorBadJson(sh,message);
   }
  
   ret=serialGetchar(num_fd);
   retLSMessageReplySuccess(sh,message,ret);

   return true;
}

static bool flushSerial(LSHandle *sh, LSMessage *message, void *ctx)
{
   LSError lserror;
   json_object *pinObj;
   json_object *fd;
   int num_fd;

   json_tokener_parse(LSMessageGetPayload(message));
   fd=json_object_object_get(pinObj,"fd");
   num_fd=json_object_get_int(fd);

   if(!num_fd) {
      LSMessageReplyErrorBadJson(sh,message);
   }
  
   serialFlush(num_fd);
   LSMessageReplySuccess(sh,message);

   return true;
}

static LSMethod serviceMethods[] = {
   { "gpio", cbHello }, 
   {"wiringPiSetup", setWiringPi},
   {"wiringPiSetupPhys", setWiringPiPhys},
   {"wiringPiSetupSys", setWiringPiSys},
   {"pinMode", setPinMode},
   {"digitalWrite", digitWrite},
   {"pullUpDnControl", setPullUpDnControl},
   {"pwmWrite", writePwm},
   {"digitalRead", digitRead},
   {"analogRead", analRead},
   {"analogWrite", analWrite},
   {"wiringPiSPISetup",setWiringPiSPI},
   {"wiringPiSPIDataRW", wiringPiSPIRWData},
   {"wiringPiI2CSetup ", piI2CSetup},
   {"wiringPiI2CRead ", piI2CRead},
   {"wiringPiI2CWrite",piI2CWrite},
   {"wiringPiI2CWriteReg8",piI2CWriteReg8},
   {"wiringPiI2CWriteReg16",piI2CWriteReg16},
   {"wiringPiI2CReadReg8",piI2CReadReg8},
   {"wiringPiI2CReadReg16 ",piI2CReadReg16},
   {"serialOpen", openSerial},
   {"serialClose", closeSerial},
   {"serialPutchar", putcharSerial},
   {"serialPuts", putsSerial},
   {"serialPrintf", printfSerial},
   {"serialDataAvail", dataAvailSerial},
   {"serialGetchar",getcharSerial},
   {"serialFlush", flushSerial}
};
 
int main(int argc, char* argv[])
{
    LSError lserror;
    LSErrorInit(&lserror);
 
    GMainLoop* mainLoop = g_main_loop_new(nullptr, false);
    LSHandle *m_handle = nullptr;   
 
    if(!LSRegister("com.webos.service.rpi.gpio", &m_handle, &lserror))
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
