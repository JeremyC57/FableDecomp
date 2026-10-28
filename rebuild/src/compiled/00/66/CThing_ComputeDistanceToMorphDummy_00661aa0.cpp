// CThing::ComputeDistanceToMorphDummy() -- distance from the thing's position to its appearance's "EYE_SET_L" dummy (1.0 when the dummy is missing)
// Leaf: ComputeDistanceToMorphDummy
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00661AA0, length 145 bytes (padded to 160).
// Label correction: the catalog's "CVertexBufferWin32::" class is wrong: `this` is a CThing (CThing::GetPos @ 0x004C73D0 on
// this, and retail +0x64 is CThing::PTCGraphicAppearance, PDB +0x74 less the 16 bytes of retail's smaller containers).
// Callees: CCharString::CCharString(const char*, long) @ 0x0099EBF0, CTCGraphicAppearance::GetDummyObject(const CCharString&,
// long, unsigned long, C3DVector*, CRightHandedSet*, float) @ 0x004C08C0, CCharString::~CCharString @ 0x0099EAE0,
// CThing::GetPos @ 0x004C73D0. Data: "EYE_SET_L" @ 0x0125AAD0, 1.0f @ 0x0122DED8 (the missing-dummy result).
// Shape: the dummy name is a named local in its own `{}` block; the lookup result is kept negated in bl across its
// destructor (`neg ; sbb ; inc` = `? false : true`); the appearance pointer is a named local read after the string ctor,
// which loads it into ecx before the argument pushes.
#include <math.h>
#pragma intrinsic(sqrt)
class CCharString { public:
    void* m_pData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                // 0x0099EAE0
};
class C3DVector { public:
    float X, Y, Z;
};
class CRightHandedSet { public:                           // 24 bytes
    C3DVector Up;                                  // +0x00
    C3DVector Forward;                             // +0x0c
};
struct CTCGraphicAppearance {
    bool GetDummyObject(const CCharString& name, long index, unsigned long flags, C3DVector* pos, CRightHandedSet* set, float scale);   // 0x004C08C0
};
class CThing { public:
    unsigned char         _pad0[0x64];
    CTCGraphicAppearance* PTCGraphicAppearance;    // +0x64 (retail)
    const C3DVector& GetPos() const;          // 0x004C73D0
    float ComputeDistanceToMorphDummy();
};
float CThing::ComputeDistanceToMorphDummy()
{
    C3DVector dummyPos;
    CRightHandedSet dummySet;
    bool missing;
    {
        CCharString name("EYE_SET_L");
        CTCGraphicAppearance* appearance = PTCGraphicAppearance;
        missing = appearance->GetDummyObject(name, 0, 0, &dummyPos, &dummySet, 1.0f) ? false : true;
    }
    if (missing)
        return 1.0f;
    const C3DVector& pos = GetPos();
    float dx = dummyPos.X - pos.X;
    float dy = dummyPos.Y - pos.Y;
    float dz = dummyPos.Z - pos.Z;
    return (float)sqrt(dx * dx + dy * dy + dz * dz);
}