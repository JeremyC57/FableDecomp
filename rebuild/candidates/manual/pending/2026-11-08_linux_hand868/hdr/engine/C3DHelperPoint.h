#ifndef FABLE_ENGINE_C3DHELPERPOINT_H
#define FABLE_ENGINE_C3DHELPERPOINT_H
// C3DHelperPoint (chunk 868, row 0x00A2F740): Ego_r.pdb struct_layouts_egor.tsv -- C3DVector Position +0x0, long BoneIndex +0xC,
// sizeof 0x10 (the retail pre-allocated map stores pair<unsigned long, C3DHelperPoint>, 0x14 bytes: landed 0x00A2E760 AllocateBuffers).
// FableWin.pdb: ??0C3DHelperPoint@@QAE@ABVC3DVector@@K@Z, ??0C3DHelperPoint@@QAE@ABV0@@Z, ?GetPosition@C3DHelperPoint@@QBE?AVC3DVector@@XZ,
// ?GetBoneIndex@C3DHelperPoint@@QBEJXZ -- all inline in retail (no Ego_r or Fable.exe out-of-line copy; 0x00A2F858..0x00A2F871 builds one
// in place from the helper-point chunk's position and bone index).
#include "rebuild_abi.h"
#include "engine/C3DVector.h"
class C3DHelperPoint {
public:
    C3DHelperPoint() { }
    C3DHelperPoint(const C3DVector& position, unsigned long boneIndex) : Position(position), BoneIndex((long)boneIndex) { }
    C3DVector GetPosition() const { return Position; }
    long GetBoneIndex() const { return BoneIndex; }
private:
    C3DVector Position;                                                  // +0x00
    long BoneIndex;                                                      // +0x0C
};
FABLE_STATIC_ASSERT(sizeof(C3DHelperPoint) == 0x10);
#endif // FABLE_ENGINE_C3DHELPERPOINT_H
