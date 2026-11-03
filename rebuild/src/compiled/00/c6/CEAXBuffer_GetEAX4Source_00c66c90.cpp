// CEAXBuffer::GetEAX4Source(unsigned long,void*,unsigned long,unsigned long*) -- Read EAX source properties.
// Leaf: ?GetEAX4Source@CEAXBuffer@@QAEHKPAXKPAK@Z
#include "engine/CEAXBuffer.h"
#include "engine/CEAXListener.h"
#include <string.h>
int CEAXBuffer::GetEAX4Source(unsigned long property,void* data,unsigned long size,unsigned long* returned)
{
 long result=0;
 switch(property & 0x7fffffff) {
 case EAXSOURCE_NONE: *returned=0; break;
 case EAXSOURCE_ALLPARAMETERS:
  if(data && size >= 72) { *(EAXSOURCEPROPERTIES*)data=*(const EAXSOURCEPROPERTIES*)&m_Source.eax4BP; *returned=72; }
  else { m_lpEAXListener->SetEAX4ErrorCode(-2); result=-3; }
  break;
 case EAXSOURCE_OBSTRUCTIONPARAMETERS:
  if(data && size >= 8) { ((EAXOBSTRUCTIONPROPERTIES*)data)->lObstruction=m_Source.eax4BP.lObstruction;((EAXOBSTRUCTIONPROPERTIES*)data)->flObstructionLFRatio=m_Source.eax4BP.flObstructionLFRatio; *returned=8; }
  else { m_lpEAXListener->SetEAX4ErrorCode(-2); result=-3; }
  break;
 case EAXSOURCE_OCCLUSIONPARAMETERS:
  if(data && size >= 16) { ((EAXOCCLUSIONPROPERTIES*)data)->lOcclusion=m_Source.eax4BP.lOcclusion;((EAXOCCLUSIONPROPERTIES*)data)->flOcclusionLFRatio=m_Source.eax4BP.flOcclusionLFRatio;((EAXOCCLUSIONPROPERTIES*)data)->flOcclusionRoomRatio=m_Source.eax4BP.flOcclusionRoomRatio;((EAXOCCLUSIONPROPERTIES*)data)->flOcclusionDirectRatio=m_Source.eax4BP.flOcclusionDirectRatio; *returned=16; }
  else { m_lpEAXListener->SetEAX4ErrorCode(-2); result=-3; }
  break;
 case EAXSOURCE_EXCLUSIONPARAMETERS:
  if(data && size >= 8) { ((EAXEXCLUSIONPROPERTIES*)data)->lExclusion=m_Source.eax4BP.lExclusion;((EAXEXCLUSIONPROPERTIES*)data)->flExclusionLFRatio=m_Source.eax4BP.flExclusionLFRatio; *returned=8; }
  else { m_lpEAXListener->SetEAX4ErrorCode(-2); result=-3; }
  break;
 case 5:
  if(data && size >= 4) { memcpy(data,&m_Source.eax4BP.lDirect,4); *returned=4; }
  else { m_lpEAXListener->SetEAX4ErrorCode(-2); result=-3; }
  break;
 case 6:
  if(data && size >= 4) { memcpy(data,&m_Source.eax4BP.lDirectHF,4); *returned=4; }
  else { m_lpEAXListener->SetEAX4ErrorCode(-2); result=-3; }
  break;
 case 7:
  if(data && size >= 4) { memcpy(data,&m_Source.eax4BP.lRoom,4); *returned=4; }
  else { m_lpEAXListener->SetEAX4ErrorCode(-2); result=-3; }
  break;
 case 8:
  if(data && size >= 4) { memcpy(data,&m_Source.eax4BP.lRoomHF,4); *returned=4; }
  else { m_lpEAXListener->SetEAX4ErrorCode(-2); result=-3; }
  break;
 case 9:
  if(data && size >= 4) { memcpy(data,&m_Source.eax4BP.lObstruction,4); *returned=4; }
  else { m_lpEAXListener->SetEAX4ErrorCode(-2); result=-3; }
  break;
 case 10:
  if(data && size >= 4) { memcpy(data,&m_Source.eax4BP.flObstructionLFRatio,4); *returned=4; }
  else { m_lpEAXListener->SetEAX4ErrorCode(-2); result=-3; }
  break;
 case 11:
  if(data && size >= 4) { memcpy(data,&m_Source.eax4BP.lOcclusion,4); *returned=4; }
  else { m_lpEAXListener->SetEAX4ErrorCode(-2); result=-3; }
  break;
 case 12:
  if(data && size >= 4) { memcpy(data,&m_Source.eax4BP.flOcclusionLFRatio,4); *returned=4; }
  else { m_lpEAXListener->SetEAX4ErrorCode(-2); result=-3; }
  break;
 case 13:
  if(data && size >= 4) { memcpy(data,&m_Source.eax4BP.flOcclusionRoomRatio,4); *returned=4; }
  else { m_lpEAXListener->SetEAX4ErrorCode(-2); result=-3; }
  break;
 case 14:
  if(data && size >= 4) { memcpy(data,&m_Source.eax4BP.flOcclusionDirectRatio,4); *returned=4; }
  else { m_lpEAXListener->SetEAX4ErrorCode(-2); result=-3; }
  break;
 case 15:
  if(data && size >= 4) { memcpy(data,&m_Source.eax4BP.lExclusion,4); *returned=4; }
  else { m_lpEAXListener->SetEAX4ErrorCode(-2); result=-3; }
  break;
 case 16:
  if(data && size >= 4) { memcpy(data,&m_Source.eax4BP.flExclusionLFRatio,4); *returned=4; }
  else { m_lpEAXListener->SetEAX4ErrorCode(-2); result=-3; }
  break;
 case 17:
  if(data && size >= 4) { memcpy(data,&m_Source.eax4BP.lOutsideVolumeHF,4); *returned=4; }
  else { m_lpEAXListener->SetEAX4ErrorCode(-2); result=-3; }
  break;
 case 18:
  if(data && size >= 4) { memcpy(data,&m_Source.eax4BP.flDopplerFactor,4); *returned=4; }
  else { m_lpEAXListener->SetEAX4ErrorCode(-2); result=-3; }
  break;
 case 19:
  if(data && size >= 4) { memcpy(data,&m_Source.eax4BP.flRolloffFactor,4); *returned=4; }
  else { m_lpEAXListener->SetEAX4ErrorCode(-2); result=-3; }
  break;
 case 20:
  if(data && size >= 4) { memcpy(data,&m_Source.eax4BP.flRoomRolloffFactor,4); *returned=4; }
  else { m_lpEAXListener->SetEAX4ErrorCode(-2); result=-3; }
  break;
 case 21:
  if(data && size >= 4) { memcpy(data,&m_Source.eax4BP.flAirAbsorptionFactor,4); *returned=4; }
  else { m_lpEAXListener->SetEAX4ErrorCode(-2); result=-3; }
  break;
 case 22:
  if(data && size >= 4) { memcpy(data,&m_Source.eax4BP.ulFlags,4); *returned=4; }
  else { m_lpEAXListener->SetEAX4ErrorCode(-2); result=-3; }
  break;
 case EAXSOURCE_SENDPARAMETERS:
  if(data && size >= 24) {
   unsigned long count=size/24; if(count > 4) count=4;
   memcpy(data,&m_Source.eax4BPSend,count*24);
   *returned=count*24;
  } else { m_lpEAXListener->SetEAX4ErrorCode(-2); result=-3; }
  break;
 case EAXSOURCE_ALLSENDPARAMETERS:
  if(data && size >= 48) {
   unsigned long count=size/48; if(count > 4) count=4;
   EAXSOURCEALLSENDPROPERTIES* sends=(EAXSOURCEALLSENDPROPERTIES*)data;
   for(unsigned long i=0;i<count;++i) {
    switch(i) {
     case 0: sends[0].guidReceivingFXSlotID=EAXPROPERTYID_EAX40_FXSlot0; break;
     case 1: sends[1].guidReceivingFXSlotID=EAXPROPERTYID_EAX40_FXSlot1; break;
     case 2: sends[2].guidReceivingFXSlotID=EAXPROPERTYID_EAX40_FXSlot2; break;
     case 3: sends[3].guidReceivingFXSlotID=EAXPROPERTYID_EAX40_FXSlot3; break;
    }
    sends[i].lOcclusion=m_Source.eax4BPOcclusionSend[i].lOcclusion;sends[i].flOcclusionLFRatio=m_Source.eax4BPOcclusionSend[i].flOcclusionLFRatio;sends[i].flOcclusionRoomRatio=m_Source.eax4BPOcclusionSend[i].flOcclusionRoomRatio;sends[i].flOcclusionDirectRatio=m_Source.eax4BPOcclusionSend[i].flOcclusionDirectRatio;
    sends[i].lExclusion=m_Source.eax4BPExclusionSend[i].lExclusion;sends[i].flExclusionLFRatio=m_Source.eax4BPExclusionSend[i].flExclusionLFRatio;
    sends[i].lSend=m_Source.eax4BPSend[i].lSend; sends[i].lSendHF=m_Source.eax4BPSend[i].lSendHF;
   }
   *returned=count*48;
  } else { m_lpEAXListener->SetEAX4ErrorCode(-2); result=-3; }
  break;
 case EAXSOURCE_OCCLUSIONSENDPARAMETERS:
  if(data && size >= 32) {
   unsigned long count=size/32; if(count > 4) count=4;
   memcpy(data,&m_Source.eax4BPOcclusionSend,count*32);
   *returned=count*32;
  } else { m_lpEAXListener->SetEAX4ErrorCode(-2); result=-3; }
  break;
 case EAXSOURCE_EXCLUSIONSENDPARAMETERS:
  if(data && size >= 24) {
   unsigned long count=size/24; if(count > 4) count=4;
   memcpy(data,&m_Source.eax4BPExclusionSend,count*24);
   *returned=count*24;
  } else { m_lpEAXListener->SetEAX4ErrorCode(-2); result=-3; }
  break;
 case EAXSOURCE_ACTIVEFXSLOTID:
  if(data && size >= 16) {
   unsigned long count=size/16; if(count > 2) count=2;
   memcpy(data,&m_Source.eax4BPActiveFXSlotID,count*16);
   *returned=count*16;
  } else { m_lpEAXListener->SetEAX4ErrorCode(-2); result=-3; }
  break;
 default: return -3;
 }
 return result;
}