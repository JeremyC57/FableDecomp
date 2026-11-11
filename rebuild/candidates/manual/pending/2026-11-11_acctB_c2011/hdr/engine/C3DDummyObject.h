#ifndef FABLE_ENGINE_C3DDUMMYOBJECT_H
#define FABLE_ENGINE_C3DDUMMYOBJECT_H
// Overlay header (chunk 2011, rows 0x00BF12A0 / 0x00BF18B0 CEnginePrimitiveRenderUtil::AddGeneratedObjects).
// C3DDummyObject (Ego_r.pdb, 52 bytes, no vtable): Transform (CMatrix3x4) +0x00, BoneIndex (long) +0x30. Members and methods from
// Ego_r.pdb / FableWin.pdb: ??0C3DDummyObject@@QAE@XZ is out of line in retail at 0x00A2E4B0 (identity Transform, BoneIndex left
// unset; the manifest label C3DDummyObject::C3DDummyObject, called by AddGeneratedObjects 0x00BF1928 on the local handed to
// C3DMeshStats::GetDummyObjectPositionAnimated as its C3DDummyObject& out); ?PeekTransform@C3DDummyObject@@QBEABVCMatrix3x4@@XZ and
// ?GetBoneIndex@C3DDummyObject@@QBEJXZ are inline (FableWin.pdb only).
#include "engine/CMatrix3x4.h"
class C3DDummyObject {
public:
    C3DDummyObject();                                                    // 0x00A2E4B0
    const CMatrix3x4& PeekTransform() const { return Transform; }
    long GetBoneIndex() const { return BoneIndex; }
private:
    CMatrix3x4 Transform;                                                // +0x00
    long BoneIndex;                                                      // +0x30
};
FABLE_STATIC_ASSERT(sizeof(C3DDummyObject) == 0x34);
#endif
