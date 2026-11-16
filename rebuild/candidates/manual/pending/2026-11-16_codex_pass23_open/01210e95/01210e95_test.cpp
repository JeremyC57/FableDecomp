// Self-contained fixture, real shared class and pasted production implementation.
#ifndef _MSC_VER
#define __fastcall
#endif
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stddef.h>
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

static void check(bool x){if(!x){fputs("audio channel assertion failed\n",stderr);exit(1);}}
static char Empty[]="";
char* _afxPchNil=Empty;
CString::~CString(){}
PKCCriticalSection::PKCCriticalSection(){memset(m_section,0,sizeof(m_section));}
PKCCriticalSection::~PKCCriticalSection(){}
PKCCriticalSection g_lock_0143bb14;
static CUnprovenAudioChannelBank* active;
static int entered,left,held,failAt,mutate;
static unsigned int selected;
static float changed;
PKCProtectionInstance::PKCProtectionInstance(PKCCriticalSection* section):m_pSection(section),m_tick(7){
 check(section==&g_lock_0143bb14&&!held);++entered;
#ifndef _MSC_VER
 if(failAt==1)throw 1;
#endif
 held=1;if(mutate&&selected<16)active->Channels[selected].Second=changed;
}
PKCProtectionInstance::~PKCProtectionInstance(){check(held&&m_pSection==&g_lock_0143bb14&&m_tick==7);held=0;++left;}
CUnprovenAudioChannelBank::CUnprovenAudioChannelBank(void* owner):Owner(owner){
 memset(Channels,0,sizeof(Channels));for(int i=0;i<8;++i)AuxiliaryValues[i]=1.0f;
}
CUnprovenAudioChannelBank::~CUnprovenAudioChannelBank(){}
void CUnprovenAudioChannelBank::SetChannel(unsigned int,float,float){check(false);}
float CUnprovenAudioChannelBank::ReadFirst(unsigned int){check(false);return 0.0f;}
unsigned int CUnprovenAudioChannelBank::GetChannelCount(){return 16;}
void CUnprovenAudioChannelBank::SetChannelName(unsigned int,const char*){check(false);}
void CUnprovenAudioChannelBank::SetAuxiliaryValue(unsigned int,float){check(false);}
float CUnprovenAudioChannelBank::GetAuxiliaryValue(unsigned int){check(false);return 0.0f;}
unsigned int CUnprovenAudioChannelBank::GetAuxiliaryCount(){return 8;}
void CUnprovenAudioChannelBank::SetAuxiliaryName(unsigned int,const char*){check(false);}
static float expected(float value,bool wrap){
 if(wrap&&value>=3.1415927410125732f)return (float)((double)value-6.2831854820251465);
 if(wrap&&value<-3.1415927410125732f)return (float)((double)value+6.2831854820251465);
 return value;
}
int main(){
#ifdef _MSC_VER
 check(sizeof(CUnprovenAngularChannel)==52&&sizeof(CUnprovenAudioChannelBank)==0x3c8);
 check(offsetof(CUnprovenAudioChannelBank,Channels)==4&&offsetof(CUnprovenAudioChannelBank,AuxiliaryValues)==0x344&&offsetof(CUnprovenAudioChannelBank,ChannelNames)==0x364&&offsetof(CUnprovenAudioChannelBank,AuxiliaryNames)==0x3a4&&offsetof(CUnprovenAudioChannelBank,Owner)==0x3c4);
#endif
 const float inputs[]={0.0f,0.5f,-0.5f,3.0f,-3.0f,3.5f,-3.5f,7.0f,-7.0f,16.0f,-16.0f,1.0f,3.1415927410125732f,-3.1415927410125732f};
 int cases=0;
 for(unsigned int index=0;index<20;++index)for(int wrap=0;wrap<2;++wrap)for(unsigned int k=0;k<sizeof(inputs)/sizeof(inputs[0]);++k)for(int change=0;change<2;++change){
  CUnprovenAudioChannelBank bank(&index);active=&bank;selected=index;mutate=change;changed=inputs[k];
  for(int j=0;j<16;++j){bank.Channels[j].First=64.0f+j;bank.Channels[j].Second=inputs[(k+1)%(sizeof(inputs)/sizeof(inputs[0]))];bank.Channels[j].WrapAngle=wrap!=0;memset(bank.Channels[j].Unrecovered08,0x5a,sizeof(bank.Channels[j].Unrecovered08));}
  float value=change?inputs[k]:bank.Channels[0].Second;entered=left=held=failAt=0;float result=bank.ReadSecond(index);
  check(result==(index<16?expected(value,wrap!=0):0.0f));check(entered==1&&left==1&&!held&&bank.Owner==&index);
  for(int j=0;j<16;++j){check(bank.Channels[j].First==64.0f+j&&bank.Channels[j].WrapAngle==(wrap!=0));for(unsigned int q=0;q<sizeof(bank.Channels[j].Unrecovered08);++q)check(bank.Channels[j].Unrecovered08[q]==0x5a);}
  ++cases;
 }
 check(cases==1120);
#ifndef _MSC_VER
 for(unsigned int index=0;index<20;++index){CUnprovenAudioChannelBank bank(0);active=&bank;selected=index;entered=left=held=0;failAt=1;bool caught=false;try{bank.ReadSecond(index);}catch(int x){caught=x==1;}check(caught&&entered==1&&left==0&&!held);}
#endif
 puts("01210e95_TEST PASS");return 0;
}
