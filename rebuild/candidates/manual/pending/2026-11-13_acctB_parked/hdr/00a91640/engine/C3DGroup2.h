#ifndef FABLE_ENGINE_C3DGROUP2_H
#define FABLE_ENGINE_C3DGROUP2_H
#ifdef FABLE_RETAIL_C3DGROUP2
// New header (chunk 1017, row 0x00AE3920 CParticleProgram::RemapBoneIndices): one skinning group of a particle program / mesh.
// Ego_r.pdb (struct_layouts_egor.tsv) BoneIndex long +0x00, VertexBlends CArray<C3DVertexBlend2> +0x04: 16 bytes in retail (the
// STLport vector is 12; the PDB's 20 is the Dinkumware 16). Member functions: Ego_r.pdb / FableWin.pdb ?SetBoneIndex@C3DGroup2@@QAEXJ@Z
// (out of line, retail 0x00AC4070, bound by the landed CParticleProgram::AddGroup 0x00AE37D0), ?GetBoneIndex@C3DGroup2@@QBEJXZ (inline).
// The implicit copy constructor / destructor are the member-wise ones (the vector's).
#include "engine/CArray.h"
#include "engine/C3DVertexBlend2.h"
#ifdef FABLE_RETAIL_C3DPRIMITIVE2_LOADER
class C3DMeshFileGroupChunk;
#endif
class C3DGroup2 {
public:
#ifdef FABLE_RETAIL_C3DPRIMITIVE2_LOADER
    C3DGroup2(const C3DMeshFileGroupChunk& chunk);           // 0x00AC4150
#endif
    long GetBoneIndex() const { return BoneIndex; }
    void SetBoneIndex(long bone_index);                      // 0x00AC4070
private:
    long BoneIndex;                                          // +0x00
    CArray<C3DVertexBlend2> VertexBlends;                    // +0x04
};
FABLE_STATIC_ASSERT(sizeof(C3DGroup2) == 0x10);
#endif
#endif // FABLE_ENGINE_C3DGROUP2_H
