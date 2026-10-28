#include <stdio.h>
#include <string.h>
#define GetProcAddress TestGetProcAddress
// CAudioContext::SourceRewind(unsigned long value) -- Resolve and cache the OpenAL entry point before forwarding the argument.
// Leaf: ?SourceRewind@CAudioContext@@QAEXK@Z
// Identity: own literal alSourceRewind and function slot0x84; actual helper00c4d740 and kernel GetProcAddress0143fe44. OpenAL API identity proven, wrapper class/method spelling descriptive existing convention.
#ifndef _MSC_VER
#define __cdecl
#define __stdcall
#define __declspec(x)
#endif
extern "C" void* __stdcall GetProcAddress(void*,const char*);
typedef void (__cdecl* SourceFunction)(unsigned long);
typedef void (__cdecl* DistanceFunction)(long);
typedef const char* (__cdecl* StringFunction)(long);
// Layout: no shared header. Actual module+4, source callbacks6c/74/7c/84, stringd0/distanced4, resolver flag104. Original libpk class name unknown; established CAudioContext binding retained. Genuine virtual interface slot0 and untouched fields retained as unmodeled ranges.
struct CAudioContext{virtual void UnusedSlot0();void* Module;unsigned char Unmodeled08[100];SourceFunction pfnSourcePlay;unsigned char Unmodeled70[4];SourceFunction pfnSourcePause;unsigned char Unmodeled78[4];SourceFunction pfnSourceStop;unsigned char Unmodeled80[4];SourceFunction pfnSourceRewind;unsigned char Unmodeled88[72];StringFunction pfnGetString;DistanceFunction pfnDistanceModel;unsigned char UnmodeledD8[44];bool UseALGetProcAddress;void* GetProcAddress(const char*);void DistanceModel(long);const char* GetString(long);void SourceRewind(unsigned long);void SourcePlay(unsigned long);void SourceStop(unsigned long);void SourcePause(unsigned long);};
void CAudioContext::SourceRewind(unsigned long value){if(!pfnSourceRewind){if(UseALGetProcAddress)pfnSourceRewind=(SourceFunction)GetProcAddress("alSourceRewind");else pfnSourceRewind=(SourceFunction)::GetProcAddress(Module,"alSourceRewind");}return pfnSourceRewind(value);}
static int directResolves,alResolves,calls,errors;static long lastValue;static CAudioContext* owner;static void* module;static const char reply[]="OpenAL test";void CAudioContext::UnusedSlot0(){}
static void __cdecl Target(unsigned long value){++calls;lastValue=(long)value;}
extern "C" void* __stdcall GetProcAddress(void* m,const char* name){++directResolves;if(m!=module||strcmp(name,"alSourceRewind"))++errors;return (void*)&Target;}
void* CAudioContext::GetProcAddress(const char* name){++alResolves;if(this!=owner||strcmp(name,"alSourceRewind"))++errors;return (void*)&Target;}
int main(){CAudioContext context;owner=&context;int token;module=&token;context.Module=module;context.pfnSourceRewind=0;context.UseALGetProcAddress=false;context.SourceRewind(17);bool ok=true;ok=ok&&calls==1&&lastValue==17&&directResolves==1&&!alResolves&&!errors;context.SourceRewind(23);ok=ok&&calls==2&&lastValue==23&&directResolves==1;context.pfnSourceRewind=0;context.UseALGetProcAddress=true;context.SourceRewind(31);ok=ok&&calls==3&&lastValue==31&&alResolves==1&&directResolves==1&&!errors;context.SourceRewind(41);ok=ok&&calls==4&&lastValue==41&&alResolves==1;puts(ok?"00c725b0_TEST PASS":"00c725b0_TEST FAIL");return ok?0:1;}