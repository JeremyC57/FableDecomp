// CEnginePrimitiveOcclusionGroupManager::RenderPrimitive -- first render lazily creates current-layer occlusion info unless disabled, advances frame count, queues a bounding-box subprimitive. Retail00bacc50 body184bytes ret12,8int3 padding. Calls globalnew00bfea1a,infoctor00b2fa90,poolnew00beb540,subbasector0099a2f0,sortedqueue00b84720. RTTI sub012a3e84=CEngineSubPrimitiveBoundingBox. CurrentOcclusionLayer is shareddata+78, not renderer sceneindex. Native base OcclusionInfo[2]+48,FramesRendered+60; PDB donor queryInfo36 versus retail32 from CArray16/12. Shared flattened headers used with narrow methods/accessors for hiddenbase slots.
// Naming (naming wave 24): ~CBaseClassNonCopyable 0x0099A300 is protected virtual (Ego_r.pdb / FableWin.pdb ??1CBaseClassNonCopyable@@MAE@XZ, engine/CBaseClassNonCopyable.h).
#include "fable_compiler.h"
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
struct COcclusionPointee {virtual ~COcclusionPointee();virtual void Delete() const;unsigned long IVRefCount; FABLE_FORCEINLINE void Release(){if(--IVRefCount==0)Delete();}};
class CEnginePrimitiveOcclusionInfo:public COcclusionPointee { public:long InvisibleFrames;void* TestFirst;void* TestLast;void* TestEnd;bool Seen;unsigned long TestFrame;CEnginePrimitiveOcclusionInfo();}; //00b2fa90
struct COcclusionPointer {
 COcclusionPointee* Object;
 FABLE_FORCEINLINE void SetNew(CEnginePrimitiveOcclusionInfo* p){if(Object){Object->Release();Object=0;}Object=p;}
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
struct CBaseClassNonCopyable {protected: virtual ~CBaseClassNonCopyable(); public:protected:CBaseClassNonCopyable();}; //0099a2f0 ??0CBaseClassNonCopyable@@IAE@XZ
struct CEngineSubPrimitiveBoundingBox;
class __declspec(novtable) CEngineSubPrimitiveBase:public CBaseClassNonCopyable { public:
 unsigned long RenderFlags;unsigned char Alpha;CEngineInternalPrimitiveBase* Parent;CEngineSubPrimitiveBase* NextSubPrimitive;CEnginePrimitiveOcclusionGroupManager* Manager;
 CEngineSubPrimitiveBase(CEngineInternalPrimitiveBase& p):Parent(&p),NextSubPrimitive(0){}
 static void* __fastcall operator new(size_t); //00beb540
};
struct CEngineSubPrimitiveBoundingBox:CEngineSubPrimitiveBase {CEngineSubPrimitiveBoundingBox(CEngineInternalPrimitiveBase& p):CEngineSubPrimitiveBase(p){}virtual ~CEngineSubPrimitiveBoundingBox();};
enum ESubPrimitiveType {};
class CEnginePrimitiveManager;
class CEngineSubPrimitiveRenderer { public: void AddSubPrimitive(ESubPrimitiveType,CEngineSubPrimitiveBase*,float,CEnginePrimitiveManager*); }; //00b84720 (Ego_r/FableWin spelling)
extern CEngineSubPrimitiveRenderer* g_pSubPrimitiveRenderer; //01436e74
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
 g_pSubPrimitiveRenderer->AddSubPrimitive((ESubPrimitiveType)7,sub,0,(CEnginePrimitiveManager*)this);
 return RENDERED;
}