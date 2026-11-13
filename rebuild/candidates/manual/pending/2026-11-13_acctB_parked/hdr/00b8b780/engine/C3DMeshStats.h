#ifndef FABLE_ENGINE_C3DMESHSTATS_H
#define FABLE_ENGINE_C3DMESHSTATS_H
#include "rebuild_abi.h"
#include "engine/CMovableResource.h"
#include "engine/C3DBoundingSphere.h"
#include "engine/C3DBoundingBox.h"
#include "fable_string.h"
class CMovableResourceMemoryPool;
#ifdef FABLE_RETAIL_C3DMESHSTATS_DUMMYOBJECTS
// Retail opt-in (chunk Q21, row 0x00B39080): the own virtuals of the abstract stats class (C3DMesh2 0x0129DEF0 overrides them, slots
// 4..9; the C3DMeshStats vtable 0x0129CF04 holds _purecall there) and the out-of-line GetDummyObject. Needs the polymorphic
// FABLE_RETAIL_CMOVABLERESOURCE base (vptr +0; no layout change).
#ifndef FABLE_RETAIL_CMOVABLERESOURCE
#error FABLE_RETAIL_C3DMESHSTATS_DUMMYOBJECTS needs FABLE_RETAIL_CMOVABLERESOURCE
#endif
class C3DDummyObject;
class C3DHelperPoint;
class C3DAnimationBlendState;
class C3DAnimationManager;
class CMatrix3x4;
class C3DVector;
#endif
// Full original Ego_r0x3580 data range, native STLport maps/arrays. External declaration only.
class C3DMeshStats: public CMovableResource {
public:
#ifdef FABLE_RETAIL_C3DMESHSTATS_BOUNDINGSPHERE
 // Opt-in (row 0x00B8B780): FableWin.pdb ?GetBoundingSphere@C3DMeshStats@@QBE?AVC3DBoundingSphere@@XZ (inline, by value).
 C3DBoundingSphere GetBoundingSphere() const { return BoundingSphere; }
#endif
 bool HasDummyObject(unsigned long)const;
#ifdef FABLE_RETAIL_C3DMESHSTATS_DUMMYOBJECTS
 // Ego_r.pdb ?GetDummyObject@C3DMeshStats@@QBEPBVC3DDummyObject@@KK@Z (retail 0x00A2E920). The six virtuals are Ego_r.pdb / FableWin.pdb
 // C3DMesh2 overrides (UBE); declared here in source order so VC's overload grouping gives retail's C3DMesh2 slots: 4/5
 // GetHelperPointPositionAnimated (0x00A898D0 / 0x00A89740), 6/7 GetDummyObjectPositionAnimated (two-state 0x00A89E40 / one-state
 // 0x00A89A70), 8/9 GetDummyObjectTransformAnimated (two-state 0x00A8A5F0, called through +0x20 at 0x00B39141 / one-state 0x00A8A220).
 const C3DDummyObject* GetDummyObject(unsigned long name_crc,unsigned long index)const;   // 0x00A2E920
 virtual C3DVector GetHelperPointPositionAnimated(const C3DHelperPoint&,const C3DAnimationBlendState&,C3DAnimationManager&)const=0;
 virtual C3DVector GetHelperPointPositionAnimated(const C3DHelperPoint&,const C3DAnimationBlendState&,const C3DAnimationBlendState&,float,C3DAnimationManager&)const=0;
 virtual void GetDummyObjectPositionAnimated(const C3DDummyObject&,C3DVector&,const C3DAnimationBlendState&,C3DAnimationManager&,bool)const=0;
 virtual void GetDummyObjectPositionAnimated(const C3DDummyObject&,C3DVector&,const C3DAnimationBlendState&,const C3DAnimationBlendState&,float,C3DAnimationManager&,bool)const=0;
 virtual void GetDummyObjectTransformAnimated(const C3DDummyObject&,CMatrix3x4&,const C3DAnimationBlendState&,C3DAnimationManager&,bool)const=0;
 virtual void GetDummyObjectTransformAnimated(const C3DDummyObject&,CMatrix3x4&,const C3DAnimationBlendState&,const C3DAnimationBlendState&,float,C3DAnimationManager&,bool)const=0;
#endif
private:
 CMovableResourceMemoryPool* MemoryPool;
 long StatsMemoryHandle;
 CCharString Name;
 C3DBoundingSphere BoundingSphere;
 C3DBoundingBox BoundingBox;
 unsigned char HelperPoints[16]; // CPreAllocVectorMap<unsigned long,C3DHelperPoint,...>
 unsigned char DummyObjects[16]; // CPreAllocVectorMap<unsigned long,C3DDummyObject,...>
 unsigned char MeshVolumes[16]; // CVectorMap<unsigned long,C3DMeshVolume,...>
 unsigned char MeshGenerators[12]; // CArray<C3DGeneratorObject>
 char* PackedNames;
 unsigned short PackedNamesSize;
 bool AnimatedFlag;
};
FABLE_STATIC_ASSERT(sizeof(C3DMeshStats)==124);
#endif
