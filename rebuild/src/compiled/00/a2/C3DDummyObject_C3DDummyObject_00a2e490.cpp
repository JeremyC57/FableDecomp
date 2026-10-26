// C3DDummyObject::C3DDummyObject(const CMatrix3x4&, long) -- copies the 48-byte transform block (rep movsd) and stores the bone index at +0x30
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A2E490, length 29 bytes. No callees.
// PDB layout: C3DDummyObject = {CMatrix3x4 Transform (+0, 48 bytes), long BoneIndex (+0x30)}.
// CMatrix3x4 is a POD union of 12 floats, so its copy is a block move; ret 8.
class CMatrix3x4 { public:
    float m[4][3];
};

class C3DDummyObject { public:
    CMatrix3x4 Transform;                  // +0
    long BoneIndex;                        // +0x30
    C3DDummyObject(const CMatrix3x4& transform, long boneIndex);
};

C3DDummyObject::C3DDummyObject(const CMatrix3x4& transform, long boneIndex)
    : Transform(transform), BoneIndex(boneIndex)
{
}