// CHeroPostcardGeneratorDef::operator=(const CHeroPostcardGeneratorDef&) -- memberwise copy: CParentDefClassBase part, the mesh-area corners, the doll centre offset (block copy) and the bounding radius; returns *this
#pragma optimize("s", on)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0045D5D3, length 64 bytes.
// Pragma: #pragma optimize("s", on) -- the def module is size-optimised.
// Callees: CParentDefClassBase::operator= @ 0x00431ED0.
// Identity: the catalog label "CParentDefClassBase::operator=" is propagated. CHeroPostcardGeneratorDef's vtable
// (0x01233CB4) slot 19 (Copy) is the 5-byte `jmp 0x0045D5D3` right before this body, and the PDB layout
// (MeshAreaTLPos +0x3d, MeshAreaBRPos +0x45, DollCentreOffset +0x4d, DollBoundingSphereRadius +0x5c) is the
// retail one shifted by the 4-byte-smaller retail SubDefInfoMap (+0x39, +0x41, +0x49, +0x58).
// Layout: the vectors sit unaligned right after the 0x39-byte packed base, yet the float after the
// C3DVector is padded to +0x58: the math vectors are byte-aligned types (declared under pack(1) like
// the def classes) while a plain float keeps its 4-byte alignment. Shape: the C2DVectors copy as two
// dword moves each, the C3DVector as a `movsd` x3 block (implicit POD assignment under "s"), so the
// body needs four callee-saved registers (ebx = this, ebp = other).
// The engine header CParentDefClassBase.h is the PDB layout (0x3d bytes); the retail base is 0x39 bytes.
#pragma pack(push, 1)
class CParentDefClassBase { public:
    virtual void v00();
    unsigned char _defClassBase[0x35];      // +0x04 CDefClassBase part, InstantiationName (+0x25), SubDefInfoMap (+0x29)
    CParentDefClassBase& operator=(const CParentDefClassBase& other);   // 0x00431ED0
};

class C2DVector { public:
    float X;
    float Y;
};

class C3DVector { public:
    float X;
    float Y;
    float Z;
};
#pragma pack(pop)

class CHeroPostcardGeneratorDef : public CParentDefClassBase { public:    // vtable 0x01233CB4
    C2DVector MeshAreaTLPos;                // +0x39
    C2DVector MeshAreaBRPos;                // +0x41
    C3DVector DollCentreOffset;             // +0x49
    float     DollBoundingSphereRadius;     // +0x58
    CHeroPostcardGeneratorDef& operator=(const CHeroPostcardGeneratorDef& other);
};

CHeroPostcardGeneratorDef& CHeroPostcardGeneratorDef::operator=(const CHeroPostcardGeneratorDef& other)
{
    CParentDefClassBase::operator=(other);
    MeshAreaTLPos = other.MeshAreaTLPos;
    MeshAreaBRPos = other.MeshAreaBRPos;
    DollCentreOffset = other.DollCentreOffset;
    DollBoundingSphereRadius = other.DollBoundingSphereRadius;
    return *this;
}