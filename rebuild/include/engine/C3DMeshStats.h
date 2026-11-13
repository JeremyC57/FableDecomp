#ifndef FABLE_ENGINE_C3DMESHSTATS_H
#define FABLE_ENGINE_C3DMESHSTATS_H
#include "rebuild_abi.h"
#include "engine/CMovableResource.h"
#include "engine/C3DBoundingSphere.h"
#include "engine/C3DBoundingBox.h"
#include "fable_string.h"
class CMovableResourceMemoryPool;
#ifdef FABLE_RETAIL_C3DMESHSTATS_GENERATORS
#include "engine/CArray.h"
#include "engine/C3DGeneratorObject.h"
#endif
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
 bool HasDummyObject(unsigned long)const;
#ifdef FABLE_RETAIL_C3DMESHSTATS_MESHGENERATORS
 // Retail opt-in (wf22, row 0x00A24520): Ego_r.pdb ?InitialiseMeshGenerators@C3DMeshStats@@QAEXABVCMeshDataBank@@ABVCCharString@@@Z
 // (retail 0x00A2EC40, landed). Declaration only.
 void InitialiseMeshGenerators(const class CMeshDataBank&,const CCharString&);   // 0x00A2EC40
#endif
#ifdef FABLE_RETAIL_C3DMESHSTATS_ISANIMATED
 bool IsAnimated()const { return AnimatedFlag; }   // FableWin.pdb ?IsAnimated@C3DMeshStats@@QBE_NXZ (inline in retail)
#endif
#ifdef FABLE_RETAIL_C3DMESHSTATS_DUMMYOBJECTS
 // Ego_r.pdb ?GetDummyObject@C3DMeshStats@@QBEPBVC3DDummyObject@@KK@Z (retail 0x00A2E920). The six virtuals are Ego_r.pdb / FableWin.pdb
 // C3DMesh2 overrides (UBE); declared here in source order so VC's overload grouping gives retail's C3DMesh2 slots: 4/5
 // GetHelperPointPositionAnimated (0x00A898D0 / 0x00A89740), 6/7 GetDummyObjectPositionAnimated (two-state 0x00A89E40 / one-state
 // 0x00A89A70), 8/9 GetDummyObjectTransformAnimated (two-state 0x00A8A5F0, called through +0x20 at 0x00B39141 / one-state 0x00A8A220).
 const C3DDummyObject* GetDummyObject(unsigned long name_crc,unsigned long index)const;   // 0x00A2E920
 virtual C3DVector GetHelperPointPositionAnimated(const C3DHelperPoint&,const C3DAnimationBlendState&,C3DAnimationManager&)const=0;
 virtual C3DVector GetHelperPointPositionAnimated(const C3DHelperPoint&,const C3DAnimationBlendState&,const C3DAnimationBlendState&,float,C3DAnimationManager&)const=0;
 // The out parameter as both PDBs spell it (chunk 2011, row 0x00BF18B0 CEnginePrimitiveRenderUtil::AddGeneratedObjects),
 // ?GetDummyObjectPositionAnimated@C3DMesh2@@UBEXABVC3DDummyObject@@AAV2@ABVC3DAnimationBlendState@@2MAAVC3DAnimationManager@@_N@Z
 // (`AAV2@` = C3DDummyObject&; the override 0x00A89E40 writes the out object's BoneIndex +0x30, 0x00A89E56). Same slots (6 / 7).
 virtual void GetDummyObjectPositionAnimated(const C3DDummyObject& dummy_object,C3DDummyObject& result,const C3DAnimationBlendState& blend_state,C3DAnimationManager& animation_manager,bool flag)const=0;
 virtual void GetDummyObjectPositionAnimated(const C3DDummyObject& dummy_object,C3DDummyObject& result,const C3DAnimationBlendState& blend_state_start,const C3DAnimationBlendState& blend_state_end,float interpolate,C3DAnimationManager& animation_manager,bool flag)const=0;
 virtual void GetDummyObjectTransformAnimated(const C3DDummyObject&,CMatrix3x4&,const C3DAnimationBlendState&,C3DAnimationManager&,bool)const=0;
 virtual void GetDummyObjectTransformAnimated(const C3DDummyObject&,CMatrix3x4&,const C3DAnimationBlendState&,const C3DAnimationBlendState&,float,C3DAnimationManager&,bool)const=0;
#endif
#ifdef FABLE_RETAIL_C3DMESHSTATS_GENERATORS
 // Retail opt-in (chunk 2011, rows 0x00BF12A0 / 0x00BF18B0 CEnginePrimitiveRenderUtil::AddGeneratedObjects): MeshGenerators typed
 // (CArray<C3DGeneratorObject>, +0x68: the rows walk [stats+68h] .. [stats+6Ch]); Ego_r.pdb / FableWin.pdb
 // ?GetMeshGeneratorsBegin@C3DMeshStats@@QBE...XZ / ?GetMeshGeneratorsEnd (inline in retail: begin read once, end re-read every
 // iteration, 0x00BF12D0 / 0x00BF1828; the PDBs' Dinkumware const_iterator is STLport's const T*). No layout change.
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
FABLE_STATIC_ASSERT_AT(86, sizeof(C3DMeshStats)==124);
#endif
