// CEaxContext::SetEffectProperty_00c68f90(unsigned long,unsigned long,void const *,unsigned long,SEaxEvent * *) -- dispatch reverb and FX slot updates with deferred commit.
// Leaf: ?SetEffectProperty_00c68f90@CEaxContext@@QAEJKKPBXKPAPAUSEaxEvent@@@Z
#include "engine/CEaxContext.h"
#include <string.h>
#pragma intrinsic(memcmp)
long CEaxContext::SetEffectProperty_00c68f90(unsigned long slot,unsigned long property,const void* value,unsigned long size,SEaxEvent** events)
{
 long result=-3;
 ClearEvents();
 if(slot>=4)return result;
 unsigned long parameter=property&0x7fffffff;
 if(parameter<0x10000){
  if(memcmp(&m_slots[slot].guidLoadEffect,&EAX_REVERB_EFFECT,sizeof(GUID))!=0){result=-9;m_lastError=-3;}
  else switch(parameter){
  case EAXREVERB_NONE:result=0;break;
 case EAXREVERB_ALLPARAMETERS:
  if(value&&size>=sizeof(EAXREVERBPROPERTIES))result=SetReverbAllParameters(slot,(const EAXREVERBPROPERTIES*)value);
  break;
 case EAXREVERB_ENVIRONMENT:
  if(value&&size>=sizeof(unsigned long))result=SetEnvironment(slot,*(const unsigned long*)value);
  break;
 case EAXREVERB_ENVIRONMENTSIZE:
  if(value&&size>=sizeof(float))result=SetEnvironmentSize(slot,*(const float*)value);
  break;
 case EAXREVERB_ENVIRONMENTDIFFUSION:
  if(value&&size>=sizeof(float))result=SetEnvironmentDiffusion(slot,*(const float*)value);
  break;
 case EAXREVERB_ROOM:
  if(value&&size>=sizeof(long))result=SetRoom(slot,*(const long*)value);
  break;
 case EAXREVERB_ROOMHF:
  if(value&&size>=sizeof(long))result=SetRoomHF(slot,*(const long*)value);
  break;
 case EAXREVERB_ROOMLF:
  if(value&&size>=sizeof(long))result=SetRoomLF(slot,*(const long*)value);
  break;
 case EAXREVERB_DECAYTIME:
  if(value&&size>=sizeof(float))result=SetDecayTime(slot,*(const float*)value,true,false);
  break;
 case EAXREVERB_DECAYHFRATIO:
  if(value&&size>=sizeof(float))result=SetDecayHFRatio(slot,*(const float*)value);
  break;
 case EAXREVERB_DECAYLFRATIO:
  if(value&&size>=sizeof(float))result=SetDecayLFRatio(slot,*(const float*)value);
  break;
 case EAXREVERB_REFLECTIONS:
  if(value&&size>=sizeof(long))result=SetReflections(slot,*(const long*)value,true,false);
  break;
 case EAXREVERB_REFLECTIONSDELAY:
  if(value&&size>=sizeof(float))result=SetReflectionsDelay(slot,*(const float*)value,true,false);
  break;
 case EAXREVERB_REFLECTIONSPAN:
  if(value&&size>=sizeof(EAXVECTOR))result=SetReflectionsPan(slot,*(const EAXVECTOR*)value);
  break;
 case EAXREVERB_REVERB:
  if(value&&size>=sizeof(long))result=SetReverb(slot,*(const long*)value,true,false);
  break;
 case EAXREVERB_REVERBDELAY:
  if(value&&size>=sizeof(float))result=SetReverbDelay(slot,*(const float*)value,true,false);
  break;
 case EAXREVERB_REVERBPAN:
  if(value&&size>=sizeof(EAXVECTOR))result=SetReverbPan(slot,*(const EAXVECTOR*)value);
  break;
 case EAXREVERB_ECHOTIME:
  if(value&&size>=sizeof(float))result=SetEchoTime(slot,*(const float*)value,true,false);
  break;
 case EAXREVERB_ECHODEPTH:
  if(value&&size>=sizeof(float))result=SetEchoDepth(slot,*(const float*)value);
  break;
 case EAXREVERB_MODULATIONTIME:
  if(value&&size>=sizeof(float))result=SetModulationTime(slot,*(const float*)value,true,false);
  break;
 case EAXREVERB_MODULATIONDEPTH:
  if(value&&size>=sizeof(float))result=SetModulationDepth(slot,*(const float*)value);
  break;
 case EAXREVERB_AIRABSORPTIONHF:
  if(value&&size>=sizeof(float))result=SetSlotAirAbsorptionHF(slot,*(const float*)value);
  break;
 case EAXREVERB_HFREFERENCE:
  if(value&&size>=sizeof(float))result=SetSlotHFReference(slot,*(const float*)value);
  break;
 case EAXREVERB_LFREFERENCE:
  if(value&&size>=sizeof(float))result=SetLFReference(slot,*(const float*)value);
  break;
 case EAXREVERB_ROOMROLLOFFFACTOR:
  if(value&&size>=sizeof(float))result=SetRoomRolloffFactor(slot,*(const float*)value);
  break;
 case EAXREVERB_FLAGS:
  if(value&&size>=sizeof(unsigned long))result=SetReverbFlags(slot,*(const unsigned long*)value);
  break;
  default:result=-3;break;
  }
 }else{
  switch(property){
  case EAXFXSLOT_NONE:result=0;break;
 case EAXFXSLOT_ALLPARAMETERS:
  if(value&&size>=sizeof(EAXFXSLOTPROPERTIES))result=SetSlotProperty(slot,*(const EAXFXSLOTPROPERTIES*)value);
  break;
 case EAXFXSLOT_LOADEFFECT:
  if(value&&size>=sizeof(GUID))result=SetSlotLoadEffect(slot,*(const GUID*)value);
  break;
 case EAXFXSLOT_VOLUME:
  if(value&&size>=sizeof(long))result=SetSlotVolume(slot,*(const long*)value);
  break;
 case EAXFXSLOT_LOCK:
  if(value&&size>=sizeof(long))result=SetSlotLock(slot,*(const long*)value);
  break;
 case EAXFXSLOT_FLAGS:
  if(value&&size>=sizeof(unsigned long))result=SetSlotFlags(slot,*(const unsigned long*)value);
  break;
  default:result=-3;break;
  }
 }
 if(result>=0&&!(property&0x80000000)){
  result=CommitDeferredSettings();
  if(result>=0)*events=m_pEvents;
 }
 return result;
}