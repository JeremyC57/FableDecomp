// 01210e95 CUnprovenAudioChannelBank::ReadSecond generated cleanup
// Leaf: ?ReadSecond@CUnprovenAudioChannelBank@@UAEMI@Z
// Flags: /EHsc
// Parent00c0af90,193bytes; generated8-byte guard cleanup and10-byte handler.
#define FABLE_RETAIL_AUDIO_CHANNEL_BANK_PASS23
#define FABLE_RETAIL_CSTRING_BUILDERS
#define FABLE_RETAIL_CSTRING_DEFAULT_CTOR
#define FABLE_RETAIL_CSTRING_ALLOCCOPY
#include "libpk/CUnprovenAudioChannelBank.h"
#include "libpk/PKCProtectionInstance.h"
extern PKCCriticalSection g_lock_0143bb14;
// Descriptive libpk class and method; exact native virtual slot1, not free/nonvirtual.
float CUnprovenAudioChannelBank::ReadSecond(unsigned int index)
{
 PKCProtectionInstance guard(&g_lock_0143bb14);
 if(index>=16)return 0.0f;
 CUnprovenAngularChannel& channel=Channels[index];
 const float value=channel.Second;
 return channel.WrapAngle ?
  (value>=3.1415927410125732f ? value-6.2831854820251465f :
   (value<-3.1415927410125732f ? value+6.2831854820251465f : value)) : value;
}
