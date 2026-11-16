// 012117a5 CSoundEventManager::PlayStreamed parent00c12e50 real-object behavior test
// Flags: none
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>
#include <new>
#ifndef _MSC_VER
#define __fastcall
#define __cdecl
#define __stdcall
#define __declspec(x)
#define __int64 long long
#define FABLE_PORTABLE
#endif
#define FABLE_RETAIL_PASS23_HOST_TEST_ABI
// 012117a5 CSoundEventManager::PlayStreamed parent00c12e50 and generated cleanup
// Leaf: ?PlayStreamed@CSoundEventManager@@UAEJPAVCStreamSoundBank@@JKPBUInput@@@Z
// Flags: /EHsc
#define FABLE_RETAIL_CPKVOICEPARAMS_POSITION_PASS23
#define FABLE_RETAIL_STREAMED_TYPED_MANAGER_PASS23
#define FABLE_RETAIL_CSOUNDEVENTMANAGER_STREAMED_PASS23
#define FABLE_RETAIL_CSOUNDEVENTGROUP
#define FABLE_RETAIL_LIBPK_STREAM_BUFFER_PASS22
#include "libpk/CSoundEventManager.h"
#include "libpk/CPooledResource.h"
#include "libpk/CSoundEventGroup.h"
#include "libpk/PKCProtectionInstance.h"
extern PKCCriticalSection g_lock_0143bb14;

long CSoundEventManager::PlayStreamed(CStreamSoundBank* bank, long position, unsigned long size, const Input* settings)
{
    PKCProtectionInstance lock(&g_lock_0143bb14);
    if (bank->m_pStream == 0)
        return -1;
    void* sample = bank->m_samples.FindByKey(&position);
    if (sample) {
        void* driver = bank->m_drivers.FindByKey(&sample);
        if (driver) {
            CPKVoiceParams params;
            params.Load(settings);
            CSoundEvent* entry = PlayDriver(driver, 0, &params, 0);
            if (entry)
                return entry->Index;
            return -1;
        }
    }
    long padding = bank->m_padding;
    unsigned long offset = GetStreamBlockOffset(bank->m_pStream, position);
    long tail = GetStreamBlockTail(bank->m_pStream, position, size);
    long handle = m_events.AllocateHandle(this, tail + offset + size, padding);
    if (handle < 0)
        return -1;
    m_events.Pool.Lock(handle);
    CPooledResource* request = new CPooledResource;
    long id = m_nextId++;
    request->m_id = id;
    request->m_offset = offset;
    request->m_tail = tail;
    request->m_handle = handle;
    request->m_flag = 0;
    request->m_pBank = bank;
    request->m_size = size;
    request->m_key = position;
    request->m_padding = padding;
    request->m_params.Load(settings);
    request->Activate(this);
    if (!bank->m_pStream->Read(request->GetFirstBlock(), request->GetBlockCount(), offset + size,
                               m_events.Pool.GetPointerFromHandle(handle), request, StreamOwner)) {
        m_events.Pool.Unlock(handle);
        request->Release(this);
        delete request;
        return -1;
    }
    return id;
}




void CSoundEventManager::AddToWorld(void*) { abort(); }
void CSoundEventManager::SetMember888(long) { abort(); }
void CSoundEventManager::SetWorldMode(bool) { abort(); }
void CSoundEventManager::SetMember7cc(long) { abort(); }
void* CSoundEventManager::GetWorldSelection() { abort(); }
void* CSoundEventManager::FindInWorld(void*) { abort(); }
void CSoundEventManager::SetMember884(unsigned char) { abort(); }
void CSoundEventManager::SetWorldGain(float) { abort(); }
float CSoundEventManager::GetWorldGain() { abort(); }
long CSoundEventManager::SetWorldEnabled(void*) { abort(); }
int CSoundEventManager::IsWorldRunning() { abort(); }
long CSoundEventManager::SetWorldState(void*) { abort(); }
void CSoundEventManager::SetListenerFrame(const CSoundPosition*,const CSoundPosition*,const CSoundPosition*) { abort(); }
void CSoundEventManager::SetListenerPosition(const CSoundPosition*) { abort(); }
int CStreamBankLookup::IsEqual(const void*,const void*)const { abort(); }
int CStreamBankLookup::IsLess(const void*,const void*)const { abort(); }
int CStreamBankLookup::IsKeyEqual(const void*,const void*)const { abort(); }
int CStreamBankLookup::IsKeyLess(const void*,const void*)const { abort(); }
void CStreamBankLookup::TransferAllFrom(CStreamBankLookup*) { abort(); }
int CSoundEventList::IsEqual(const void*,const void*)const { abort(); }
int CSoundEventList::IsLess(const void*,const void*)const { abort(); }
int CSoundEventList::IsKeyEqual(const void*,const void*)const { abort(); }
int CSoundEventList::IsKeyLess(const void*,const void*)const { abort(); }
void CSoundEventList::TransferAllFrom(CSoundEventList*) { abort(); }
CSoundEvent* CSoundEventList::FindByKey(const CSampleRef*) { abort(); }
int CSoundEventSet::IsEqual(const void*,const void*)const { abort(); }
int CSoundEventSet::IsLess(const void*,const void*)const { abort(); }
int CSoundEventSet::IsKeyEqual(const void*,const void*)const { abort(); }
int CSoundEventSet::IsKeyLess(const void*,const void*)const { abort(); }
void CHandleBasedMemoryPool_012b4098::MoveResource(void* resource, long handle, void* from, void* to, unsigned long size) { abort(); }
int CHandleBasedMemoryPool_012b4098::CanMoveResource(void* resource, long handle) { abort(); }
void CHandleBasedMemoryPool_012b4098::ValidateResource(void* resource) { abort(); }
void CPooledResource::ResetLevel(CSoundEventManager*) { abort(); }
void CPooledResource::SetUserGain(CSoundEventManager*,float) { abort(); }
void CPooledResource::SetPlaybackParameter(CSoundEventManager*,float) { abort(); }
void CPooledResource::ResetPlaybackState(CSoundEventManager*) { abort(); }
void CPooledResource::Set3DPosition(CSoundEventManager*,const CSoundPosition&) { abort(); }
void CPooledResource::SetPositionParameters(CSoundEventManager*,float,float) { abort(); }
void CPooledResource::ApplyPan(CSoundEventManager*,float) { abort(); }
bool CPooledResource::GetPositionFlag(CSoundEventManager*) { abort(); }
long* CSoundEvent::GetIndexSlot() { abort(); }
void CSoundEvent::ResetLevel(CSoundEventManager* manager) { abort(); }
void CSoundEvent::SetUserGain(CSoundEventManager* manager,float gain) { abort(); }
void CSoundEvent::SetPlaybackParameter(CSoundEventManager* manager,float value) { abort(); }
void CSoundEvent::ResetPlaybackState(CSoundEventManager* manager) { abort(); }
void CSoundEvent::Set3DPosition(CSoundEventManager* manager,const CSoundPosition& position) { abort(); }
void CSoundEvent::SetPositionParameters(CSoundEventManager* manager,float first,float second) { abort(); }
void CSoundEvent::ApplyPan(CSoundEventManager* manager,float pan) { abort(); }
bool CSoundEvent::GetPositionFlag(CSoundEventManager* manager) { abort(); }
void CStreamBlockReader::Close() { abort(); }

// Input remains opaque. The fixture payload belongs to the external Load double;
// production passes its address through without inventing a local Input engine model.
static unsigned char InputPayload[0x50];
static const Input* const ExpectedSettings=reinterpret_cast<const Input*>(InputPayload);
static CSoundEventManager* Manager;
static CStreamSoundBank* Bank;
static CSoundEvent* Resident;
static CStreamBlockReader* Stream;
static CPooledResource* LiveRequest;
static CPKVoiceParams* ExpectedParams;
static int LockDepth,Enters,Leaves,ParamCtor,Loads,Allocations,Created,Destroyed,Activated,Released,ReadCalls,BlockCalls,SampleQueries,DriverQueries,DriverCalls;
static int Mode,AllocFail,ReadResult,ThrowAt;
static long Position,Padding,FirstID,Owner;
static unsigned long Size,BlockSize;
static void* SampleToken=InputPayload+1;
static void* DriverToken=InputPayload+2;
static bool Cleaning;
struct PoolArea { CHeader Header; unsigned char Data[512]; };
static PoolArea Area;
static CHeader* Handles[2];
static unsigned long Offset(){return (unsigned long)Position%BlockSize;}
static unsigned long Tail(){unsigned long used=Offset()+Size;return (BlockSize-used%BlockSize)%BlockSize;}
static void Locked(){assert(LockDepth==1&&!Cleaning);}
static void MaybeThrow(int n){
#ifndef _MSC_VER
 if(ThrowAt==n)throw n;
#else
 assert(!ThrowAt);(void)n;
#endif
}
PKCCriticalSection::PKCCriticalSection(){}PKCCriticalSection::~PKCCriticalSection(){}
void PKCCriticalSection::Enter(){assert(LockDepth==0);++LockDepth;++Enters;}
void PKCCriticalSection::Leave(){assert(LockDepth==1);--LockDepth;++Leaves;}
PKCCriticalSection g_lock_0143bb14;
PKCProtectionInstance::PKCProtectionInstance(PKCCriticalSection*p):m_pSection(p),m_tick(0){assert(p==&g_lock_0143bb14);p->Enter();}
PKCProtectionInstance::~PKCProtectionInstance(){assert(m_pSection==&g_lock_0143bb14);m_pSection->Leave();}

// Real shared list types are constructed normally. Their external container work is controlled here.
template<class T>void PKCTRList<T>::MorphToArray(){abort();}
template<class T>void PKCTRList<T>::MorphToTree(){abort();}
template<class T>T*PKCTRList<T>::Find(const T*){abort();return 0;}
template<class T>void PKCTRList<T>::Remove(T*){abort();}
template<class T>void PKCTRList<T>::Add(T*p){Locked();assert((void*)this==(void*)&Manager->m_events);assert((void*)p==(void*)LiveRequest);assert(LiveRequest->m_state==1);++Activated;}
template<class T>void PKCTRList<T>::ForEach(void*){abort();}
template<class T>T*PKCTRList<T>::GetFirst(){abort();return 0;}
template<class T>T*PKCTRList<T>::GetLast(){abort();return 0;}
template<class T>T*PKCTRList<T>::GetItemByIndex(long){abort();return 0;}
template<class T>void PKCTRList<T>::Reserve(long){abort();}
template<class T>long PKCTRList<T>::GetCount(){abort();return 0;}
CStreamBankLookup::CStreamBankLookup():PKCTRList<void>(0){}
CSoundEventList::CSoundEventList():PKCTRList<CSoundEvent>(0){}
CSoundEventSet::CSoundEventSet():PKCTRList<CPooledResource>(0){}
void*CStreamBankLookup::FindByKey(const void*key){Locked();MaybeThrow(1);if(this==&Bank->m_samples){++SampleQueries;assert(*(const long*)key==Position);return Mode==1||Mode==2||Mode==4?SampleToken:0;}assert(this==&Bank->m_drivers);++DriverQueries;assert(*(void*const*)key==SampleToken);return Mode==1||Mode==2?DriverToken:0;}
CSoundPosition::CSoundPosition(){Position.X=Position.Y=Position.Z=0.0f;Relative=false;}
CPKVoiceParams::CPKVoiceParams(){ExpectedParams=this;++ParamCtor;memset(static_cast<void*>(this),0x35,sizeof(*this));Position.Relative=false;}
void CPKVoiceParams::Load(const Input*settings){Locked();assert(settings==ExpectedSettings);assert(this==ExpectedParams);if(LiveRequest)assert(this==&LiveRequest->m_params);++Loads;memcpy(static_cast<void*>(this),InputPayload,sizeof(*this));MaybeThrow(2);}
CSoundEvent*CSoundEventManager::PlayDriver(void*driver,long zero,const CPKVoiceParams*params,long zero2){Locked();assert(this==Manager&&driver==DriverToken&&zero==0&&zero2==0);assert(!LiveRequest&&params==ExpectedParams);assert(memcmp(params,InputPayload,sizeof(InputPayload))==0);++DriverCalls;return Mode==1?Resident:0;}
long CSoundEventSet::AllocateHandle(CSoundEventManager*manager,long bytes,long padding){Locked();assert(this==&Manager->m_events&&manager==Manager);assert(bytes==(long)(Offset()+Size+Tail())&&padding==Padding);assert(BlockCalls==3);++Allocations;return AllocFail?-1:1;}
void CHandleBasedMemoryPool_012b4098::Lock(long handle){Locked();assert(this==&Manager->m_events.Pool&&handle==1);CHeader*h=Handles[handle];if(!h->Locked){h->Locked=1;++LockCount;}}
void CHandleBasedMemoryPool_012b4098::Unlock(long handle){assert(Cleaning||LockDepth==1);assert(this==&Manager->m_events.Pool&&handle==1);CHeader*h=Handles[handle];if(h->Locked){h->Locked=0;--LockCount;}}
void*CHandleBasedMemoryPool_012b4098::GetPointerFromHandle(long handle){Locked();assert(this==&Manager->m_events.Pool&&handle==1&&LockCount==1);return reinterpret_cast<unsigned char*>(Handles[handle])+sizeof(CHeader);}
CPooledResource::CPooledResource():m_handle(0),m_id(0),m_offset(0),m_tail(0),m_state(0),m_flag(0){Locked();assert(!LiveRequest);LiveRequest=this;++Created;}
CPooledResource::~CPooledResource(){assert(Cleaning||LockDepth==1);assert(this==LiveRequest);LiveRequest=0;++Destroyed;}
long*CPooledResource::GetIndexSlot(){return &m_id;}
unsigned long CPooledResource::GetFirstBlock(){assert(this==LiveRequest);return (m_key-m_offset)/m_pBank->m_pStream->GetBlockSize();}
unsigned long CPooledResource::GetBlockCount(){assert(this==LiveRequest);return (m_offset+m_size+m_tail)/m_pBank->m_pStream->GetBlockSize();}
void CPooledResource::Activate(CSoundEventManager*manager){Locked();assert(this==LiveRequest&&manager==Manager);assert(m_handle==1&&m_id==FirstID&&m_padding==Padding);assert(m_size==Size&&m_offset==Offset()&&m_tail==Tail()&&m_key==(unsigned long)Position&&m_pBank==Bank);assert(!m_flag&&!m_state);assert(memcmp(&m_params,InputPayload,sizeof(InputPayload))==0);m_state=1;manager->m_events.Add(this);}
void CPooledResource::Release(CSoundEventManager*manager){assert(Cleaning||LockDepth==1);assert(this==LiveRequest&&manager==Manager&&m_state==1);assert(Area.Header.Locked==0&&manager->m_events.Pool.LockCount==0);m_state=0;++Released;}
unsigned long __fastcall GetStreamBlockOffset(CStreamBlockReader*stream,unsigned long position){unsigned long block=stream->GetBlockSize();return position-(position/block)*block;}
long __fastcall GetStreamBlockTail(CStreamBlockReader*stream,long position,long size){unsigned long used=GetStreamBlockOffset(stream,position)+size;unsigned long block=stream->GetBlockSize();return ((used+block-1)/block)*block-used;}
unsigned long CStreamBlockReader::GetBlockSize(){Locked();assert(this==Stream);++BlockCalls;return BlockSize;}
bool CStreamBlockReader::Read(unsigned long first,unsigned long count,unsigned long bytes,void*buffer,CPooledResource*request,long owner){Locked();assert(this==Stream&&request==LiveRequest&&owner==Owner);assert(first==(unsigned long)Position/BlockSize);assert(count==(Offset()+Size+Tail())/BlockSize&&bytes==Offset()+Size);assert(buffer==Area.Data&&Area.Header.Locked==1&&Manager->m_events.Pool.LockCount==1);assert(Activated==1&&BlockCalls==5);++ReadCalls;MaybeThrow(3);return ReadResult!=0;}
static void CleanRequest(){if(LiveRequest){Cleaning=true;Manager->m_events.Pool.Unlock(1);if(LiveRequest->m_state)LiveRequest->Release(Manager);delete LiveRequest;Cleaning=false;}}
static void Run(int mode,int allocfail,int read,long position,unsigned long size,unsigned long block,long padding,long id,long owner,int throwat){
 CSoundEventManager manager;CStreamSoundBank bank;CStreamBlockReader stream;CSoundEvent resident;
 Manager=&manager;Bank=&bank;Stream=&stream;Resident=&resident;LiveRequest=0;
 Mode=mode;AllocFail=allocfail;ReadResult=read;Position=position;Size=size;BlockSize=block;Padding=padding;FirstID=id;Owner=owner;ThrowAt=throwat;Cleaning=false;
 LockDepth=Enters=Leaves=ParamCtor=Loads=Allocations=Created=Destroyed=Activated=Released=ReadCalls=BlockCalls=SampleQueries=DriverQueries=DriverCalls=0;
 for(unsigned int i=0;i<sizeof(InputPayload);++i)InputPayload[i]=(unsigned char)(i*13+(unsigned long)position+size+mode);
 // Opaque external Load-double output: keep the newly represented real members valid.
 float px=(float)position,py=(float)size,pz=(float)mode;
 memcpy(InputPayload+0x30,&px,4);memcpy(InputPayload+0x34,&py,4);memcpy(InputPayload+0x38,&pz,4);
 InputPayload[0x3c]=(unsigned char)((position+mode)&1);
 memset(&Area,0,sizeof(Area));Handles[0]=0;Handles[1]=&Area.Header;manager.m_events.Pool.Handles._Myfirst=Handles;manager.m_events.Pool.Handles._Mylast=Handles+2;manager.m_events.Pool.Handles._Myend=Handles+2;manager.m_events.Pool.LockCount=0;
 manager.StreamOwner=owner;manager.m_nextId=id;bank.m_pStream=mode==0?0:&stream;bank.m_padding=padding;resident.Index=1729;
#ifdef _MSC_VER
 assert((char*)&manager.ListenerPosition-(char*)&manager==8&&(char*)&manager.ListenerFront-(char*)&manager==20&&(char*)&manager.ListenerTop-(char*)&manager==32);assert((char*)&manager.ListenerValid-(char*)&manager==0x885);
 assert(sizeof(CPooledResource)==0x78&&sizeof(CPKVoiceParams)==0x50);assert((char*)&manager.m_events-(char*)&manager==0x2c);assert((char*)&manager.m_events.Pool-(char*)&manager==0x360);assert((char*)&manager.StreamOwner-(char*)&manager==0x7b8);assert((char*)&manager.m_nextId-(char*)&manager==0x88c);assert((char*)&bank.m_samples-(char*)&bank==0x38&&(char*)&bank.m_drivers-(char*)&bank==0x50);
#endif
 long result=-999;
#ifndef _MSC_VER
 try{result=manager.PlayStreamed(&bank,position,size,ExpectedSettings);}catch(int n){assert(n==throwat);assert(LockDepth==0&&Enters==1&&Leaves==1);if(n==1)assert(!Created);if(n==2)assert(Loads==1);if(n==3)assert(ReadCalls==1&&Created==1&&Destroyed==0);CleanRequest();assert(!LiveRequest&&Created==Destroyed&&manager.m_events.Pool.LockCount==0);return;}
#else
 result=manager.PlayStreamed(&bank,position,size,ExpectedSettings);
#endif
 assert(!throwat);assert(LockDepth==0&&Enters==1&&Leaves==1);assert(bank.m_pStream==(mode==0?0:&stream)&&bank.m_padding==padding&&manager.StreamOwner==owner);
 if(mode==0){assert(result==-1&&!SampleQueries&&!ParamCtor&&!Allocations&&!Created&&!BlockCalls&&manager.m_nextId==id);}
 else if(mode==1||mode==2){assert(result==(mode==1?1729:-1));assert(SampleQueries==1&&DriverQueries==1&&DriverCalls==1&&ParamCtor==1&&Loads==1);assert(!Allocations&&!Created&&!ReadCalls&&!BlockCalls&&manager.m_nextId==id);}
 else {assert(SampleQueries==1&&DriverQueries==(mode==4?1:0)&&!DriverCalls&&Allocations==1);if(allocfail){assert(result==-1&&!Created&&!Loads&&BlockCalls==3&&manager.m_nextId==id);}else{assert(result==(read?id:-1));assert(manager.m_nextId==id+1&&Created==1&&ParamCtor==1&&Loads==1&&ReadCalls==1&&Activated==1);assert(Destroyed==(read?0:1)&&Released==(read?0:1));assert(manager.m_events.Pool.LockCount==(read?1:0)&&Area.Header.Locked==(read?1:0));if(read)assert(LiveRequest&&*LiveRequest->GetIndexSlot()==id);}}
 CleanRequest();assert(!LiveRequest&&Created==Destroyed&&manager.m_events.Pool.LockCount==0&&Area.Header.Locked==0);
}
int main(){unsigned long positions[]={0,1,7,32,129},sizes[]={0,1,4,31},blocks[]={1,4,16};long paddings[]={0,3},ids[]={0,31},owners[]={-1,77};long count=0;
 for(int mode=0;mode<5;++mode)for(int alloc=0;alloc<2;++alloc)for(int read=0;read<2;++read)for(int p=0;p<5;++p)for(int s=0;s<4;++s)for(int b=0;b<3;++b)for(int pad=0;pad<2;++pad)for(int id=0;id<2;++id)for(int owner=0;owner<2;++owner){Run(mode,alloc,read,positions[p],sizes[s],blocks[b],paddings[pad],ids[id],owners[owner],0);++count;}
#ifndef _MSC_VER
 for(int n=1;n<=3;++n)Run(3,0,1,7,31,4,3,31,77,n);
 assert(count==9600);puts("012117a5_TEST PASS");
#else
 assert(count==9600);puts("012117a5_TEST PASS");
#endif
 return 0;}
