// 01216410 CEAXBuffer constructor and generated base cleanup standalone test
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <new>
#include <stddef.h>
#ifndef _MSC_VER
#define __fastcall
#define __cdecl
#define __stdcall
#define __int64 long long
#define __declspec(x)
#define FABLE_PORTABLE
#endif
#define FABLE_RETAIL_CCODA_HOST_ALLOCATION_PASS23
#define FABLE_RETAIL_PASS23_HOST_TEST_ABI
#define private public
#define protected public
// 01216410 CEAXBuffer constructor parent00c61610 and generated cleanup
// Leaf: ??0CEAXBuffer@@QAE@PAVCEAXListener@@JJW4EAXVERSION@@PAPAU_CHANGES@@@Z
// Flags: /EHsc
#define FABLE_RETAIL_CEAXBUFFER_CTOR_PASS23
#include "engine/CEAXBuffer.h"

CEAXBuffer::CEAXBuffer(CEAXListener* lpEAXListener,long BufferID,long Volume,EAXVERSION eaxVersion,_CHANGES** lplpChanges)
{
 m_EAXVersion=eaxVersion;
 m_dwFlags=0x754;
 m_dwFlags_Defer=0;
 m_lDirect=0;
 m_flDirectdB=0.0f;
 m_dwEAX3Changes=m_dwEAX3Changes_Defer=0;
 m_bEAX4Active=true;
 m_lpEAXListener=lpEAXListener;
 m_BufferID=BufferID;
 m_lpEAXListener->DeleteChangesList();
 m_lVolume=Volume;
 m_Buffer.dwInsideConeAngle=360;
 m_Buffer.dwOutsideConeAngle=360;
 m_Buffer.vPosition.x=0.0f;
 m_Buffer.vPosition.y=0.0f;
 m_Buffer.vPosition.z=0.0f;
 m_Buffer.vVelocity.x=0.0f;
 m_Buffer.vVelocity.y=0.0f;
 m_Buffer.vVelocity.z=0.0f;
 m_Buffer.vConeOrientation.x=0.0f;
 m_Buffer.vConeOrientation.y=0.0f;
 m_Buffer.vConeOrientation.z=1.0f;
 m_Buffer.lConeOutsideVolume=0;
 m_Buffer.flMinDistance=1.0f;
 m_Buffer.flMaxDistance=1.0e9f;
 m_Buffer.dwMode=0;
 m_Buffer_Defer=m_Buffer;
 m_Source.eax3BP.lDirect=0;
 m_Source.eax3BP.lDirectHF=0;
 m_Source.eax3BP.lRoom=0;
 m_Source.eax3BP.lRoomHF=0;
 m_Source.eax3BP.lObstruction=0;
 m_Source.eax3BP.flObstructionLFRatio=0.0f;
 m_Source.eax3BP.lOcclusion=0;
 m_Source.eax3BP.flOcclusionLFRatio=0.25f;
 m_Source.eax3BP.flOcclusionRoomRatio=1.5f;
 m_Source.eax3BP.flOcclusionDirectRatio=1.0f;
 m_Source.eax3BP.lExclusion=0;
 m_Source.eax3BP.flExclusionLFRatio=1.0f;
 m_Source.eax3BP.lOutsideVolumeHF=0;
 m_Source.eax3BP.flDopplerFactor=0.0f;
 m_Source.eax3BP.flRolloffFactor=0.0f;
 m_Source.eax3BP.flRoomRolloffFactor=0.0f;
 m_Source.eax3BP.flAirAbsorptionFactor=1.0f;
 m_Source.eax3BP.ulFlags=7;
 m_Dest.eax3BP=m_Source.eax3BP;
 m_Source.eax4BP.lDirect=0;
 m_Source.eax4BP.lDirectHF=0;
 m_Source.eax4BP.lRoom=0;
 m_Source.eax4BP.lRoomHF=0;
 m_Source.eax4BP.lObstruction=0;
 m_Source.eax4BP.flObstructionLFRatio=0.0f;
 m_Source.eax4BP.lOcclusion=0;
 m_Source.eax4BP.flOcclusionLFRatio=0.25f;
 m_Source.eax4BP.flOcclusionRoomRatio=1.5f;
 m_Source.eax4BP.flOcclusionDirectRatio=1.0f;
 m_Source.eax4BP.lExclusion=0;
 m_Source.eax4BP.flExclusionLFRatio=1.0f;
 m_Source.eax4BP.lOutsideVolumeHF=0;
 m_Source.eax4BP.flDopplerFactor=1.0f;
 m_Source.eax4BP.flRolloffFactor=0.0f;
 m_Source.eax4BP.flRoomRolloffFactor=0.0f;
 m_Source.eax4BP.flAirAbsorptionFactor=0.0f;
 m_Source.eax4BP.ulFlags=7;
 m_Source.eax4BPActiveFXSlotID[0]=EAX_NULL_GUID;
 m_Source.eax4BPActiveFXSlotID[1]=EAX_PrimaryFXSlotID;
 m_Source.eax4BPSend[0].guidReceivingFXSlotID=EAXPROPERTYID_EAX40_FXSlot0;
 m_Source.eax4BPSend[0].lSend=0;
 m_Source.eax4BPSend[0].lSendHF=0;
 m_Source.eax4BPSend[1].guidReceivingFXSlotID=EAXPROPERTYID_EAX40_FXSlot1;
 m_Source.eax4BPSend[1].lSend=0;
 m_Source.eax4BPSend[1].lSendHF=0;
 m_Source.eax4BPSend[2].guidReceivingFXSlotID=EAXPROPERTYID_EAX40_FXSlot2;
 m_Source.eax4BPSend[2].lSend=0;
 m_Source.eax4BPSend[2].lSendHF=0;
 m_Source.eax4BPSend[3].guidReceivingFXSlotID=EAXPROPERTYID_EAX40_FXSlot3;
 m_Source.eax4BPSend[3].lSend=0;
 m_Source.eax4BPSend[3].lSendHF=0;
 m_Source.eax4BPOcclusionSend[0].guidReceivingFXSlotID=EAXPROPERTYID_EAX40_FXSlot0;
 m_Source.eax4BPOcclusionSend[0].lOcclusion=0;
 m_Source.eax4BPOcclusionSend[0].flOcclusionLFRatio=0.25f;
 m_Source.eax4BPOcclusionSend[0].flOcclusionRoomRatio=1.5f;
 m_Source.eax4BPOcclusionSend[0].flOcclusionDirectRatio=1.0f;
 m_Source.eax4BPOcclusionSend[1]=m_Source.eax4BPOcclusionSend[0];
 m_Source.eax4BPOcclusionSend[1].guidReceivingFXSlotID=EAXPROPERTYID_EAX40_FXSlot1;
 m_Source.eax4BPOcclusionSend[2]=m_Source.eax4BPOcclusionSend[0];
 m_Source.eax4BPOcclusionSend[2].guidReceivingFXSlotID=EAXPROPERTYID_EAX40_FXSlot2;
 m_Source.eax4BPOcclusionSend[3]=m_Source.eax4BPOcclusionSend[0];
 m_Source.eax4BPOcclusionSend[3].guidReceivingFXSlotID=EAXPROPERTYID_EAX40_FXSlot3;
 m_Source.eax4BPExclusionSend[0].guidReceivingFXSlotID=EAXPROPERTYID_EAX40_FXSlot0;
 m_Source.eax4BPExclusionSend[0].lExclusion=0;
 m_Source.eax4BPExclusionSend[0].flExclusionLFRatio=1.0f;
 m_Source.eax4BPExclusionSend[1]=m_Source.eax4BPExclusionSend[0];
 m_Source.eax4BPExclusionSend[1].guidReceivingFXSlotID=EAXPROPERTYID_EAX40_FXSlot1;
 m_Source.eax4BPExclusionSend[2]=m_Source.eax4BPExclusionSend[0];
 m_Source.eax4BPExclusionSend[2].guidReceivingFXSlotID=EAXPROPERTYID_EAX40_FXSlot2;
 m_Source.eax4BPExclusionSend[3]=m_Source.eax4BPExclusionSend[0];
 m_Source.eax4BPExclusionSend[3].guidReceivingFXSlotID=EAXPROPERTYID_EAX40_FXSlot3;
 m_lpEAXListener->AddBuffer(this);
 *lplpChanges=m_lpEAXListener->lpChanges;
}

#undef private
#undef protected
extern "C" const GUID EAX_NULL_GUID={0x0UL,0x0,0x0,{0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0}};
extern "C" const GUID EAX_PrimaryFXSlotID={0xf317866dUL,0x924c,0x450c,{0x86,0x1b,0xe6,0xda,0xa2,0x5e,0x7c,0x20}};
extern "C" const GUID EAXPROPERTYID_EAX40_FXSlot0={0xc4d79f1eUL,0xf1ac,0x436b,{0xa8,0x1d,0xa7,0x38,0xe7,0x4,0x54,0x69}};
extern "C" const GUID EAXPROPERTYID_EAX40_FXSlot1={0x8c00e96UL,0x74be,0x4491,{0x93,0xaa,0xe8,0xad,0x35,0xa4,0x91,0x17}};
extern "C" const GUID EAXPROPERTYID_EAX40_FXSlot2={0x1d433b88UL,0xf0f6,0x4637,{0x91,0x9f,0x60,0xe7,0xe0,0x6b,0x5e,0xdd}};
extern "C" const GUID EAXPROPERTYID_EAX40_FXSlot3={0xefff08eaUL,0xc7d8,0x44ab,{0x93,0xad,0x6d,0xbd,0x5f,0x91,0x0,0x64}};
static CEAXBuffer* ExpectedBuffer;static CEAXListener* ExpectedListener;static CEAXBufferList* ExpectedNode;static _CHANGES* Queued;
static long ExpectedID,ExpectedVolume;static EAXVERSION ExpectedVersion;static int AddResult,Stage,ThrowPoint;
static unsigned Clears,Adds,BufferDtors,BaseDtors;static bool Active;
static void CheckGUID(const GUID&a,const GUID&b){assert(a.Data1==b.Data1&&a.Data2==b.Data2&&a.Data3==b.Data3);for(unsigned i=0;i<8;++i)assert(a.Data4[i]==b.Data4[i]);}
template<class P> static void CheckSource(const P&p,float doppler,float absorption){assert(p.lDirect==0&&p.lDirectHF==0&&p.lRoom==0&&p.lRoomHF==0&&p.lObstruction==0&&p.flObstructionLFRatio==0&&p.lOcclusion==0&&p.flOcclusionLFRatio==.25f&&p.flOcclusionRoomRatio==1.5f&&p.flOcclusionDirectRatio==1&&p.lExclusion==0&&p.flExclusionLFRatio==1&&p.lOutsideVolumeHF==0&&p.flDopplerFactor==doppler&&p.flRolloffFactor==0&&p.flRoomRolloffFactor==0&&p.flAirAbsorptionFactor==absorption&&p.ulFlags==7);}
static void CheckComplete(const CEAXBuffer&b){
 assert(b.m_dwEAX3Changes==0&&b.m_dwEAX3Changes_Defer==0&&b.m_bEAX4Active&&b.m_EAXVersion==ExpectedVersion&&b.m_lpEAXListener==ExpectedListener&&b.m_dwFlags==0x754&&b.m_dwFlags_Defer==0&&b.m_BufferID==ExpectedID&&b.m_lVolume==ExpectedVolume&&b.m_lDirect==0&&b.m_flDirectdB==0);
 CheckSource(b.m_Source.eax4BP,1,0);CheckSource(b.m_Source.eax3BP,0,1);CheckSource(b.m_Dest.eax3BP,0,1);CheckGUID(b.m_Source.eax4BPActiveFXSlotID[0],EAX_NULL_GUID);CheckGUID(b.m_Source.eax4BPActiveFXSlotID[1],EAX_PrimaryFXSlotID);
 const GUID*guids[4]={&EAXPROPERTYID_EAX40_FXSlot0,&EAXPROPERTYID_EAX40_FXSlot1,&EAXPROPERTYID_EAX40_FXSlot2,&EAXPROPERTYID_EAX40_FXSlot3};
 for(unsigned i=0;i<4;++i){const EAXSOURCESENDPROPERTIES&s=b.m_Source.eax4BPSend[i];const EAXSOURCEOCCLUSIONSENDPROPERTIES&o=b.m_Source.eax4BPOcclusionSend[i];const EAXSOURCEEXCLUSIONSENDPROPERTIES&e=b.m_Source.eax4BPExclusionSend[i];CheckGUID(s.guidReceivingFXSlotID,*guids[i]);assert(s.lSend==0&&s.lSendHF==0);CheckGUID(o.guidReceivingFXSlotID,*guids[i]);assert(o.lOcclusion==0&&o.flOcclusionLFRatio==.25f&&o.flOcclusionRoomRatio==1.5f&&o.flOcclusionDirectRatio==1);CheckGUID(e.guidReceivingFXSlotID,*guids[i]);assert(e.lExclusion==0&&e.flExclusionLFRatio==1);}
 const BUFFER&p=b.m_Buffer;assert(p.dwInsideConeAngle==360&&p.dwOutsideConeAngle==360&&p.vPosition.x==0&&p.vPosition.y==0&&p.vPosition.z==0&&p.vVelocity.x==0&&p.vVelocity.y==0&&p.vVelocity.z==0&&p.vConeOrientation.x==0&&p.vConeOrientation.y==0&&p.vConeOrientation.z==1&&p.lConeOutsideVolume==0&&p.flMinDistance==1&&p.flMaxDistance==1.0e9f&&p.dwMode==0);assert(memcmp(&b.m_Buffer,&b.m_Buffer_Defer,sizeof(BUFFER))==0);
}
CCodaObjectBase::~CCodaObjectBase(){if((const void*)this==(const void*)ExpectedBuffer)++BaseDtors;}
CEAXListener::~CEAXListener(){assert(!m_lpBufferList);}
CEAXBufferList::CEAXBufferList():lpNext(0),lpEAXBuffer(0){}CEAXBufferList::~CEAXBufferList(){assert(!lpNext&&!lpEAXBuffer);}
CEAXBuffer::~CEAXBuffer(){assert(Active&&this==ExpectedBuffer&&Stage==2&&ExpectedNode->lpEAXBuffer==this&&ExpectedListener->m_lpBufferList==ExpectedNode);++BufferDtors;ExpectedNode->lpEAXBuffer=0;ExpectedListener->m_lpBufferList=0;}
void CodaFree(void*){abort();}void* CodaMalloc(unsigned int){abort();return 0;}
void CEAXListener::DeleteChangesList(){assert(Active&&this==ExpectedListener&&Stage==0);++Clears;Stage=1;assert(ExpectedBuffer->m_lpEAXListener==this&&ExpectedBuffer->m_EAXVersion==ExpectedVersion&&ExpectedBuffer->m_BufferID==ExpectedID&&ExpectedBuffer->m_dwFlags==0x754&&ExpectedBuffer->m_dwFlags_Defer==0&&ExpectedBuffer->m_dwEAX3Changes==0&&ExpectedBuffer->m_dwEAX3Changes_Defer==0&&ExpectedBuffer->m_bEAX4Active);lpChanges=0;
#ifndef _MSC_VER
 if(ThrowPoint==1)throw 17;
#endif
}
int CEAXListener::AddBuffer(CEAXBuffer*p){assert(Active&&this==ExpectedListener&&p==ExpectedBuffer&&Stage==1&&lpChanges==0&&!m_lpBufferList);CheckComplete(*p);++Adds;Stage=2;
#ifndef _MSC_VER
 if(ThrowPoint==2)throw 17;
#endif
 ExpectedNode->lpEAXBuffer=p;m_lpBufferList=ExpectedNode;lpChanges=Queued;return AddResult;}
static void Allow(unsigned char*allowed,const void*object,const void*field,unsigned n){unsigned off=(unsigned)((const unsigned char*)field-(const unsigned char*)object);memset(allowed+off,1,n);}
int main(){
 unsigned cases=0,bufferOffset=0,deferOffset=0;const long ids[]={-1,0,1,2147483647L,-2147483647L};const long volumes[]={-10000,-1,0,1000,2147483647L,-2147483647L};const int results[]={0,-5,7};
 {
 CEAXListener listener;CEAXBufferList node;_CHANGES e0,e1;ExpectedListener=&listener;ExpectedNode=&node;listener.m_lpBufferList=0;listener.lpChanges=0;ThrowPoint=0;
 for(unsigned id=0;id<5;++id)for(unsigned volume=0;volume<6;++volume)for(unsigned version=0;version<6;++version)for(unsigned result=0;result<3;++result)for(unsigned queue=0;queue<3;++queue)for(unsigned seed=0;seed<3;++seed){
  union Storage{double alignment;unsigned char bytes[sizeof(CEAXBuffer)];}storage;unsigned char before[sizeof(CEAXBuffer)],allowed[sizeof(CEAXBuffer)];memset(storage.bytes,(int)(0xa0+seed),sizeof(storage.bytes));if(cases){memset(storage.bytes+bufferOffset,(int)(0x40+seed),sizeof(BUFFER));memset(storage.bytes+deferOffset,(int)(0x70+seed),sizeof(BUFFER));}memcpy(before,storage.bytes,sizeof(before));memset(allowed,0,sizeof(allowed));ExpectedID=ids[id];ExpectedVolume=volumes[volume];ExpectedVersion=(EAXVERSION)version;AddResult=results[result];Queued=queue==0?0:queue==1?&e0:&e1;listener.lpChanges=queue==0?&e1:&e0;_CHANGES*out=&e1;ExpectedBuffer=(CEAXBuffer*)(void*)storage.bytes;Stage=0;Clears=Adds=BufferDtors=BaseDtors=0;Active=true;
  CEAXBuffer*p=::new((void*)storage.bytes)CEAXBuffer(&listener,ExpectedID,ExpectedVolume,ExpectedVersion,&out);assert(p==ExpectedBuffer&&Stage==2&&Clears==1&&Adds==1&&BufferDtors==0&&BaseDtors==0&&out==Queued&&listener.lpChanges==Queued&&node.lpEAXBuffer==p);CheckComplete(*p);if(!cases){bufferOffset=(unsigned)((unsigned char*)&p->m_Buffer-(unsigned char*)p);deferOffset=(unsigned)((unsigned char*)&p->m_Buffer_Defer-(unsigned char*)p);
#ifdef _MSC_VER
 assert(sizeof(CEAXBuffer)==0x3c0&&bufferOffset==0x250&&deferOffset==0x2dc);assert((unsigned char*)&p->m_Source-(unsigned char*)p==0x18&&(unsigned char*)&p->m_Dest-(unsigned char*)p==0x208);
#endif
 }
  // Permit exactly the original constructor's initialized fields; preserve all other bytes, including source-slot/tail and buffer conversion cache.
  Allow(allowed,p,p,sizeof(void*));
#define FIELD(n) Allow(allowed,p,&p->n,sizeof(p->n))
  FIELD(m_EAXVersion);FIELD(m_dwFlags);FIELD(m_dwFlags_Defer);FIELD(m_lDirect);FIELD(m_flDirectdB);FIELD(m_dwEAX3Changes);FIELD(m_dwEAX3Changes_Defer);FIELD(m_bEAX4Active);FIELD(m_lpEAXListener);FIELD(m_BufferID);FIELD(m_lVolume);FIELD(m_Source);FIELD(m_Dest);FIELD(m_Buffer.vPosition);FIELD(m_Buffer.vVelocity);FIELD(m_Buffer.dwInsideConeAngle);FIELD(m_Buffer.dwOutsideConeAngle);FIELD(m_Buffer.vConeOrientation);FIELD(m_Buffer.lConeOutsideVolume);FIELD(m_Buffer.flMinDistance);FIELD(m_Buffer.flMaxDistance);FIELD(m_Buffer.dwMode);FIELD(m_Buffer_Defer);
#undef FIELD
  for(unsigned n=0;n<sizeof(CEAXBuffer);++n)if(!allowed[n])assert(storage.bytes[n]==before[n]);p->~CEAXBuffer();assert(BufferDtors==1&&BaseDtors==1&&!node.lpEAXBuffer&&!listener.m_lpBufferList);Active=false;ExpectedBuffer=0;++cases;
 }
 assert(cases==4860);
#ifndef _MSC_VER
 for(int failure=1;failure<=2;++failure){union Storage{double alignment;unsigned char bytes[sizeof(CEAXBuffer)];}storage;memset(storage.bytes,0,sizeof(storage.bytes));ExpectedBuffer=(CEAXBuffer*)(void*)storage.bytes;ExpectedID=1;ExpectedVolume=-100;ExpectedVersion=EAX4;listener.lpChanges=&e0;listener.m_lpBufferList=0;node.lpEAXBuffer=0;Queued=&e1;AddResult=0;ThrowPoint=failure;Stage=0;Clears=Adds=BufferDtors=BaseDtors=0;_CHANGES*out=&e0;Active=true;bool caught=false;try{::new((void*)storage.bytes)CEAXBuffer(&listener,ExpectedID,ExpectedVolume,ExpectedVersion,&out);}catch(int x){assert(x==17);caught=true;}assert(caught&&Clears==1&&Adds==(failure==2?1U:0U)&&BufferDtors==0&&BaseDtors==1&&out==&e0&&!listener.m_lpBufferList&&!node.lpEAXBuffer);Active=false;ExpectedBuffer=0;}
#endif
 }puts("01216410_TEST PASS");return 0;
}