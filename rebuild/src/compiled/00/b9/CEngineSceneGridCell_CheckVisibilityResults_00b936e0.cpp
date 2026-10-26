// CEngineSceneGridCell::CheckVisibilityResults -- retain visible primitives and detach newly visible group primitives. Retail00b936e0 complete181bytes; ignored args preserve ABI. Shared fields native, calleeB2FAC0 occluded count, B8FE60 remove, B92450 delete group. Shared flattened primitive headers restore native fields; virtual-only interface restores slot27. No cell/handle header. Callee00b2fa40 resets occlusion record through manager01436e58.
#ifndef _MSC_VER
#define __forceinline inline
class CEngineSceneGridCell;
class CEngineInternalPrimitiveBase { public:void* __vftable;unsigned char _pad_0x04[sizeof(long)],_pad_0x08[sizeof(long)];unsigned long EnginePrimitiveFlags;float AlphaMipBias,FadeDistance_FadeBeginDistance,FadeDistance_MaxDrawDistance;unsigned long OcclusionLayerBitMask;CEngineSceneGridCell* GridCell;unsigned char Other[16];CEngineInternalPrimitiveBase* NextPrimitive;void* RefPrimitive;CEngineInternalPrimitiveBase* NextLayerMask;void* RefLayerMask;};
#else
#include "engine/CEngineInternalPrimitiveBase.h"
#include "engine/CEngineInternalPrimitiveOcclusionBase.h"
#include "engine/CEnginePersistentPrimitiveBase.h"
#include "engine/CIVCountedPointeeBase.h"
#include "engine/CEnginePrimitiveOcclusionManager.h"
#include "engine/CEngineInternalPrimitiveOcclusionGroup.h"
#endif
class CEnginePrimitiveHandle;
struct CPersistentPrimitiveMethods {virtual ~CPersistentPrimitiveMethods();virtual void Delete()const;virtual void ReservedSlot2();virtual void NotifyReleased(const CEnginePrimitiveHandle&);long IVRefCount;long Type;};
template<class T> class CIVCountedPointer { public:T* m_p;
 long GetRefCount()const{return m_p?m_p->IVRefCount:0;}
 T* operator->()const{return m_p;}
 void Release(){T* p=m_p;if(p){if(--p->IVRefCount==0)p->Delete();m_p=0;}}
 CIVCountedPointer& operator=(T* p){Release();m_p=p;return *this;}
};
class CEnginePrimitiveHandle { public:void* VTable;CIVCountedPointer<CPersistentPrimitiveMethods> Data;
 __forceinline void Clear(){if(Data.GetRefCount()==1)Data->NotifyReleased(*this);Data=0;}
};
class CEnginePrimitiveOcclusionInfo;
struct CNativeOcclusionBase {CEngineInternalPrimitiveBase Base;CEnginePrimitiveOcclusionInfo* OcclusionInfo[2];};
struct CPrimitiveVirtuals {
 virtual void s00();virtual void s01();virtual void s02();virtual void s03();virtual void s04();virtual void s05();virtual void s06();virtual void s07();virtual void s08();virtual void s09();virtual void s10();virtual void s11();virtual void s12();virtual void s13();virtual void s14();virtual void s15();virtual void s16();virtual void s17();virtual void s18();virtual void s19();virtual void s20();virtual void s21();virtual void s22();virtual void s23();virtual void s24();virtual void s25();virtual void s26();virtual CNativeOcclusionBase* GetAsOcclusionPrimitive();
};
#ifndef _MSC_VER
class CEnginePrimitiveOcclusionManager { public:unsigned char Header[12];unsigned long PrimitiveOccludeDelay;};
#endif
struct CEnginePrimitiveOcclusionManagerMethods:CEnginePrimitiveOcclusionManager {unsigned long GetOccludedFrames(CEnginePrimitiveOcclusionInfo*); //00b2fac0
 __forceinline bool IsVisible(CEnginePrimitiveOcclusionInfo* info){if(GetOccludedFrames(info)>PrimitiveOccludeDelay)return false;return true;}
};
extern CEnginePrimitiveOcclusionManagerMethods* g_pPrimitiveOcclusionManager; //01436e58
class CEngineSceneGridCell { public:CEngineSceneGridCell* Parent;CEngineInternalPrimitiveBase* FirstPrimitive;float BoundingBox[6],DrawDistance;CEnginePrimitiveHandle GroupPrimitiveHandle[2];long LocalCount;unsigned long TotalCount,RenderMask,OccludedLayers;unsigned char Flags,OccludedFrames,PreloadFrames;void DeleteOcclusionGroup(long layer,bool reset);bool CheckVisibilityResults(long layer,unsigned long,unsigned long,bool force);};
struct CPrimitiveMethods:CEngineInternalPrimitiveBase {void RemoveFromList();}; //00b8fe60
struct CNativeOcclusionGroup {CNativeOcclusionBase Base;CEngineSceneGridCell* GridCell;void* Grid;unsigned long GridX,GridY,FramesRendered;};
static __forceinline bool IsLayerVisible(CNativeOcclusionBase* p,long layer){if(layer!=-1){CEnginePrimitiveOcclusionInfo* info=p->OcclusionInfo[layer];if(info && g_pPrimitiveOcclusionManager->IsVisible(info))return true;}return false;}
bool CEngineSceneGridCell::CheckVisibilityResults(long layer,unsigned long,unsigned long,bool force){
 bool visible=false;
 for(CEngineInternalPrimitiveBase* p=FirstPrimitive;p;){
  CEngineInternalPrimitiveBase* next=p->NextPrimitive;
  CNativeOcclusionBase* occ=reinterpret_cast<CPrimitiveVirtuals*>(p)->GetAsOcclusionPrimitive();
  if(occ && (force || (occ->Base.EnginePrimitiveFlags&0x4000) || IsLayerVisible(occ,layer))){
   visible=true;OccludedFrames=0;
   if(reinterpret_cast<CPersistentPrimitiveMethods*>(p)->Type==0x29){
    CNativeOcclusionGroup* group=reinterpret_cast<CNativeOcclusionGroup*>(p);CEngineSceneGridCell* cell=group->GridCell;
    reinterpret_cast<CPrimitiveMethods*>(p)->RemoveFromList();cell->DeleteOcclusionGroup(layer,group->FramesRendered>1);
   }
  }
  p=next;
 }
 if(visible)return true;
 ++OccludedFrames;return false;
}