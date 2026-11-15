#ifndef FABLE_ENGINE_C3DSKELETALMORPH_H
#define FABLE_ENGINE_C3DSKELETALMORPH_H
// New header (wfA5, rows 0x00AA0FA0 / 0x00AA2110): C3DSkeletalMorph and C3DSkeletalMorphTarget (no repo header; the landed rows
// 0x00A45260 PeekCombinedMorph / 0x00A44CD0 GetBoneScaling model them locally). Declarations only, layouts not modelled.
// Ego_r.pdb / FableWin.pdb: ?PeekCombinedMorph@C3DSkeletalMorph@@QBE?AV?$CCountedPointer@$$CBVC3DSkeletalMorphTarget@@@@XZ (0x00A45260),
// ?GetBoneScaling@C3DSkeletalMorphTarget@@QBEXAAVC3DVector@@K@Z (0x00A44CD0).
#include "engine/CCountedPointer.h"
class C3DVector;
class C3DSkeletalMorphTarget {
public:
    void GetBoneScaling(C3DVector& scaling, unsigned long bone_name_crc) const;      // 0x00A44CD0
};
class C3DSkeletalMorph {
public:
    CCountedPointer<const C3DSkeletalMorphTarget> PeekCombinedMorph() const;           // 0x00A45260
};
#endif
