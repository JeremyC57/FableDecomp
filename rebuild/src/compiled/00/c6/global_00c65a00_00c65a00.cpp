// CEaxSource::CommitDeferredSettings() -- combine source and slot attenuation and commit deferred properties.
// Leaf: ?CommitDeferredSettings@CEaxSource@@QAEJXZ
#include "engine/CEaxSource.h"
#include <string.h>
#define min(a,b) (((a)<(b))?(a):(b))
#define max(a,b) (((a)>(b))?(a):(b))
long CEaxSource::CommitDeferredSettings()
{
 if(m_dirty&0x400000){
  CEaxContext* context=m_pContext;
  if(memcmp(&m_activeSlots.guidActiveFXSlots[0],&context->m_context.guidPrimaryFXSlotID,sizeof(GUID))==0 ||
     memcmp(&m_activeSlots.guidActiveFXSlots[0],&EAX_PrimaryFXSlotID,sizeof(GUID))==0 ||
     memcmp(&m_activeSlots.guidActiveFXSlots[1],&context->m_context.guidPrimaryFXSlotID,sizeof(GUID))==0 ||
     memcmp(&m_activeSlots.guidActiveFXSlots[1],&EAX_PrimaryFXSlotID,sizeof(GUID))==0)
   m_activeReverb=true;
  else m_activeReverb=false;
  if(m_activeReverb&&context->m_primarySlot!=-1&&(context->m_slots[context->m_primarySlot].ulFlags&1)){
   m_deferred.lRoom=max(m_current.lRoom+m_sends[context->m_primarySlot].lSend,-10000);
   m_deferred.lRoomHF=max(m_sends[context->m_primarySlot].lSendHF+m_current.lRoomHF,-10000);
   long occlusion=m_current.lOcclusion;
   if(occlusion==0){
    m_deferred.lOcclusion=m_occlusionSends[context->m_primarySlot].lOcclusion;
    m_deferred.flOcclusionLFRatio=m_occlusionSends[context->m_primarySlot].flOcclusionLFRatio;
    m_deferred.flOcclusionRoomRatio=m_occlusionSends[context->m_primarySlot].flOcclusionRoomRatio;
    m_deferred.flOcclusionDirectRatio=m_occlusionSends[context->m_primarySlot].flOcclusionDirectRatio;
   }else if(m_occlusionSends[context->m_primarySlot].lOcclusion==0){
    m_deferred.lOcclusion=occlusion;
    m_deferred.flOcclusionLFRatio=m_current.flOcclusionLFRatio;
    m_deferred.flOcclusionRoomRatio=m_current.flOcclusionRoomRatio;
    m_deferred.flOcclusionDirectRatio=m_current.flOcclusionDirectRatio;
   }else{
    long total=max(m_occlusionSends[context->m_primarySlot].lOcclusion+occlusion,-10000);
    m_deferred.lOcclusion=total;
    float direct=min(10.0f,(occlusion*m_current.flOcclusionDirectRatio+m_occlusionSends[context->m_primarySlot].lOcclusion*m_occlusionSends[context->m_primarySlot].flOcclusionDirectRatio)/total);
    m_deferred.flOcclusionDirectRatio=direct;
    float room=min(10.0f,(occlusion*m_current.flOcclusionRoomRatio+m_occlusionSends[context->m_primarySlot].lOcclusion*m_occlusionSends[context->m_primarySlot].flOcclusionRoomRatio)/total);
    m_deferred.flOcclusionRoomRatio=room;
    float currentDirect=max(m_current.flOcclusionLFRatio*m_current.flOcclusionDirectRatio,m_current.flOcclusionLFRatio+m_current.flOcclusionDirectRatio-1.0f);
    float sendDirect=max(m_occlusionSends[context->m_primarySlot].flOcclusionLFRatio*m_occlusionSends[context->m_primarySlot].flOcclusionDirectRatio,m_occlusionSends[context->m_primarySlot].flOcclusionLFRatio+m_occlusionSends[context->m_primarySlot].flOcclusionDirectRatio-1.0f);
    float combinedDirect=occlusion*currentDirect;
    combinedDirect+=m_occlusionSends[context->m_primarySlot].lOcclusion*sendDirect;
    combinedDirect/=total;
    float currentRoom=max(m_current.flOcclusionLFRatio*m_current.flOcclusionRoomRatio,m_current.flOcclusionLFRatio+m_current.flOcclusionRoomRatio-1.0f);
    const CEaxSource& source=*this;
    float sendRoom=(source.m_occlusionSends[context->m_primarySlot].flOcclusionRoomRatio*source.m_occlusionSends[context->m_primarySlot].flOcclusionLFRatio > source.m_occlusionSends[context->m_primarySlot].flOcclusionRoomRatio+source.m_occlusionSends[context->m_primarySlot].flOcclusionLFRatio-1.0f) ? m_occlusionSends[context->m_primarySlot].flOcclusionRoomRatio*m_occlusionSends[context->m_primarySlot].flOcclusionLFRatio : m_occlusionSends[context->m_primarySlot].flOcclusionRoomRatio+m_occlusionSends[context->m_primarySlot].flOcclusionLFRatio-1.0f;
    float combinedRoom=(occlusion*currentRoom+m_occlusionSends[context->m_primarySlot].lOcclusion*sendRoom)/total;
    if(direct!=0.0f&&room!=0.0f){
     m_deferred.flOcclusionLFRatio=min(1.0f,combinedDirect/direct);
     m_deferred.flOcclusionLFRatio=min(m_deferred.flOcclusionLFRatio,combinedDirect+1.0f-direct);
     m_deferred.flOcclusionLFRatio=min(m_deferred.flOcclusionLFRatio,combinedRoom/room);
     m_deferred.flOcclusionLFRatio=min(m_deferred.flOcclusionLFRatio,combinedRoom+1.0f-room);
     m_deferred.flOcclusionLFRatio=max(m_deferred.flOcclusionLFRatio,0.0f);
    }else m_deferred.flOcclusionLFRatio=0.0f;
   }
   long exclusion=m_current.lExclusion;
   if(exclusion==0){
    m_deferred.lExclusion=m_exclusionSends[context->m_primarySlot].lExclusion;
    m_deferred.flExclusionLFRatio=m_exclusionSends[context->m_primarySlot].flExclusionLFRatio;
   }else if(m_exclusionSends[context->m_primarySlot].lExclusion==0){
    m_deferred.lExclusion=exclusion;
    m_deferred.flExclusionLFRatio=m_current.flExclusionLFRatio;
   }else{
    long total=max(m_exclusionSends[context->m_primarySlot].lExclusion+exclusion,-10000);
    m_deferred.lExclusion=total;
    m_deferred.flExclusionLFRatio=min(1.0f,(exclusion*m_current.flExclusionLFRatio+m_exclusionSends[context->m_primarySlot].lExclusion*m_exclusionSends[context->m_primarySlot].flExclusionLFRatio)/total);
   }
  }else{
   m_deferred.lRoom=-10000;
   m_deferred.lRoomHF=m_current.lRoomHF;
   m_deferred.lOcclusion=m_current.lOcclusion;
   m_deferred.flOcclusionLFRatio=m_current.flOcclusionLFRatio;
   m_deferred.flOcclusionRoomRatio=m_current.flOcclusionRoomRatio;
   m_deferred.flOcclusionDirectRatio=m_current.flOcclusionDirectRatio;
   m_deferred.lExclusion=m_current.lExclusion;
   m_deferred.flExclusionLFRatio=m_current.flExclusionLFRatio;
  }
  m_dirty=0x200000;
 }
 if(m_dirty&0x200000){
  m_pContext->QueueEvent_00c5ffb0(1,EAXSOURCE_ALLPARAMETERS,&m_deferred,m_sourceID);
  m_dirty=0;
 }
 if(m_dirty&0x00000008){
  m_pContext->QueueEvent_00c5ffb0(1,5,&m_deferred.lDirect,m_sourceID);
  m_dirty&=~0x00000008;
 }
 if(m_dirty&0x00000010){
  m_pContext->QueueEvent_00c5ffb0(1,6,&m_deferred.lDirectHF,m_sourceID);
  m_dirty&=~0x00000010;
 }
 if(m_dirty&0x00000020){
  m_pContext->QueueEvent_00c5ffb0(1,7,&m_deferred.lRoom,m_sourceID);
  m_dirty&=~0x00000020;
 }
 if(m_dirty&0x00000040){
  m_pContext->QueueEvent_00c5ffb0(1,8,&m_deferred.lRoomHF,m_sourceID);
  m_dirty&=~0x00000040;
 }
 if(m_dirty&0x00000080){
  m_pContext->QueueEvent_00c5ffb0(1,9,&m_deferred.lObstruction,m_sourceID);
  m_dirty&=~0x00000080;
 }
 if(m_dirty&0x00000100){
  m_pContext->QueueEvent_00c5ffb0(1,10,&m_deferred.flObstructionLFRatio,m_sourceID);
  m_dirty&=~0x00000100;
 }
 if(m_dirty&0x00000200){
  m_pContext->QueueEvent_00c5ffb0(1,11,&m_deferred.lOcclusion,m_sourceID);
  m_dirty&=~0x00000200;
 }
 if(m_dirty&0x00000400){
  m_pContext->QueueEvent_00c5ffb0(1,12,&m_deferred.flOcclusionLFRatio,m_sourceID);
  m_dirty&=~0x00000400;
 }
 if(m_dirty&0x00000800){
  m_pContext->QueueEvent_00c5ffb0(1,13,&m_deferred.flOcclusionRoomRatio,m_sourceID);
  m_dirty&=~0x00000800;
 }
 if(m_dirty&0x00001000){
  m_pContext->QueueEvent_00c5ffb0(1,14,&m_deferred.flOcclusionDirectRatio,m_sourceID);
  m_dirty&=~0x00001000;
 }
 if(m_dirty&0x00002000){
  m_pContext->QueueEvent_00c5ffb0(1,15,&m_deferred.lExclusion,m_sourceID);
  m_dirty&=~0x00002000;
 }
 if(m_dirty&0x00004000){
  m_pContext->QueueEvent_00c5ffb0(1,16,&m_deferred.flExclusionLFRatio,m_sourceID);
  m_dirty&=~0x00004000;
 }
 if(m_dirty&0x00008000){
  m_pContext->QueueEvent_00c5ffb0(1,17,&m_deferred.lOutsideVolumeHF,m_sourceID);
  m_dirty&=~0x00008000;
 }
 if(m_dirty&0x00010000){
  m_pContext->QueueEvent_00c5ffb0(1,18,&m_deferred.flDopplerFactor,m_sourceID);
  m_dirty&=~0x00010000;
 }
 if(m_dirty&0x00020000){
  m_pContext->QueueEvent_00c5ffb0(1,19,&m_deferred.flRolloffFactor,m_sourceID);
  m_dirty&=~0x00020000;
 }
 if(m_dirty&0x00040000){
  m_pContext->QueueEvent_00c5ffb0(1,20,&m_deferred.flRoomRolloffFactor,m_sourceID);
  m_dirty&=~0x00040000;
 }
 if(m_dirty&0x00080000){
  m_pContext->QueueEvent_00c5ffb0(1,21,&m_deferred.flAirAbsorptionFactor,m_sourceID);
  m_dirty&=~0x00080000;
 }
 if(m_dirty&0x00100000){
  m_pContext->QueueEvent_00c5ffb0(1,22,&m_deferred.ulFlags,m_sourceID);
  m_dirty&=~0x00100000;
 }
 if(m_dirty&0x00000001){
  m_pContext->QueueEvent_00c5ffb0(1,2,&m_deferred.lObstruction,m_sourceID);
  m_dirty&=~0x00000001;
 }
 if(m_dirty&0x00000002){
  m_pContext->QueueEvent_00c5ffb0(1,3,&m_deferred.lOcclusion,m_sourceID);
  m_dirty&=~0x00000002;
 }
 if(m_dirty&0x00000004){
  m_pContext->QueueEvent_00c5ffb0(1,4,&m_deferred.lExclusion,m_sourceID);
  m_dirty&=~0x00000004;
 }
 return 0;
}