// CEngineInternalPrimitiveSpriteTrail::~CEngineInternalPrimitiveSpriteTrail() -- Release owned sequence storage and destroy the base.
// Leaf: ??1CEngineInternalPrimitiveSpriteTrail@@UAE@XZ
// Identity: Own deleting-dtor00bcc320 and full RTTI12a6a4c; actual inherited hierarchy and original Trail+104; base vtable12354d0.
#define CIVCountedPointeeBase CIVCountedPointeeBase_HeaderLayout
#include "engine/CIVCountedPointeeBase.h"
#undef CIVCountedPointeeBase
#define CEnginePersistentPrimitiveBase CEnginePersistentPrimitiveBase_HeaderLayout
#include "engine/CEnginePersistentPrimitiveBase.h"
#undef CEnginePersistentPrimitiveBase
#define CEngineInternalPrimitiveBase CEngineInternalPrimitiveBase_HeaderLayout
#include "engine/CEngineInternalPrimitiveBase.h"
#undef CEngineInternalPrimitiveBase
#include <stdlib.h>
#ifndef _MSC_VER
#define __forceinline inline
#endif
namespace std {
template<class T>class allocator{};
template<class V,class T,class A>class _STLP_alloc_proxy:public A {public:V _M_data;};
template<class T,class A>class _Vector_base {public:T* _M_start;T* _M_finish;_STLP_alloc_proxy<T*,T,A> _M_end_of_storage;_Vector_base(){_M_start=0;_M_finish=0;_M_end_of_storage._M_data=0;}__forceinline ~_Vector_base(){if(_M_start)free(_M_start);}};
template<class T,class A=allocator<T> >class vector:public _Vector_base<T,A> {public:__forceinline ~vector(){for(T* p=this->_M_start;p!=this->_M_finish;++p)p->~T();}};
}
template<class T>class CArray {public:std::vector<T> Data;};
// Layout: shared base headers describe flattened vptrs; genuine polymorphic hierarchy is required for compiler-generated destruction. Original base fields and offsets retained; no target base reconstruction claim.
class CIVCountedPointeeBase {public:virtual __forceinline ~CIVCountedPointeeBase(){}virtual void Delete()const;unsigned long IVRefCount;};
class CEnginePersistentPrimitiveBase:public CIVCountedPointeeBase {public:long Type;};
class CEngineSceneGridCell;
class CEngineInternalPrimitiveBase:public CEnginePersistentPrimitiveBase {public:
unsigned long EnginePrimitiveFlags;float AlphaMipBias;struct CEngineFadeDistance{float FadeBeginDistance,MaxDrawDistance;} FadeDistance;unsigned long OcclusionLayerBitMask;CEngineSceneGridCell* GridCell;
struct CEngineSceneGridPos{signed char Level;unsigned char X,Y;} GridPos;unsigned char Alpha,AlphaRef,PreloadStat;unsigned short Pad;int InterpolationSet;unsigned long RenderLayerMask;unsigned short StateFlags,LastFrameRendered;CEngineInternalPrimitiveBase* NextPrimitive;CEngineInternalPrimitiveBase** RefPrimitive;CEngineInternalPrimitiveBase* NextLayerMask;CEngineInternalPrimitiveBase** RefLayerMask;
};
class C3DVector {public:float X,Y,Z;};
class CEngineInternalPrimitiveSpriteTrail:public CEngineInternalPrimitiveBase {public:class CSpriteTrailEntry {public:C3DVector Position;float Width;};unsigned long MaxLength,GraphicIndex,GraphicFrame;float SkipPoints,Width;unsigned int HeadColour,MidColour,TailColour;CArray<CSpriteTrailEntry> Trail;long NextPos;unsigned short Flags;int BlendOp,BlendMode;float WorldMat[12];};


void DestroyCandidate(CEngineInternalPrimitiveSpriteTrail* p){p->CEngineInternalPrimitiveSpriteTrail::~CEngineInternalPrimitiveSpriteTrail();}