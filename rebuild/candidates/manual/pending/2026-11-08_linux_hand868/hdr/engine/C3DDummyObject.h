#ifndef FABLE_ENGINE_C3DDUMMYOBJECT_H
#define FABLE_ENGINE_C3DDUMMYOBJECT_H
// C3DDummyObject (chunk 868, row 0x00A2F740): Ego_r.pdb struct_layouts_egor.tsv -- CMatrix3x4 Transform +0x0, long BoneIndex +0x30,
// sizeof 0x34 (the retail pre-allocated map stores pair<unsigned long, C3DDummyObject>, 0x38 bytes: landed 0x00A2E760 AllocateBuffers).
// FableWin.pdb: ??0C3DDummyObject@@QAE@ABVCMatrix3x4@@K@Z, ??0C3DDummyObject@@QAE@XZ, ?PeekTransform@C3DDummyObject@@QBEABVCMatrix3x4@@XZ,
// ?GetPosition@C3DDummyObject@@QBE?AVC3DVector@@XZ (the translation row) -- inline in retail (0x00A2FA30..0x00A2FA4C builds one in place:
// the 12-dword `rep movsd` of the chunk's transform, then the bone index).
#include "rebuild_abi.h"
#include "engine/C3DVector.h"
#include "engine/CMatrix3x4.h"
class C3DDummyObject {
public:
    C3DDummyObject() { }
    C3DDummyObject(const CMatrix3x4& transform, unsigned long boneIndex) : Transform(transform), BoneIndex((long)boneIndex) { }
    const CMatrix3x4& PeekTransform() const { return Transform; }
    C3DVector GetPosition() const { return C3DVector(Transform.E41, Transform.E42, Transform.E43); }
    long GetBoneIndex() const { return BoneIndex; }
private:
    CMatrix3x4 Transform;                                                // +0x00
    long BoneIndex;                                                      // +0x30
};
FABLE_STATIC_ASSERT(sizeof(C3DDummyObject) == 0x34);
#endif // FABLE_ENGINE_C3DDUMMYOBJECT_H
