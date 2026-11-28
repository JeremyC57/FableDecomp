#ifndef FABLE_ENGINE_CENGINEINTERNALPRIMITIVEANIMATEDMESH_H
#define FABLE_ENGINE_CENGINEINTERNALPRIMITIVEANIMATEDMESH_H
// Retail opt-in (Codex pass18): all original fields preserved with native complete sizes.
#include "engine/CEngineInternalPrimitiveMeshBase.h"
#ifdef FABLE_RETAIL_CENGINEINTERNALPRIMITIVEANIMATEDMESH_WORLDPOS
#ifndef FABLE_RETAIL_C3DPOSITIONINTERPOLATOR
#define FABLE_RETAIL_C3DPOSITIONINTERPOLATOR
#endif
#include "engine/C3DPositionInterpolator.h"
#endif
#ifdef FABLE_RETAIL_CENGINEINTERNALPRIMITIVEANIMATEDMESH_ORIGINAL_CACHE
#include "engine/C3DAnimationInterpolator.h"
#include "engine/C3DOrientationInterpolator.h"
#include "engine/CEngineRenderInfoAnimatedMesh.h"
#include "engine/CArray.h"
#include "stlport/fable_map.h"
#endif
#ifdef FABLE_RETAIL_CENGINEINTERNALPRIMITIVEANIMATEDMESH_ANIM
#include "engine/C3DAnimationInterpolator.h"
#endif
class C3DMesh2;
class CEngineInternalPrimitiveAnimatedMesh:public CEngineInternalPrimitiveMeshBase {
public:

#ifdef FABLE_RETAIL_PASS22_ANIMATED_MESH_RENDER
// Original two PDBs, donor02E82300 and native00BD71B0; evidence in00bd71cc.
#ifdef FABLE_RETAIL_DEBUG_BONE_PASS29
 const CEngineRenderInfoAnimatedMesh* PeekRenderInfo() const { return &RenderInfo; }
#endif
 CEngineRenderInfoAnimatedMesh* GetRenderInfo(){return &RenderInfo;}
 bool SetupMesh(float,float);
#endif
#ifdef FABLE_RETAIL_CENGINEINTERNALPRIMITIVEANIMATEDMESH_CTOR_PASS30
 // FableWin names the public default constructor; the native constructor calls the base with type 9 and mask 10.
 CEngineInternalPrimitiveAnimatedMesh();
#endif
 virtual void CalcInterpolatedObjectTransform(CMatrix3x4&,float,bool)const;
#ifdef FABLE_RETAIL_CENGINEINTERNALPRIMITIVEANIMATEDMESH_ISINSIDEFRUSTUM
 // Opt-in (egod e07, row 0x00BCF040): Ego_r.pdb ?IsInsideFrustum@CEngineInternalPrimitiveAnimatedMesh@@UAE_NABVCEngineCamera@@_N@Z, the override
 // of the CEngineInternalPrimitiveBase virtual (same slot). Declaration only; no layout change.
 virtual bool IsInsideFrustum(const CEngineCamera& frustum,bool distance_test_only);
#endif
 virtual void InitialiseSimulation(const C3DMeshInfo*,const C3DMesh2&);
#ifdef FABLE_RETAIL_CENGINEINTERNALPRIMITIVEANIMATEDMESH_SETUPMESH2D
 // Retail opt-in (agentNF77, row 0x00BD1F70 CEnginePrimitiveManager2DAnimatedMeshes::Render2DPrimitive): Ego_r.pdb / FableWin.pdb
 // ?SetupMesh2D@CEngineInternalPrimitiveAnimatedMesh@@QAE_NXZ (public; out of line in retail at 0x00BCF430: `mov ecx,ebp ; call`, 0x00BD2111).
 // Declaration only: no layout change.
 bool SetupMesh2D();                                                     // 0x00BCF430
#endif
#ifdef FABLE_RETAIL_CENGINEINTERNALPRIMITIVEANIMATEDMESH_BONESCALINGS
 // Retail opt-in (chunk Q32, row 0x00BD2A90): Ego_r.pdb / FableWin.pdb ?InitialiseBoneScalings@CEngineInternalPrimitiveAnimatedMesh@@QBEXABVC3DMesh2@@@Z
 // (public const; retail 0x00BD2A90, called from 0x00B83798). Being const, it fills the Cache through a mutable member (below).
 void InitialiseBoneScalings(const C3DMesh2&) const;
#endif
#if defined(FABLE_RETAIL_CENGINEINTERNALPRIMITIVEANIMATEDMESH_ORIGINAL_CACHE) || defined(FABLE_RETAIL_CENGINEINTERNALPRIMITIVEANIMATEDMESH_ANIM)
 // FABLE_RETAIL_CENGINEINTERNALPRIMITIVEANIMATEDMESH_ANIM: retail opt-in (chunk 2011, row 0x00BF18B0): the member typed alone (+0x70;
 // the row passes &Anim.StartState +0x74 / &Anim.EndState +0x7C, 0x00BF1950 / 0x00BF1954). No layout change.
 C3DAnimationInterpolator Anim;
#else
 unsigned char Anim[20]; // Original C3DAnimationInterpolator
#endif
#ifdef FABLE_RETAIL_CENGINEINTERNALPRIMITIVEANIMATEDMESH_WORLDPOS
 // Opt-in (chunk 1020, row 0x00BCF4F0): the member with its Ego_r type (same 28 bytes at +0x84).
 C3DPositionInterpolator WorldPos;
#else
 unsigned char WorldPos[28]; // Original C3DPositionInterpolator
#endif
#ifdef FABLE_RETAIL_CENGINEINTERNALPRIMITIVEANIMATEDMESH_ORIGINAL_CACHE
 C3DOrientationInterpolator Orientation;
 // Both PDBs: CCache owns only map<unsigned long,CArray<C3DVector> > BoneScalings.
 // Native STLport map12 shrinks Cache+0xd4 to12, placing full RenderInfo72 at0xe0.
 class CCache {public:std::map<unsigned long,CArray<C3DVector> > BoneScalings;};
 // (FABLE_RETAIL_CENGINEINTERNALPRIMITIVEANIMATEDMESH_BONESCALINGS: the const InitialiseBoneScalings 0x00BD2A90 inserts into it.)
#ifdef FABLE_RETAIL_CENGINEINTERNALPRIMITIVEANIMATEDMESH_BONESCALINGS
 mutable
#endif
 CCache Cache;
#else
 unsigned char Orientation[52]; // Original C3DOrientationInterpolator
 unsigned char Cache[16]; // Original CCache
#endif
#ifdef FABLE_RETAIL_CENGINEINTERNALPRIMITIVEANIMATEDMESH_GETBONETRANSFORMATIONS
 // Retail opt-in (agentC40, row 0x00BD2D90): Ego_r.pdb / FableWin.pdb ?GetBoneTransformations@CEngineInternalPrimitiveAnimatedMesh@@QAEPBVCMatrix4x4@@XZ
 // and ?GetBoneScalings@CEngineInternalPrimitiveAnimatedMesh@@QBEPBVC3DVector@@ABVC3DMesh2@@@Z (0x00B83750). Declarations only.
 const CMatrix4x4* GetBoneTransformations();
 const C3DVector* GetBoneScalings(const C3DMesh2&) const;
#endif
#if defined(FABLE_RETAIL_CENGINEINTERNALPRIMITIVEANIMATEDMESH_RENDERINFO) && defined(FABLE_RETAIL_CENGINEINTERNALPRIMITIVEANIMATEDMESH_ORIGINAL_CACHE)
 // Retail opt-in (wfA6b, row 0x00BD5C90 CEnginePrimitiveManagerAnimatedMeshesBase::RenderPrimitiveToShadowBuffer): FableWin.pdb
 // ?SetupMesh@CEngineInternalPrimitiveAnimatedMesh@@QAE_NMM@Z (out of line, landed 0x00BCF2E0) and the inline
 // ?GetRenderInfo@CEngineInternalPrimitiveAnimatedMesh@@QAEPAVCEngineRenderInfoAnimatedMesh@@XZ (&RenderInfo; no retail copy: the row
 // reads +0xE4 Mesh and copies +0xEC ObjectTransform in place, 0x00BD5F22..0x00BD5F37). No layout change.
 bool SetupMesh(float distance, float radius);                        // 0x00BCF2E0
 CEngineRenderInfoAnimatedMesh* GetRenderInfo() { return &RenderInfo; }
#endif
private:
#if defined(FABLE_RETAIL_CENGINEINTERNALPRIMITIVEANIMATEDMESH_ORIGINAL_CACHE)
 CEngineRenderInfoAnimatedMesh RenderInfo;
};
FABLE_STATIC_ASSERT_AT(62, sizeof(CEngineInternalPrimitiveAnimatedMesh)==0x128);
#elif defined(FABLE_RETAIL_CENGINEINTERNALPRIMITIVEANIMATEDMESH_SIZE128)
 // Opt-in (chunk 1020, row 0x00BCF4F0): retail's object is 0x128 bytes, 4 fewer than Ego_r's 300 -- CEngineInternalPrimitive2DAnimatedMesh
 // keeps Initialised at +0x128 and Layer at +0x12C (Update 0x00BD2BC0 `mov [esi+128h],bl` / `mov [esi+12Ch],..`; Initialise 0x00BCF4F6
 // `mov eax,[esi+12Ch]`), and CEngineInternalPrimitiveMorphedAnimatedMesh::Update 0x00BD2B70 addresses its own first member at +0x128.
 // OPEN: which member is 4 bytes shorter in retail is not proven (Anim +0x70 / WorldPos +0x84 / Orientation +0xA0 are proven by the
 // landed 0x00BCF670 / 0x00BCF760 / 0x00BCF930); the shortfall is put in this opaque tail, which no landed row reads.
 unsigned char RenderInfo[68]; // Original CEngineRenderInfoAnimatedMesh (retail size unproven, see above)
};
FABLE_STATIC_ASSERT_AT(71, sizeof(CEngineInternalPrimitiveAnimatedMesh)==0x128);
#else
 unsigned char RenderInfo[72]; // Original CEngineRenderInfoAnimatedMesh
};
FABLE_STATIC_ASSERT_AT(75, sizeof(CEngineInternalPrimitiveAnimatedMesh)==300);
#endif
#endif
