#ifndef FABLE_ENGINE_N3DANIMATIONBUILDER_COUTPUTANIMATION_H
#define FABLE_ENGINE_N3DANIMATIONBUILDER_COUTPUTANIMATION_H
// New header (chunk 2005, row 0x00AB2520 C3DAnimationBuilder::BuildMeshAnimation): Ego_r.pdb N3DAnimationBuilder::CSequenceInfo
// (0x34 bytes: OriginalIndex +0, GeneratedTransform +4 CMatrix3x4; user default constructor ??0CSequenceInfo@N3DAnimationBuilder@@QAE@XZ
// that stores nothing in retail) and N3DAnimationBuilder::COutputAnimation (Sequences CArray<CSequenceInfo> +0, GeneratedBoneCount
// after it: +0xC in retail where the CArray is the 12-byte STLport vector; Ego_r's 16-byte vector puts it at +0x10).
// Retail evidence: BuildMeshAnimation clears +0xC and counts the generated bones in it, resizes +0 through
// ?resize@?$vector@VCSequenceInfo@N3DAnimationBuilder@@...@QAEXIABVCSequenceInfo@N3DAnimationBuilder@@@Z (0x00AB3290, landed) and
// writes OriginalIndex / GeneratedTransform with a 0x34 stride. Both PDB class keys V (the mangled BuildMeshAnimation signature).
#include <stddef.h>
#include "rebuild_abi.h"
#include "engine/CMatrix3x4.h"
#include "engine/CArray.h"
namespace N3DAnimationBuilder {
class CSequenceInfo {
public:
    CSequenceInfo() { }                                  // ??0CSequenceInfo@N3DAnimationBuilder@@QAE@XZ (no stores)
    long       OriginalIndex;                            // +0x00
    CMatrix3x4 GeneratedTransform;                       // +0x04
};
class COutputAnimation {
public:
    CArray<CSequenceInfo> Sequences;                     // +0x00
    unsigned long         GeneratedBoneCount;            // +0x0c (retail)
};
}
FABLE_STATIC_ASSERT(sizeof(N3DAnimationBuilder::CSequenceInfo) == 0x34);
FABLE_STATIC_ASSERT(offsetof(N3DAnimationBuilder::COutputAnimation, GeneratedBoneCount) == 0xc);
#endif
