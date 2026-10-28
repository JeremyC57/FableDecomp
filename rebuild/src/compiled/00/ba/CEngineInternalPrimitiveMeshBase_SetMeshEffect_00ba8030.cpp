// CEngineInternalPrimitiveMeshBase::SetMeshEffect(const Pointer& value) -- replace the owned counted pointer.
// Leaf: ?SetMeshEffect@CEngineInternalPrimitiveMeshBase@@UAEXABV?$CIVCountedPointer@VCEngineMeshEffect@@@@@Z
// Identity: proven; RTTI vtable0x12A41D4 slot25; PDB SetMeshEffect(CIVCountedPointer<CEngineMeshEffect>const&) virtual; PDB own MeshEffect+104.
// Storage audit: original CIVCountedPointer<T>::Object storage is CIVCountedPointeeBase*, preserving original template/API identity. Prior selected snapshot/history unchanged.
// Layout: no usable engine header (mesh-base header quarantined); partial class preserves own typed member at retail offset 0x68.
// Complete-model audit: original field/base/signature revision; unused common declarations pruned. IVRefCount is unsigned long, Release()const returns long, definition pointers store CDefPointeeBase* and default through ENullDef. All helper uncertainty and omitted unused APIs are documented in COMPLETE_INTRUSIVE_MODEL_AUDIT.md. No original full layout is claimed for forward-only pointees.
#define CIVCountedPointeeBase CIVCountedPointeeBase_HeaderEvidence
#include "engine/CIVCountedPointeeBase.h"
#undef CIVCountedPointeeBase
#define CEnginePersistentPrimitiveBase CEnginePersistentPrimitiveBase_HeaderEvidence
#include "engine/CEnginePersistentPrimitiveBase.h"
#undef CEnginePersistentPrimitiveBase
#define CEngineInternalPrimitiveBase CEngineInternalPrimitiveBase_HeaderEvidence
#include "engine/CEngineInternalPrimitiveBase.h"
#undef CEngineInternalPrimitiveBase
#define CEngineInternalPrimitiveOcclusionBase CEngineInternalPrimitiveOcclusionBase_HeaderEvidence
#include "engine/CEngineInternalPrimitiveOcclusionBase.h"
#undef CEngineInternalPrimitiveOcclusionBase
#ifndef _MSC_VER
#define __fastcall
#define __cdecl
#define __forceinline inline
#define NOINLINE __attribute__((noinline))
typedef __SIZE_TYPE__ PlacementSize;
#else
#define NOINLINE __declspec(noinline)
typedef unsigned int PlacementSize;
#endif
inline void* operator new(PlacementSize,void* p){return p;}
class CIVCountedPointeeBase{public:CIVCountedPointeeBase():IVRefCount(1){}virtual ~CIVCountedPointeeBase(){}virtual void Delete()const;mutable unsigned long IVRefCount;void AddRef()const{++IVRefCount;}long GetRefCount()const{return (long)IVRefCount;}long Release()const{long result=(long)--IVRefCount;if(result==0)Delete();return result;}};
template<class T>class CIVCountedPointer{public:CIVCountedPointeeBase* Object;CIVCountedPointer():Object(0){}CIVCountedPointer(const CIVCountedPointer& other):Object(other.Object){if(Object)Object->AddRef();}__forceinline ~CIVCountedPointer(){if(Object){Object->Release();Object=0;}}CIVCountedPointer& operator=(const CIVCountedPointer& other){if(Object!=other.Object){if(Object){Object->Release();Object=0;}Object=other.Object;if(Object)Object->AddRef();}return *this;}T* operator->()const{return (T*)Object;}};
class CRGBColour{public:unsigned char R,G,B,A;};class CEngineFadeDistance{public:float FadeBeginDistance,MaxDrawDistance;};class CEngineSceneGridPos{public:signed char Level;unsigned char X,Y;};enum EEngineInterpolationSet{EEngineInterpolationSet_DescriptiveZero};class CEnginePrimitiveThemeChannelDesc{public:unsigned char ChannelIndex0,ChannelIndex1,ChannelBlendFactor,Pad;};class CEngineSceneGridCell;
class CEnginePersistentPrimitiveBase:public CIVCountedPointeeBase{public:long Type;};class CEngineInternalPrimitiveBase:public CEnginePersistentPrimitiveBase{public:unsigned long EnginePrimitiveFlags;float AlphaMipBias;CEngineFadeDistance FadeDistance;unsigned long OcclusionLayerBitMask;CEngineSceneGridCell* GridCell;CEngineSceneGridPos GridPos;unsigned char Alpha,AlphaRef,PreloadStat;unsigned char _unmodeled2A[2];EEngineInterpolationSet InterpolationSet;unsigned long RenderLayerMask;unsigned char _unmodeledRenderFlags34[2];unsigned short LastFrameRendered;CEngineInternalPrimitiveBase* NextPrimitive;CEngineInternalPrimitiveBase** RefPrimitive;CEngineInternalPrimitiveBase* NextLayerMask;CEngineInternalPrimitiveBase** RefLayerMask;CEngineInternalPrimitiveBase(long,unsigned long);virtual void UnknownPrimitiveSlot2(){}virtual void UnknownPrimitiveSlot3(){}virtual void UnknownPrimitiveSlot4(){}virtual void UnknownPrimitiveSlot5(){}virtual void UnknownPrimitiveSlot6(){}virtual void UnknownPrimitiveSlot7(){}virtual void UnknownPrimitiveSlot8(){}virtual void UnknownPrimitiveSlot9(){}virtual void UnknownPrimitiveSlot10(){}virtual void UnknownPrimitiveSlot11(){}virtual void UnknownPrimitiveSlot12(){}virtual void UnknownPrimitiveSlot13(){}virtual void UnknownPrimitiveSlot14(){}virtual void UnknownPrimitiveSlot15(){}virtual void UnknownPrimitiveSlot16(){}virtual void UnknownPrimitiveSlot17(){}virtual void UnknownPrimitiveSlot18(){}virtual void UnknownPrimitiveSlot19(){}virtual void UnknownPrimitiveSlot20(){}virtual void UnknownPrimitiveSlot21(){}virtual void UnknownPrimitiveSlot22(){}virtual void UnknownPrimitiveSlot23(){}virtual void UnknownPrimitiveSlot24(){}};
class CEnginePrimitiveOcclusionInfo;class CEngineInternalPrimitiveOcclusionBase:public CEngineInternalPrimitiveBase{public:CIVCountedPointer<CEnginePrimitiveOcclusionInfo> OcclusionInfo[2];CEngineInternalPrimitiveOcclusionBase(long,unsigned long);};class C3DMeshInfo;class CEnginePrimitiveSimulationInfo;class CEngineMeshEffect;class CEngineInternalPrimitiveMeshBase:public CEngineInternalPrimitiveOcclusionBase{public:C3DMeshInfo* MeshInfo;C3DMeshInfo* ShadowMeshInfo;float Scale;CRGBColour TintColour;bool BulletTimeAffectsCloth;CIVCountedPointer<CEnginePrimitiveSimulationInfo> SimulationInfo;CIVCountedPointer<CEngineMeshEffect> MeshEffect;CEnginePrimitiveThemeChannelDesc ThemeChannelDesc;CEngineInternalPrimitiveMeshBase(long,unsigned long);virtual void SetMeshEffect(const CIVCountedPointer<CEngineMeshEffect>&);};void CEngineInternalPrimitiveMeshBase::SetMeshEffect(const CIVCountedPointer<CEngineMeshEffect>& value){MeshEffect=value;}

#ifdef _MSC_VER
typedef char AuditSize_0[(sizeof(CIVCountedPointeeBase)==8)?1:-1];
typedef char AuditSize_1[(sizeof(CEnginePersistentPrimitiveBase)==12)?1:-1];
typedef char AuditSize_2[(sizeof(CEngineInternalPrimitiveBase)==72)?1:-1];
typedef char AuditSize_3[(sizeof(CEngineInternalPrimitiveOcclusionBase)==80)?1:-1];
typedef char AuditSize_4[(sizeof(CEngineInternalPrimitiveMeshBase)==112)?1:-1];
#endif