#ifndef FABLE_ENGINE_C3DMESHSTATS_H
#define FABLE_ENGINE_C3DMESHSTATS_H
#include "rebuild_abi.h"
#include "engine/CMovableResource.h"
#include "engine/C3DBoundingSphere.h"
#include "engine/C3DBoundingBox.h"
#include "fable_string.h"
class CMovableResourceMemoryPool;
#ifdef FABLE_RETAIL_C3DMESHSTATS_INITIALISEFROMFILEDATA
// Retail opt-in (wfE8, row 0x00A2F740 C3DMeshStats::InitialiseFromFileData): the three vector maps typed (Ego_r.pdb
// struct_layouts_egor.tsv: CPreAllocVectorMap<unsigned long,C3DHelperPoint> +0x38, CPreAllocVectorMap<unsigned long,C3DDummyObject>
// +0x48, CVectorMap<unsigned long,C3DMeshVolume> +0x58, 16 bytes each; no layout change) and the loader with its private helpers
// (Ego_r.pdb ?InitialiseFromFileData@C3DMeshStats@@QAE_NABVCCharString@@ABVC3DMeshFile@@_NPAVCMovableResourceMemoryPool@@@Z,
// ?AllocateBuffers@C3DMeshStats@@AAE_NKKK@Z 0x00A2E760, ?PackNames@C3DMeshStats@@AAEXAAV?$CArray@D@@ABV?$set@VCCharString@@..@std@@1@Z
// 0x00A2EEF0). Needs FABLE_RETAIL_C3DMESHSTATS_GENERATORS.
#ifndef FABLE_RETAIL_C3DMESHSTATS_GENERATORS
#error FABLE_RETAIL_C3DMESHSTATS_INITIALISEFROMFILEDATA needs FABLE_RETAIL_C3DMESHSTATS_GENERATORS
#endif
#include "stlport/fable_map.h"
#include "engine/C3DHelperPoint.h"
#include "engine/C3DDummyObject.h"
#include "engine/C3DMeshVolume.h"
#include "engine/CPreAllocVectorMap.h"
#include "engine/CVectorMap.h"
class C3DMeshFile;
#endif
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
#ifdef FABLE_RETAIL_C3DMESHSTATS_UNPACKNAMES
 // Retail opt-in (NF214, q12343: rows 0x00A2F0F0 / 0x00A2F120 / 0x00A2F150): the packed-name queries, both PDBs (Ego_r.pdb
 // 0x00B8E920.. / FableWin.pdb 0x031E4B50..): ?GetAllDummyObjectNames@C3DMeshStats@@QBEXAAV?$vector@VCCharString@@..@std@@_N@Z,
 // ?GetAllHelperPointNames@..QBE..._N@Z, ?GetDummyObjectNamesStartingWith@..QBE...ABVCCharString@@_N@Z, ?GetHelperPointNamesStartingWith@
 // ..QBE...ABVCCharString@@_N@Z (public const) and the private const worker ?UnpackNames@C3DMeshStats@@ABEXAAV?$vector@VCCharString@@
 // V?$allocator@VCCharString@@@std@@@std@@_NABVCCharString@@1@Z (`ABE`). Declarations only, no layout change; public again after.
 void GetAllDummyObjectNames(std::vector<CCharString>& names,bool sort)const;                                      // 0x00A2F0F0
 void GetAllHelperPointNames(std::vector<CCharString>& names,bool sort)const;                                      // 0x00A2F120
 void GetDummyObjectNamesStartingWith(std::vector<CCharString>& names,const CCharString& prefix,bool sort)const;   // 0x00A2F150
 void GetHelperPointNamesStartingWith(std::vector<CCharString>& names,const CCharString& prefix,bool sort)const;   // 0x00A2F170
private:
 void UnpackNames(std::vector<CCharString>& names,bool dummy_objects,const CCharString& prefix,bool sort)const;   // 0x00A2ED10
public:
#endif
#ifdef FABLE_RETAIL_C3DMESHSTATS_BOUNDINGSPHERE
 // Opt-in (row 0x00B8B780): FableWin.pdb ?GetBoundingSphere@C3DMeshStats@@QBE?AVC3DBoundingSphere@@XZ (inline, by value).
 C3DBoundingSphere GetBoundingSphere() const { return BoundingSphere; }
#endif
#ifdef FABLE_RETAIL_C3DMESHSTATS_LIFETIME_PASS23
 // Original user lifetime methods, both PDBs; declarations only, no layout change.
 C3DMeshStats();virtual ~C3DMeshStats();
#endif
 bool HasDummyObject(unsigned long)const;
#ifdef FABLE_RETAIL_C3DMESHSTATS_HASHELPERPOINT
 // Retail opt-in (agentNF41, row 0x00A36100 C3DMeshObject::HasHelperPoint): Ego_r.pdb ?HasHelperPoint@C3DMeshStats@@QBE_NK@Z (Ego_r
 // 0x00B8E300, directly before HasDummyObject 0x00B8E340 as retail 0x00A2EA20 / 0x00A2EA60; landed row 0x00A2EA20). Declaration only.
 bool HasHelperPoint(unsigned long)const;                    // 0x00A2EA20
#endif
#ifdef FABLE_RETAIL_C3DMESHSTATS_LOADBINARY
 // Retail opt-in (Codex pass22): both PDB public reference signature, native A2F2D0.
 bool LoadBinary(class CMemoryDataInputStream&);
#endif
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
#ifdef FABLE_RETAIL_C3DMESHOBJECT_DUMMY_PAIR_PASS24
 const std::pair<unsigned long, C3DDummyObject>* FindDummyObjectsBegin(unsigned long name_crc) const;
#endif
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
#ifdef FABLE_RETAIL_C3DMESHSTATS_INITIALISEFROMFILEDATA
 bool InitialiseFromFileData(const CCharString& name,const C3DMeshFile& file,bool animated,CMovableResourceMemoryPool* pool);   // 0x00A2F740
private:
 bool AllocateBuffers(unsigned long helper_points,unsigned long dummy_objects,unsigned long packed_names_size);   // 0x00A2E760
 void PackNames(CArray<char>& packed,const std::set<CCharString>& helper_point_names,const std::set<CCharString>& dummy_object_names);   // 0x00A2EEF0
#endif
#if defined(FABLE_RETAIL_C3DMESHSTATS_LOADBINARY) || defined(FABLE_RETAIL_C3DMESHSTATS_PROTECTED_POOL)
protected: // Both original PDBs protect the stats data range; native first field +4.
// FABLE_RETAIL_C3DMESHSTATS_PROTECTED_POOL (wfE8 6th agent, row 0x00A8B990): the derived C3DMesh2's own members read the pool
// (PackBoneNames 0x00A8B6C0 `mov ecx,[esi+4]` before AllocateResource, InitialiseBoneNamePointers 0x00A892E0), so the original access
// is protected, not private. Access only: no layout or codegen change.
#else
private:
#endif
 CMovableResourceMemoryPool* MemoryPool;
 long StatsMemoryHandle;
 CCharString Name;
 C3DBoundingSphere BoundingSphere;
 C3DBoundingBox BoundingBox;
 #ifdef FABLE_RETAIL_C3DMESHSTATS_INITIALISEFROMFILEDATA
 CPreAllocVectorMap<unsigned long,C3DHelperPoint> HelperPoints;   // +0x38
 CPreAllocVectorMap<unsigned long,C3DDummyObject> DummyObjects;   // +0x48
 CVectorMap<unsigned long,C3DMeshVolume> MeshVolumes;             // +0x58
 #else
 unsigned char HelperPoints[16]; // CPreAllocVectorMap<unsigned long,C3DHelperPoint,...>
 unsigned char DummyObjects[16]; // CPreAllocVectorMap<unsigned long,C3DDummyObject,...>
 unsigned char MeshVolumes[16]; // CVectorMap<unsigned long,C3DMeshVolume,...>
 #endif
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
