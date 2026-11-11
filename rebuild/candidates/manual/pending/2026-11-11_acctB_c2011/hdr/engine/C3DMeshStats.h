#ifndef FABLE_ENGINE_C3DMESHSTATS_H
#define FABLE_ENGINE_C3DMESHSTATS_H
#include "rebuild_abi.h"
#include "engine/CMovableResource.h"
#include "engine/C3DBoundingSphere.h"
#include "engine/C3DBoundingBox.h"
#include "fable_string.h"
#ifdef FABLE_RETAIL_C3DMESHSTATS_GENERATORS
#include "engine/CArray.h"
#include "engine/C3DGeneratorObject.h"
class C3DHelperPoint;
class C3DAnimationBlendState;
class C3DAnimationManager;
#endif
class CMovableResourceMemoryPool;
// Full original Ego_r0x3580 data range, native STLport maps/arrays. External declaration only.
class C3DMeshStats: public CMovableResource {
public:
 bool HasDummyObject(unsigned long)const;
#ifdef FABLE_RETAIL_C3DMESHSTATS_GENERATORS
 // Overlay opt-in (chunk 2011, rows 0x00BF12A0 / 0x00BF18B0 CEnginePrimitiveRenderUtil::AddGeneratedObjects); needs
 // FABLE_RETAIL_CMOVABLERESOURCE. MeshGenerators typed (CArray<C3DGeneratorObject>, +0x68: the rows walk [stats+68h] .. [stats+6Ch]);
 // Ego_r.pdb / FableWin.pdb ?GetMeshGeneratorsBegin@C3DMeshStats@@QBE...XZ / ?GetMeshGeneratorsEnd (inline in retail: begin read
 // once, end re-read every iteration, 0x00BF12D0 / 0x00BF1828; the PDBs' Dinkumware const_iterator is STLport's const T*).
 // Vtable 0x0129CF04 slots 4..9 are _purecall 0x00BFEA08: the six C3DMesh2 overrides (rtti_bases: C3DMesh2 : C3DMeshStats; vtable
 // 0x0129DEF0 slots 4..9) are pure here. Slots by `ret N` of the C3DMesh2 bodies: 4 GetHelperPointPositionAnimated (blend pair,
 // ret 18h) / 5 (one blend, ret 10h), 6 GetDummyObjectPositionAnimated (blend pair, ret 1Ch; writes the out object's BoneIndex
 // +0x30, 0x00A89E56) / 7 (one blend, ret 14h), 8 GetDummyObjectTransformAnimated (blend pair) / 9 (one blend): VC7.1 gives the
 // later-declared overload of a set the earlier slot, so each set is declared one-blend first.
 virtual C3DVector GetHelperPointPositionAnimated(const C3DHelperPoint& helper_point,const C3DAnimationBlendState& blend_state,C3DAnimationManager& animation_manager)const=0;
 virtual C3DVector GetHelperPointPositionAnimated(const C3DHelperPoint& helper_point,const C3DAnimationBlendState& blend_state_start,const C3DAnimationBlendState& blend_state_end,float interpolate,C3DAnimationManager& animation_manager)const=0;
 virtual void GetDummyObjectPositionAnimated(const C3DDummyObject& dummy_object,C3DDummyObject& result,const C3DAnimationBlendState& blend_state,C3DAnimationManager& animation_manager,bool flag)const=0;
 virtual void GetDummyObjectPositionAnimated(const C3DDummyObject& dummy_object,C3DDummyObject& result,const C3DAnimationBlendState& blend_state_start,const C3DAnimationBlendState& blend_state_end,float interpolate,C3DAnimationManager& animation_manager,bool flag)const=0;
 virtual void GetDummyObjectTransformAnimated(const C3DDummyObject& dummy_object,CMatrix3x4& result,const C3DAnimationBlendState& blend_state,C3DAnimationManager& animation_manager,bool flag)const=0;
 virtual void GetDummyObjectTransformAnimated(const C3DDummyObject& dummy_object,CMatrix3x4& result,const C3DAnimationBlendState& blend_state_start,const C3DAnimationBlendState& blend_state_end,float interpolate,C3DAnimationManager& animation_manager,bool flag)const=0;
 const C3DGeneratorObject* GetMeshGeneratorsBegin()const{return MeshGenerators.begin();}
 const C3DGeneratorObject* GetMeshGeneratorsEnd()const{return MeshGenerators.end();}
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
#ifdef FABLE_RETAIL_C3DMESHSTATS_GENERATORS
 CArray<C3DGeneratorObject> MeshGenerators;
#else
 unsigned char MeshGenerators[12]; // CArray<C3DGeneratorObject>
#endif
 char* PackedNames;
 unsigned short PackedNamesSize;
 bool AnimatedFlag;
};
FABLE_STATIC_ASSERT(sizeof(C3DMeshStats)==124);
#endif
