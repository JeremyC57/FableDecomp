// CEngineSceneGridCell::DeleteOcclusionGroup -- clear layer group handle and optionally reset member primitive occlusion histories. Retail00b92450 complete172bytes. Shared flattened primitive headers restore native fields; virtual-only interface restores slot27. No cell/handle header. Callee00b2fa40 resets occlusion record through manager01436e58.
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
struct CEnginePrimitiveOcclusionManagerMethods:CEnginePrimitiveOcclusionManager {void Reset(CEnginePrimitiveOcclusionInfo*);}; //00b2fa40
extern CEnginePrimitiveOcclusionManagerMethods* g_pPrimitiveOcclusionManager; //01436e58
class CEngineSceneGridCell { public:CEngineSceneGridCell* Parent;CEngineInternalPrimitiveBase* FirstPrimitive;float BoundingBox[6],DrawDistance;CEnginePrimitiveHandle GroupPrimitiveHandle[2];long LocalCount;unsigned long TotalCount,RenderMask,OccludedLayers;unsigned char Flags,OccludedFrames,PreloadFrames;void DeleteOcclusionGroup(long layer,bool reset);};
void CEngineSceneGridCell::DeleteOcclusionGroup(long layer,bool reset){
 unsigned long mask;OccludedLayers=OccludedLayers&~(mask=1UL<<(layer+1));GroupPrimitiveHandle[layer].Clear();
 if(reset){for(CEngineInternalPrimitiveBase* p=FirstPrimitive;p;p=p->NextPrimitive){
  if(reinterpret_cast<CPersistentPrimitiveMethods*>(p)->Type!=0x29){
   CNativeOcclusionBase* occ=reinterpret_cast<CPrimitiveVirtuals*>(p)->GetAsOcclusionPrimitive();
   if(occ && (p->OcclusionLayerBitMask&mask) && layer!=-1){CEnginePrimitiveOcclusionInfo* info=occ->OcclusionInfo[layer];if(info)g_pPrimitiveOcclusionManager->Reset(info);}
  }
 }}
}