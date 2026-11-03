// int CEAXBuffer::CommitEAX3() -- combine source and slot attenuation and commit deferred properties.
// Leaf: ?CommitEAX3@CEAXBuffer@@QAEHXZ
// Name (owner decision 2026-11-03 D3): GameCODA 2.10 EAXBuffer.obj Set3DBuffer tail-call relocation names public int
//   CEAXBuffer::CommitEAX3(); retail Set3DBuffer 0x00C60FF0 reaches 0x00C65A00 on the same branch (Codex pass 12 SDK_COMPARISON).
#include "engine/CEAXBuffer.h"
#include <string.h>
#define min(a,b) (((a)<(b))?(a):(b))
#define max(a,b) (((a)>(b))?(a):(b))
int CEAXBuffer::CommitEAX3()
{
 if(m_dwEAX3Changes&0x400000){
  CEAXListener* context=m_lpEAXListener;
  if(memcmp(&m_Source.eax4BPActiveFXSlotID[0],&context->m_Source.eax4Context.guidPrimaryFXSlotID,sizeof(GUID))==0 ||
     memcmp(&m_Source.eax4BPActiveFXSlotID[0],&EAX_PrimaryFXSlotID,sizeof(GUID))==0 ||
     memcmp(&m_Source.eax4BPActiveFXSlotID[1],&context->m_Source.eax4Context.guidPrimaryFXSlotID,sizeof(GUID))==0 ||
     memcmp(&m_Source.eax4BPActiveFXSlotID[1],&EAX_PrimaryFXSlotID,sizeof(GUID))==0)
   m_bEAX4Active=true;
  else m_bEAX4Active=false;
  if(m_bEAX4Active&&context->m_ListenerSlotID!=-1&&(context->m_Source.eax4FXSlot[context->m_ListenerSlotID].ulFlags&1)){
   m_Dest.eax3BP.lRoom=max(m_Source.eax4BP.lRoom+m_Source.eax4BPSend[context->m_ListenerSlotID].lSend,-10000);
   m_Dest.eax3BP.lRoomHF=max(m_Source.eax4BPSend[context->m_ListenerSlotID].lSendHF+m_Source.eax4BP.lRoomHF,-10000);
   long occlusion=m_Source.eax4BP.lOcclusion;
   if(occlusion==0){
    m_Dest.eax3BP.lOcclusion=m_Source.eax4BPOcclusionSend[context->m_ListenerSlotID].lOcclusion;
    m_Dest.eax3BP.flOcclusionLFRatio=m_Source.eax4BPOcclusionSend[context->m_ListenerSlotID].flOcclusionLFRatio;
    m_Dest.eax3BP.flOcclusionRoomRatio=m_Source.eax4BPOcclusionSend[context->m_ListenerSlotID].flOcclusionRoomRatio;
    m_Dest.eax3BP.flOcclusionDirectRatio=m_Source.eax4BPOcclusionSend[context->m_ListenerSlotID].flOcclusionDirectRatio;
   }else if(m_Source.eax4BPOcclusionSend[context->m_ListenerSlotID].lOcclusion==0){
    m_Dest.eax3BP.lOcclusion=occlusion;
    m_Dest.eax3BP.flOcclusionLFRatio=m_Source.eax4BP.flOcclusionLFRatio;
    m_Dest.eax3BP.flOcclusionRoomRatio=m_Source.eax4BP.flOcclusionRoomRatio;
    m_Dest.eax3BP.flOcclusionDirectRatio=m_Source.eax4BP.flOcclusionDirectRatio;
   }else{
    long total=max(m_Source.eax4BPOcclusionSend[context->m_ListenerSlotID].lOcclusion+occlusion,-10000);
    m_Dest.eax3BP.lOcclusion=total;
    float direct=min(10.0f,(occlusion*m_Source.eax4BP.flOcclusionDirectRatio+m_Source.eax4BPOcclusionSend[context->m_ListenerSlotID].lOcclusion*m_Source.eax4BPOcclusionSend[context->m_ListenerSlotID].flOcclusionDirectRatio)/total);
    m_Dest.eax3BP.flOcclusionDirectRatio=direct;
    float room=min(10.0f,(occlusion*m_Source.eax4BP.flOcclusionRoomRatio+m_Source.eax4BPOcclusionSend[context->m_ListenerSlotID].lOcclusion*m_Source.eax4BPOcclusionSend[context->m_ListenerSlotID].flOcclusionRoomRatio)/total);
    m_Dest.eax3BP.flOcclusionRoomRatio=room;
    float currentDirect=max(m_Source.eax4BP.flOcclusionLFRatio*m_Source.eax4BP.flOcclusionDirectRatio,m_Source.eax4BP.flOcclusionLFRatio+m_Source.eax4BP.flOcclusionDirectRatio-1.0f);
    float sendDirect=max(m_Source.eax4BPOcclusionSend[context->m_ListenerSlotID].flOcclusionLFRatio*m_Source.eax4BPOcclusionSend[context->m_ListenerSlotID].flOcclusionDirectRatio,m_Source.eax4BPOcclusionSend[context->m_ListenerSlotID].flOcclusionLFRatio+m_Source.eax4BPOcclusionSend[context->m_ListenerSlotID].flOcclusionDirectRatio-1.0f);
    float combinedDirect=occlusion*currentDirect;
    combinedDirect+=m_Source.eax4BPOcclusionSend[context->m_ListenerSlotID].lOcclusion*sendDirect;
    combinedDirect/=total;
    float currentRoom=max(m_Source.eax4BP.flOcclusionLFRatio*m_Source.eax4BP.flOcclusionRoomRatio,m_Source.eax4BP.flOcclusionLFRatio+m_Source.eax4BP.flOcclusionRoomRatio-1.0f);
    const CEAXBuffer& source=*this;
    float sendRoom=(source.m_Source.eax4BPOcclusionSend[context->m_ListenerSlotID].flOcclusionRoomRatio*source.m_Source.eax4BPOcclusionSend[context->m_ListenerSlotID].flOcclusionLFRatio > source.m_Source.eax4BPOcclusionSend[context->m_ListenerSlotID].flOcclusionRoomRatio+source.m_Source.eax4BPOcclusionSend[context->m_ListenerSlotID].flOcclusionLFRatio-1.0f) ? m_Source.eax4BPOcclusionSend[context->m_ListenerSlotID].flOcclusionRoomRatio*m_Source.eax4BPOcclusionSend[context->m_ListenerSlotID].flOcclusionLFRatio : m_Source.eax4BPOcclusionSend[context->m_ListenerSlotID].flOcclusionRoomRatio+m_Source.eax4BPOcclusionSend[context->m_ListenerSlotID].flOcclusionLFRatio-1.0f;
    float combinedRoom=(occlusion*currentRoom+m_Source.eax4BPOcclusionSend[context->m_ListenerSlotID].lOcclusion*sendRoom)/total;
    if(direct!=0.0f&&room!=0.0f){
     m_Dest.eax3BP.flOcclusionLFRatio=min(1.0f,combinedDirect/direct);
     m_Dest.eax3BP.flOcclusionLFRatio=min(m_Dest.eax3BP.flOcclusionLFRatio,combinedDirect+1.0f-direct);
     m_Dest.eax3BP.flOcclusionLFRatio=min(m_Dest.eax3BP.flOcclusionLFRatio,combinedRoom/room);
     m_Dest.eax3BP.flOcclusionLFRatio=min(m_Dest.eax3BP.flOcclusionLFRatio,combinedRoom+1.0f-room);
     m_Dest.eax3BP.flOcclusionLFRatio=max(m_Dest.eax3BP.flOcclusionLFRatio,0.0f);
    }else m_Dest.eax3BP.flOcclusionLFRatio=0.0f;
   }
   long exclusion=m_Source.eax4BP.lExclusion;
   if(exclusion==0){
    m_Dest.eax3BP.lExclusion=m_Source.eax4BPExclusionSend[context->m_ListenerSlotID].lExclusion;
    m_Dest.eax3BP.flExclusionLFRatio=m_Source.eax4BPExclusionSend[context->m_ListenerSlotID].flExclusionLFRatio;
   }else if(m_Source.eax4BPExclusionSend[context->m_ListenerSlotID].lExclusion==0){
    m_Dest.eax3BP.lExclusion=exclusion;
    m_Dest.eax3BP.flExclusionLFRatio=m_Source.eax4BP.flExclusionLFRatio;
   }else{
    long total=max(m_Source.eax4BPExclusionSend[context->m_ListenerSlotID].lExclusion+exclusion,-10000);
    m_Dest.eax3BP.lExclusion=total;
    m_Dest.eax3BP.flExclusionLFRatio=min(1.0f,(exclusion*m_Source.eax4BP.flExclusionLFRatio+m_Source.eax4BPExclusionSend[context->m_ListenerSlotID].lExclusion*m_Source.eax4BPExclusionSend[context->m_ListenerSlotID].flExclusionLFRatio)/total);
   }
  }else{
   m_Dest.eax3BP.lRoom=-10000;
   m_Dest.eax3BP.lRoomHF=m_Source.eax4BP.lRoomHF;
   m_Dest.eax3BP.lOcclusion=m_Source.eax4BP.lOcclusion;
   m_Dest.eax3BP.flOcclusionLFRatio=m_Source.eax4BP.flOcclusionLFRatio;
   m_Dest.eax3BP.flOcclusionRoomRatio=m_Source.eax4BP.flOcclusionRoomRatio;
   m_Dest.eax3BP.flOcclusionDirectRatio=m_Source.eax4BP.flOcclusionDirectRatio;
   m_Dest.eax3BP.lExclusion=m_Source.eax4BP.lExclusion;
   m_Dest.eax3BP.flExclusionLFRatio=m_Source.eax4BP.flExclusionLFRatio;
  }
  m_dwEAX3Changes=0x200000;
 }
 if(m_dwEAX3Changes&0x200000){
  m_lpEAXListener->AddToChangesList(EAX3BUFFER,EAXSOURCE_ALLPARAMETERS,&m_Dest.eax3BP,m_BufferID);
  m_dwEAX3Changes=0;
 }
 if(m_dwEAX3Changes&0x00000008){
  m_lpEAXListener->AddToChangesList(EAX3BUFFER,5,&m_Dest.eax3BP.lDirect,m_BufferID);
  m_dwEAX3Changes&=~0x00000008;
 }
 if(m_dwEAX3Changes&0x00000010){
  m_lpEAXListener->AddToChangesList(EAX3BUFFER,6,&m_Dest.eax3BP.lDirectHF,m_BufferID);
  m_dwEAX3Changes&=~0x00000010;
 }
 if(m_dwEAX3Changes&0x00000020){
  m_lpEAXListener->AddToChangesList(EAX3BUFFER,7,&m_Dest.eax3BP.lRoom,m_BufferID);
  m_dwEAX3Changes&=~0x00000020;
 }
 if(m_dwEAX3Changes&0x00000040){
  m_lpEAXListener->AddToChangesList(EAX3BUFFER,8,&m_Dest.eax3BP.lRoomHF,m_BufferID);
  m_dwEAX3Changes&=~0x00000040;
 }
 if(m_dwEAX3Changes&0x00000080){
  m_lpEAXListener->AddToChangesList(EAX3BUFFER,9,&m_Dest.eax3BP.lObstruction,m_BufferID);
  m_dwEAX3Changes&=~0x00000080;
 }
 if(m_dwEAX3Changes&0x00000100){
  m_lpEAXListener->AddToChangesList(EAX3BUFFER,10,&m_Dest.eax3BP.flObstructionLFRatio,m_BufferID);
  m_dwEAX3Changes&=~0x00000100;
 }
 if(m_dwEAX3Changes&0x00000200){
  m_lpEAXListener->AddToChangesList(EAX3BUFFER,11,&m_Dest.eax3BP.lOcclusion,m_BufferID);
  m_dwEAX3Changes&=~0x00000200;
 }
 if(m_dwEAX3Changes&0x00000400){
  m_lpEAXListener->AddToChangesList(EAX3BUFFER,12,&m_Dest.eax3BP.flOcclusionLFRatio,m_BufferID);
  m_dwEAX3Changes&=~0x00000400;
 }
 if(m_dwEAX3Changes&0x00000800){
  m_lpEAXListener->AddToChangesList(EAX3BUFFER,13,&m_Dest.eax3BP.flOcclusionRoomRatio,m_BufferID);
  m_dwEAX3Changes&=~0x00000800;
 }
 if(m_dwEAX3Changes&0x00001000){
  m_lpEAXListener->AddToChangesList(EAX3BUFFER,14,&m_Dest.eax3BP.flOcclusionDirectRatio,m_BufferID);
  m_dwEAX3Changes&=~0x00001000;
 }
 if(m_dwEAX3Changes&0x00002000){
  m_lpEAXListener->AddToChangesList(EAX3BUFFER,15,&m_Dest.eax3BP.lExclusion,m_BufferID);
  m_dwEAX3Changes&=~0x00002000;
 }
 if(m_dwEAX3Changes&0x00004000){
  m_lpEAXListener->AddToChangesList(EAX3BUFFER,16,&m_Dest.eax3BP.flExclusionLFRatio,m_BufferID);
  m_dwEAX3Changes&=~0x00004000;
 }
 if(m_dwEAX3Changes&0x00008000){
  m_lpEAXListener->AddToChangesList(EAX3BUFFER,17,&m_Dest.eax3BP.lOutsideVolumeHF,m_BufferID);
  m_dwEAX3Changes&=~0x00008000;
 }
 if(m_dwEAX3Changes&0x00010000){
  m_lpEAXListener->AddToChangesList(EAX3BUFFER,18,&m_Dest.eax3BP.flDopplerFactor,m_BufferID);
  m_dwEAX3Changes&=~0x00010000;
 }
 if(m_dwEAX3Changes&0x00020000){
  m_lpEAXListener->AddToChangesList(EAX3BUFFER,19,&m_Dest.eax3BP.flRolloffFactor,m_BufferID);
  m_dwEAX3Changes&=~0x00020000;
 }
 if(m_dwEAX3Changes&0x00040000){
  m_lpEAXListener->AddToChangesList(EAX3BUFFER,20,&m_Dest.eax3BP.flRoomRolloffFactor,m_BufferID);
  m_dwEAX3Changes&=~0x00040000;
 }
 if(m_dwEAX3Changes&0x00080000){
  m_lpEAXListener->AddToChangesList(EAX3BUFFER,21,&m_Dest.eax3BP.flAirAbsorptionFactor,m_BufferID);
  m_dwEAX3Changes&=~0x00080000;
 }
 if(m_dwEAX3Changes&0x00100000){
  m_lpEAXListener->AddToChangesList(EAX3BUFFER,22,&m_Dest.eax3BP.ulFlags,m_BufferID);
  m_dwEAX3Changes&=~0x00100000;
 }
 if(m_dwEAX3Changes&0x00000001){
  m_lpEAXListener->AddToChangesList(EAX3BUFFER,2,&m_Dest.eax3BP.lObstruction,m_BufferID);
  m_dwEAX3Changes&=~0x00000001;
 }
 if(m_dwEAX3Changes&0x00000002){
  m_lpEAXListener->AddToChangesList(EAX3BUFFER,3,&m_Dest.eax3BP.lOcclusion,m_BufferID);
  m_dwEAX3Changes&=~0x00000002;
 }
 if(m_dwEAX3Changes&0x00000004){
  m_lpEAXListener->AddToChangesList(EAX3BUFFER,4,&m_Dest.eax3BP.lExclusion,m_BufferID);
  m_dwEAX3Changes&=~0x00000004;
 }
 return 0;
}