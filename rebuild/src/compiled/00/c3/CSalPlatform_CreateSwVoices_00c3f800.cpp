// 00c3f800 CSalPlatform::CreateSwVoices
// Flags: /EHsc
// Leaf: ?CreateSwVoices@CSalPlatform@@AAE_NXZ
#define FABLE_RETAIL_CSALPLATFORM_CREATE_SW_PASS23
#define FABLE_RETAIL_PASS22_CCODACONTEXT_CREATE
#include "engine/CSalPlatform.h"
#include "engine/CSalSw2DVoice.h"
#include "engine/CSalSw3DVoice.h"
#include "engine/CodaCaps.h"
// Layout: native array strides250/248; genuine six-channel history and gain arrays.
FABLE_STATIC_ASSERT(sizeof(CSalSw2DVoice)==0x250);
FABLE_STATIC_ASSERT(sizeof(CSalSw3DVoice)==0x248);
// SDK2.10 changes this method's bool result into an int error code. Native caller tests AL.
bool CSalPlatform::CreateSwVoices()
{
 if(g_SalCaps.SW2DVoice.nVoices>0){
  m_Sw2DVoiceMem=new CSalSw2DVoice[g_SalCaps.SW2DVoice.nVoices];
  if(!m_Sw2DVoiceMem){
   g_SalCaps.SW2DVoice.nVoices=0;
   g_SalCaps.SW2DVoice.nFreeVoices=0;
   g_SalCaps.SW3DVoice.nVoices=0;
   g_SalCaps.SW3DVoice.nFreeVoices=0;
   return false;
  }
  m_Sw2DVoices.CreateStatic(m_Sw2DVoiceMem,sizeof(CSalSw2DVoice),g_SalCaps.SW2DVoice.nVoices);
 }
 if(g_SalCaps.SW3DVoice.nVoices>0){
  m_Sw3DVoiceMem=new CSalSw3DVoice[g_SalCaps.SW3DVoice.nVoices];
  if(!m_Sw3DVoiceMem){
   g_SalCaps.SW3DVoice.nVoices=0;
   g_SalCaps.SW3DVoice.nFreeVoices=0;
   return false;
  }
  m_Sw3DVoices.CreateStatic(m_Sw3DVoiceMem,sizeof(CSalSw3DVoice),g_SalCaps.SW3DVoice.nVoices);
 }
 return true;
}