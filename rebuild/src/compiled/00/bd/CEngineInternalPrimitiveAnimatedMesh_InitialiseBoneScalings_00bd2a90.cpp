// CEngineInternalPrimitiveAnimatedMesh::InitialiseBoneScalings(const C3DMesh2& mesh) const -- when the animation's start blend
// state has a skeletal morph, take its combined morph target, size this mesh's cached bone-scaling array (Cache.BoneScalings keyed
// by the mesh ID) to the bone count and fill each entry from the target by the bone's name CRC.
// Retail: Fable.exe (TLC, VC7.1) @ 0x00BD2A90, 218 bytes (+ int3 padding). __fastcall, `ret 4`.
// Leaf: ?InitialiseBoneScalings@CEngineInternalPrimitiveAnimatedMesh@@QBEXABVC3DMesh2@@@Z
// Identity: Ego_r.pdb / FableWin.pdb ?InitialiseBoneScalings@CEngineInternalPrimitiveAnimatedMesh@@QBEXABVC3DMesh2@@@Z (public const);
//   the only caller 0x00B83798 is GetBoneScalings' find-miss path (find, InitialiseBoneScalings(mesh), find again), matching
//   ?GetBoneScalings@CEngineInternalPrimitiveAnimatedMesh@@QBEPBVC3DVector@@ABVC3DMesh2@@@Z.
// Callees: C3DSkeletalMorph::PeekCombinedMorph() const 0x00A45260 (CCountedPointer<const C3DSkeletalMorphTarget> by hidden pointer;
//   landed), std::map<unsigned long,CArray<C3DVector> >::operator[] 0x00BD9110, std::vector<C3DVector>::resize(unsigned, const
//   C3DVector&) 0x0075C040 (STLport's one-argument resize(n) inline, its C3DVector() temporary left uninitialised),
//   C3DSkeletalMorphTarget::GetBoneScaling(C3DVector&, unsigned long) const 0x00A44CD0 (landed); the counted pointer's release
//   inline (DeleteFunc through the info block, operator delete 0x00BFE9BC).
// Layout: engine/CEngineInternalPrimitiveAnimatedMesh.h ORIGINAL_CACHE (Anim +0x70: StartState +0x74, its CSharedData pointer +0x78;
//   CSharedData::SkeletalMorph +0x18; Cache +0xD4), engine/C3DMesh2.h (BoneCount +0x98, Bones +0x9C, MeshID +0xE8).
// Shape: HasSkeletalMorph() and PeekSkeletalMorph() are separate inline accessors (FableWin.pdb names), so the shared-data pointer
//   is read twice (0x00BD2A96 / 0x00BD2AAC). The key is GetMeshID()'s temporary. STLport's in-class map::operator[] body is visible
//   (FABLE_RETAIL_STLP_MAP) and kept out of line: VC7.1 then knows the key temporary is only read and packs it into the dead
//   `mesh` parameter slot (0x00BD2ADE `mov [esp+30h],edx`), with the bone count spilled to its own local; with operator[] declared
//   only, the two slots swap (218v218).
// Header opt-ins (chunk Q32 overlay): FABLE_RETAIL_CENGINEINTERNALPRIMITIVEANIMATEDMESH_BONESCALINGS (declaration + mutable Cache),
//   FABLE_RETAIL_C3DANIMATIONBLENDSTATE_SKELETALMORPH (HasSkeletalMorph / PeekSkeletalMorph), FABLE_RETAIL_C3DMESH2_BONES
//   (GetBoneCount / GetBone); GetMeshID from the existing FABLE_RETAIL_C3DANIMATIONINFO_BUILD.
#define FABLE_RETAIL_CENGINEINTERNALPRIMITIVEBASE_ATTACHED
#define FABLE_RETAIL_CENGINEPRIMITIVEHANDLE
#define FABLE_RETAIL_CENGINEPRIMITIVEHANDLE_DTOR
#define FABLE_RETAIL_CENGINEPERSISTENTPRIMITIVEBASE
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CIVCOUNTEDPOINTER
#define FABLE_RETAIL_CIVCOUNTEDPOINTER_CONVERT
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCHARSTRING_GETCRC_INLINE
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_CENGINEINTERNALPRIMITIVEOCCLUSIONBASE
#define FABLE_RETAIL_CENGINEINTERNALPRIMITIVEOCCLUSIONBASE_RESETOCCLUSIONINFO
#define FABLE_RETAIL_CMESHDATABANK_BASES
#define FABLE_RETAIL_CBANKFILE_ENTRYQUERY
#define FABLE_RETAIL_CMESHDATABANKENTRY
#define FABLE_RETAIL_CMESHDATABANK_GETANIMATIONMANAGER
#define FABLE_RETAIL_C3DMESHINFO_GETMESHSTATS
#define FABLE_RETAIL_C3DMESHINFO_LOADSTATS
#define FABLE_RETAIL_CMOVABLERESOURCE
#define FABLE_RETAIL_C3DMESHSTATS_DUMMYOBJECTS
#define FABLE_RETAIL_CMATRIX3X4
#define FABLE_RETAIL_CENGINEINTERNALPRIMITIVEANIMATEDMESH_ORIGINAL_CACHE
#define FABLE_RETAIL_C3DANIMATIONBLENDSTATE_TYPED_DATA
#define FABLE_RETAIL_C3DANIMATIONBLENDSTATE_SHAREDDATA
#define FABLE_RETAIL_C3DANIMATIONBLENDSTATE_SKELETALMORPH
#define FABLE_RETAIL_CENGINEINTERNALPRIMITIVEANIMATEDMESH_BONESCALINGS
#define FABLE_RETAIL_C3DANIMATIONINFO_BUILD
#define FABLE_RETAIL_C3DMESH2_BONES
#define FABLE_RETAIL_STLP_MAP
// C3DBone2 (Ego_r.pdb, 60 bytes; no shared header): defined before C3DMesh2.h, whose inline GetBone indexes it.
class C3DBone2 {
public:
    unsigned long GetNameCRC() const { return NameCRC; }                // Ego_r ?GetNameCRC@C3DBone2@@QBEKXZ (inline)
    unsigned long NameCRC;                                               // +0x00
    long Parent;                                                         // +0x04
    long OriginalNoChildren;                                             // +0x08
    unsigned char LocalizationMatrix[48];                                // +0x0C CMatrix3x4
};
#include "rebuild_abi.h"
#include "engine/CIVCountedPointeeBase.h"
#include "engine/CIVCountedPointer.h"
#include "engine/CCountedPointer.h"
#include "engine/C3DVector.h"
#include "engine/C3DAnimationBlendState.h"
#include "engine/CEngineInternalPrimitiveAnimatedMesh.h"
#include "engine/C3DMesh2.h"

class C3DSkeletalMorphTarget { public:
    void GetBoneScaling(C3DVector& scaling, unsigned long boneNameCRC) const;   // 0x00A44CD0
};
class C3DSkeletalMorph { public:
    CCountedPointer<const C3DSkeletalMorphTarget> PeekCombinedMorph() const;   // 0x00A45260
};

void CEngineInternalPrimitiveAnimatedMesh::InitialiseBoneScalings(const C3DMesh2& mesh) const
{
    if (Anim.StartState.HasSkeletalMorph()) {
        CCountedPointer<const C3DSkeletalMorphTarget> target = Anim.StartState.PeekSkeletalMorph().PeekCombinedMorph();
        unsigned long boneCount = mesh.GetBoneCount();
        CArray<C3DVector>& scalings = Cache.BoneScalings[mesh.GetMeshID()];
        scalings.resize(boneCount);
        for (unsigned long i = 0; i < boneCount; ++i)
            target->GetBoneScaling(scalings[i], mesh.GetBone(i)->GetNameCRC());
    }
}