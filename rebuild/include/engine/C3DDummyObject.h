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
#if defined(FABLE_RETAIL_PHYSICS_FILE_COPY_CTORS_PASS29) && !defined(FABLE_RETAIL_C3DDUMMYOBJECT_TRANSFORMCTOR)
    // The file reader calls this constructor after the by-value getter.
    C3DDummyObject(const CMatrix3x4&, unsigned long);
#endif
    C3DDummyObject();                                                    // 0x00A2E4B0
#ifdef FABLE_RETAIL_C3DDUMMYOBJECT_TRANSFORMCTOR
    // FableWin constructor, inlined at 0x00A2FA30.
    C3DDummyObject(const CMatrix3x4& transform,unsigned long bone_index) : Transform(transform), BoneIndex(bone_index) { }
#endif
#ifdef FABLE_RETAIL_C3DMESHOBJECT_STATIC_DUMMY_PASS26
    // FableWin 031D2270 calls GetTranslation on Transform; retail GetDummyObjectPositionStatic reads its last row.
    C3DVector GetPosition() const { return Transform.GetTranslation(); }
#endif
    const CMatrix3x4& PeekTransform() const { return Transform; }
    long GetBoneIndex() const { return BoneIndex; }
#ifdef FABLE_RETAIL_C3DDUMMYOBJECT_MESH_FRIEND_PASS23
    // Both PDBs: fields protected. FableWin03364d7b..84 directly copies BoneIndex;
    // all four original mesh methods directly address Transform, proving this friendship.
    friend class C3DMesh2;
protected:
#else
private:
#endif
    CMatrix3x4 Transform;                                                // +0x00
    long BoneIndex;                                                      // +0x30
};
FABLE_STATIC_ASSERT_AT(19, sizeof(C3DDummyObject) == 0x34);
#endif
