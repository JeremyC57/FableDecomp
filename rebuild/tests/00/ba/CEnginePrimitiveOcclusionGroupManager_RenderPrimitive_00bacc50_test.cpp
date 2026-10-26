// CEnginePrimitiveOcclusionGroupManager::RenderPrimitive -- first render lazily creates current-layer occlusion info unless disabled, advances frame count, queues a bounding-box subprimitive. Retail00bacc50 body184bytes ret12,8int3 padding. Calls globalnew00bfea1a,infoctor00b2fa90,poolnew00beb540,subbasector0099a2f0,sortedqueue00b84720. RTTI sub012a3e84=CEngineSubPrimitiveBoundingBox. CurrentOcclusionLayer is shareddata+78, not renderer sceneindex. Native base OcclusionInfo[2]+48,FramesRendered+60; PDB donor queryInfo36 versus retail32 from CArray16/12. Shared flattened headers used with narrow methods/accessors for hiddenbase slots.
#ifndef _MSC_VER
#define __declspec(x)
#define __forceinline inline
#define __fastcall
#endif
#include <stddef.h>
#ifdef _MSC_VER
#include "engine/CEngineInternalPrimitiveBase.h"
#include "engine/CEngineInternalPrimitiveOcclusionGroup.h"
#include "engine/CEnginePrimitiveOcclusionGroupManager.h"
#include "engine/CIVCountedPointeeBase.h"
#else
class CEngineInternalPrimitiveBase { public:unsigned long EnginePrimitiveFlags;};
struct COcclusionPointer;
class CEngineInternalPrimitiveOcclusionGroup:public CEngineInternalPrimitiveBase { public:COcclusionPointer* HostInfos;unsigned long FramesRendered;};
class CEnginePrimitiveOcclusionGroupManager { public:void* VTable;};
#endif
struct COcclusionPointee {virtual ~COcclusionPointee();virtual void Delete() const;unsigned long IVRefCount; __forceinline void Release(){if(--IVRefCount==0)Delete();}};
class CEnginePrimitiveOcclusionInfo:public COcclusionPointee { public:long InvisibleFrames;void* TestFirst;void* TestLast;void* TestEnd;bool Seen;unsigned long TestFrame;CEnginePrimitiveOcclusionInfo();}; //00b2fa90
struct COcclusionPointer {
 COcclusionPointee* Object;
 __forceinline void SetNew(CEnginePrimitiveOcclusionInfo* p){if(Object){Object->Release();Object=0;}Object=p;}
};
static COcclusionPointer& GetInfo(CEngineInternalPrimitiveOcclusionGroup& g,long index){
#ifdef _MSC_VER
 return reinterpret_cast<COcclusionPointer*>(g._pad_0x48)[index];
#else
 return g.HostInfos[index];
#endif
}
static unsigned long PrimitiveFlags(CEngineInternalPrimitiveBase& p){return p.EnginePrimitiveFlags;}
// PDB CEnginePrimitiveRendererSharedData CurrentOcclusionLayer+78; no shared header exists.
struct CEnginePrimitiveRendererSharedData {unsigned char InterpolationAndLoadState[0x78];long CurrentOcclusionLayer;};
extern CEnginePrimitiveRendererSharedData* g_pPrimitiveRendererSharedData; //01436e78
struct CSubBase {virtual ~CSubBase();CSubBase();}; //0099a2f0
struct CEngineSubPrimitiveBoundingBox;
struct __declspec(novtable) CSubPrimitiveBase:CSubBase {
 unsigned long RenderFlags;unsigned char Alpha;CEngineInternalPrimitiveBase* Parent;CSubPrimitiveBase* NextSubPrimitive;CEnginePrimitiveOcclusionGroupManager* Manager;
 CSubPrimitiveBase(CEngineInternalPrimitiveBase& p):Parent(&p),NextSubPrimitive(0){}
 static void* __fastcall operator new(size_t); //00beb540
};
struct CEngineSubPrimitiveBoundingBox:CSubPrimitiveBase {CEngineSubPrimitiveBoundingBox(CEngineInternalPrimitiveBase& p):CSubPrimitiveBase(p){}virtual ~CEngineSubPrimitiveBoundingBox();};
struct COcclusionSubRenderer {void AddSorted(unsigned long,CSubPrimitiveBase*,float,CEnginePrimitiveOcclusionGroupManager*);}; //00b84720
extern COcclusionSubRenderer* g_pSubPrimitiveRenderer; //01436e74
class CEngineSubPrimitiveChain;
enum ERendered {NOTHING=0,RENDERED=2};enum ELayer {DEFAULT_LAYER=0};
struct CEnginePrimitiveOcclusionGroupManager_Methods:CEnginePrimitiveOcclusionGroupManager {ERendered RenderPrimitive(CEngineInternalPrimitiveBase&,ELayer,CEngineSubPrimitiveChain&);};
ERendered CEnginePrimitiveOcclusionGroupManager_Methods::RenderPrimitive(CEngineInternalPrimitiveBase& primitive,ELayer,CEngineSubPrimitiveChain&)
{
 CEngineInternalPrimitiveOcclusionGroup& group=reinterpret_cast<CEngineInternalPrimitiveOcclusionGroup&>(primitive);
 if(group.FramesRendered==0){
  long index=g_pPrimitiveRendererSharedData->CurrentOcclusionLayer;
  if(index!=-1 && !(PrimitiveFlags(primitive)&0x4000) && !GetInfo(group,index).Object){
   CEnginePrimitiveOcclusionInfo* info=new CEnginePrimitiveOcclusionInfo;
   GetInfo(group,index).SetNew(info);
  }
 }
 ++group.FramesRendered;
 CEngineSubPrimitiveBoundingBox* sub=new CEngineSubPrimitiveBoundingBox(primitive);
 g_pSubPrimitiveRenderer->AddSorted(7,sub,0,this);
 return RENDERED;
}

#include <cstdio>
#include <cstdlib>
class CEngineSubPrimitiveChain { public:int Marker;};
static int failures=0,infoAllocations=0,infoCtors=0,poolAllocations=0,subCtors=0,queueCalls=0,deleteCalls=0;
static bool failInfo=false,failPool=false,injectOld=false;
static COcclusionPointee* oldInfo=0;
static CEngineInternalPrimitiveOcclusionGroup* currentGroup=0;
static CEnginePrimitiveOcclusionGroupManager_Methods* currentManager=0;
static CEngineSubPrimitiveBoundingBox* queued=0;
static long selectedLayer;
static unsigned long expectedFrame;
CEnginePrimitiveRendererSharedData* g_pPrimitiveRendererSharedData=0;
COcclusionSubRenderer* g_pSubPrimitiveRenderer=0;
static void check(bool b){if(!b)++failures;}
void* operator new(size_t n){++infoAllocations;check(n==sizeof(CEnginePrimitiveOcclusionInfo));return failInfo?0:std::malloc(n);}
void operator delete(void* p){std::free(p);}
COcclusionPointee::~COcclusionPointee(){}
void COcclusionPointee::Delete()const{++deleteCalls;}
CEnginePrimitiveOcclusionInfo::CEnginePrimitiveOcclusionInfo(){++infoCtors;IVRefCount=1;InvisibleFrames=0;TestFirst=TestLast=TestEnd=0;Seen=false;TestFrame=19;if(injectOld)GetInfo(*currentGroup,selectedLayer).Object=oldInfo;}
CSubBase::CSubBase(){++subCtors;}
CSubBase::~CSubBase(){}
CEngineSubPrimitiveBoundingBox::~CEngineSubPrimitiveBoundingBox(){}
void* CSubPrimitiveBase::operator new(size_t n){++poolAllocations;check(n==sizeof(CEngineSubPrimitiveBoundingBox));return failPool?0:std::malloc(n);}
void COcclusionSubRenderer::AddSorted(unsigned long layer,CSubPrimitiveBase* p,float distance,CEnginePrimitiveOcclusionGroupManager* manager){++queueCalls;check(this==g_pSubPrimitiveRenderer && layer==7 && distance==0 && manager==currentManager && currentGroup->FramesRendered==expectedFrame);check((p==0)==failPool);if(p)check(p->Parent==reinterpret_cast<CEngineInternalPrimitiveBase*>(currentGroup) && !p->NextSubPrimitive);queued=static_cast<CEngineSubPrimitiveBoundingBox*>(p);}
static void run(unsigned long frames,long layer,unsigned long flags,bool existing,bool infoFailure,bool poolFailure,unsigned long oldRefs)
{
 infoAllocations=infoCtors=poolAllocations=subCtors=queueCalls=deleteCalls=0;queued=0;failInfo=infoFailure;failPool=poolFailure;injectOld=oldRefs!=0;selectedLayer=layer;
 CEnginePrimitiveOcclusionGroupManager_Methods manager;currentManager=&manager;
 CEngineInternalPrimitiveOcclusionGroup group;currentGroup=&group;
#ifndef _MSC_VER
 COcclusionPointer hostSlots[2];group.HostInfos=hostSlots;
#endif
 GetInfo(group,0).Object=GetInfo(group,1).Object=0;
 COcclusionPointee previous;previous.IVRefCount=oldRefs;oldInfo=&previous;
 if(existing)GetInfo(group,layer).Object=&previous;
 group.FramesRendered=frames;reinterpret_cast<CEngineInternalPrimitiveBase&>(group).EnginePrimitiveFlags=flags;
 CEnginePrimitiveRendererSharedData shared;shared.CurrentOcclusionLayer=layer;g_pPrimitiveRendererSharedData=&shared;
 COcclusionSubRenderer renderer;g_pSubPrimitiveRenderer=&renderer;
 CEngineSubPrimitiveChain chain;chain.Marker=631;expectedFrame=frames+1;
 ERendered result=manager.RenderPrimitive(reinterpret_cast<CEngineInternalPrimitiveBase&>(group),(ELayer)123,chain);
 bool creates=frames==0 && layer!=-1 && !(flags&0x4000) && !existing;
 check(result==RENDERED && chain.Marker==631 && group.FramesRendered==expectedFrame);
 check(infoAllocations==(creates?1:0) && infoCtors==(creates&&!infoFailure?1:0));check(poolAllocations==1 && queueCalls==1 && subCtors==(poolFailure?0:1));
 if(creates){COcclusionPointee* info=GetInfo(group,layer).Object;check((info==0)==infoFailure);if(info){check(info!=&previous && info->IVRefCount==1);delete info;}}
 if(creates && !infoFailure && injectOld){check(previous.IVRefCount==oldRefs-1 && deleteCalls==(oldRefs==1?1:0));}else check(deleteCalls==0);
 if(layer>=0)check(!GetInfo(group,1-layer).Object);
 if(queued)delete queued;
}
int main(){
#ifdef _MSC_VER
 check(sizeof(COcclusionPointee)==sizeof(CIVCountedPointeeBase) && sizeof(CEnginePrimitiveOcclusionInfo)==0x20 && sizeof(CEngineSubPrimitiveBoundingBox)==0x18);
#endif
 run(1,0,0,false,false,false,0);run(0,-1,0,false,false,false,0);run(0,0,0x4000,false,false,false,0);run(0,1,0,true,false,false,2);run(0,0,0,false,false,false,0);run(0,1,0,false,false,false,1);run(0,0,0,false,false,false,3);run(0,0,0,false,true,false,0);run(0,1,0,false,false,true,0);
 if(failures){std::printf("00bacc50_TEST FAIL %d\n",failures);return 1;}std::printf("00bacc50_TEST PASS\n");return 0;
}