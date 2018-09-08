#include <string.h>
#include "../include/gpioBase.h"

void LSMessageReplyErrorBadJson(LSHandle *sh, LSMessage *msg)
{
   LSError lserror;
   LSErrorInit(&lserror);

   bool retVal = LSMessageReply(sh, msg, "{ \"returnValue\": false, \"errorText\": \"Malformed json\"}", NULL);
   if(!retVal) {
      LSErrorPrint(&lserror,stderr);
      LSErrorFree(&lserror);
   }
}

void LSMessageReplySetupFail(LSHandle *sh, LSMessage *msg) 
{
   LSError lserror;
   LSErrorInit(&lserror);

   bool retVal = LSMessageReply(sh, msg, "{ \"returnValue\": false, \"errorText\": \"setup fail!\"}", NULL);
   if(!retVal) {
      LSErrorPrint(&lserror,stderr);
      LSErrorFree(&lserror);
   }
}

void LSMessageReplySuccess(LSHandle *sh, LSMessage *msg) 
{
   LSError lserror;
   LSErrorInit(&lserror);

   bool retVal = LSMessageReply(sh, msg, "{ \"returnValue\": true, \"Success\": \"OK\"}", NULL);
   if(!retVal) {
      LSErrorPrint(&lserror,stderr);
      LSErrorFree(&lserror);
   }
}
