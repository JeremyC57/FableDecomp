// CEAXBuffer::SetSourceProperty_00c68510(unsigned long,void const *,unsigned long,_CHANGES * *) -- dispatch EAX 4 source updates and return committed notifications.
// Leaf: ?SetSourceProperty_00c68510@CEAXBuffer@@QAEJKPBXKPAPAU_CHANGES@@@Z
#include "engine/CEAXBuffer.h"
long CEAXBuffer::SetSourceProperty_00c68510(unsigned long property,const void* value,unsigned long size,_CHANGES** events)
{
 long result=-3;
 m_lpEAXListener->ClearEvents();
 switch(property&0x7fffffff){
 case EAXSOURCE_NONE:result=0;break;
 case EAXSOURCE_ALLPARAMETERS:
  if(value&&size>=sizeof(EAXSOURCEPROPERTIES))result=SetAllParameters((const EAXSOURCEPROPERTIES*)value);
  break;
 case EAXSOURCE_OBSTRUCTIONPARAMETERS:
  if(value&&size>=sizeof(EAXOBSTRUCTIONPROPERTIES))result=SetObstructionParameters((const EAXOBSTRUCTIONPROPERTIES*)value);
  break;
 case EAXSOURCE_OCCLUSIONPARAMETERS:
  if(value&&size>=sizeof(EAXOCCLUSIONPROPERTIES))result=SetOcclusionParameters((const EAXOCCLUSIONPROPERTIES*)value);
  break;
 case EAXSOURCE_EXCLUSIONPARAMETERS:
  if(value&&size>=sizeof(EAXEXCLUSIONPROPERTIES))result=SetExclusionParameters((const EAXEXCLUSIONPROPERTIES*)value);
  break;
 case EAXSOURCE_DIRECT:
  if(value&&size>=sizeof(long))result=SetDirect(*(const long*)value);
  break;
 case EAXSOURCE_DIRECTHF:
  if(value&&size>=sizeof(long))result=SetDirectHF(*(const long*)value);
  break;
 case EAXSOURCE_ROOM:
  if(value&&size>=sizeof(long))result=SetRoom(*(const long*)value);
  break;
 case EAXSOURCE_ROOMHF:
  if(value&&size>=sizeof(long))result=SetRoomHF(*(const long*)value);
  break;
 case EAXSOURCE_OBSTRUCTION:
  if(value&&size>=sizeof(long))result=SetObstruction(*(const long*)value);
  break;
 case EAXSOURCE_OBSTRUCTIONLFRATIO:
  if(value&&size>=sizeof(float))result=SetObstructionLFRatio(*(const float*)value);
  break;
 case EAXSOURCE_OCCLUSION:
  if(value&&size>=sizeof(long))result=SetOcclusion(*(const long*)value);
  break;
 case EAXSOURCE_OCCLUSIONLFRATIO:
  if(value&&size>=sizeof(float))result=SetOcclusionLFRatio(*(const float*)value);
  break;
 case EAXSOURCE_OCCLUSIONROOMRATIO:
  if(value&&size>=sizeof(float))result=SetOcclusionRoomRatio(*(const float*)value);
  break;
 case EAXSOURCE_OCCLUSIONDIRECTRATIO:
  if(value&&size>=sizeof(float))result=SetOcclusionDirectRatio(*(const float*)value);
  break;
 case EAXSOURCE_EXCLUSION:
  if(value&&size>=sizeof(long))result=SetExclusion(*(const long*)value);
  break;
 case EAXSOURCE_EXCLUSIONLFRATIO:
  if(value&&size>=sizeof(float))result=SetExclusionLFRatio(*(const float*)value);
  break;
 case EAXSOURCE_OUTSIDEVOLUMEHF:
  if(value&&size>=sizeof(long))result=SetOutsideVolumeHF(*(const long*)value);
  break;
 case EAXSOURCE_DOPPLERFACTOR:
  if(value&&size>=sizeof(float))result=SetDopplerFactor(*(const float*)value);
  break;
 case EAXSOURCE_ROLLOFFFACTOR:
  if(value&&size>=sizeof(float))result=SetRolloffFactor(*(const float*)value);
  break;
 case EAXSOURCE_ROOMROLLOFFFACTOR:
  if(value&&size>=sizeof(float))result=SetRoomRolloffFactor(*(const float*)value);
  break;
 case EAXSOURCE_AIRABSORPTIONFACTOR:
  if(value&&size>=sizeof(float))result=SetAirAbsorptionFactor(*(const float*)value);
  break;
 case EAXSOURCE_FLAGS:
  if(value&&size>=sizeof(unsigned long))result=SetFlags(*(const unsigned long*)value);
  break;
 case EAXSOURCE_SENDPARAMETERS:
  if(value&&size>=sizeof(EAXSOURCESENDPROPERTIES))result=SetSendParameters((const EAXSOURCESENDPROPERTIES*)value,size);
  break;
 case EAXSOURCE_ALLSENDPARAMETERS:
  if(value&&size>=sizeof(EAXSOURCEALLSENDPROPERTIES))result=SetAllSendParameters((const EAXSOURCEALLSENDPROPERTIES*)value,size);
  break;
 case EAXSOURCE_OCCLUSIONSENDPARAMETERS:
  if(value&&size>=sizeof(EAXSOURCEOCCLUSIONSENDPROPERTIES))result=SetOcclusionSendParameters((const EAXSOURCEOCCLUSIONSENDPROPERTIES*)value,size);
  break;
 case EAXSOURCE_EXCLUSIONSENDPARAMETERS:
  if(value&&size>=sizeof(EAXSOURCEEXCLUSIONSENDPROPERTIES))result=SetExclusionSendParameters((const EAXSOURCEEXCLUSIONSENDPROPERTIES*)value,size);
  break;
 case EAXSOURCE_ACTIVEFXSLOTID:
  if(value&&size>=sizeof(GUID))result=SetActiveFXSlotID((const GUID*)value,size);
  break;
 default:result=-3;break;
 }
 if(result>=0&&!(property&0x80000000)){
  result=CommitEAX3();
  if(result>=0)*events=m_lpEAXListener->lpChanges;
 }
 return result;
}