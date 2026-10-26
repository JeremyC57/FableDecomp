// CTCCameraPointScripted::GetStartPos(const C3DVector* origin, const CRightHandedSet* axes) const -- the scripted camera's start position: (parent position + StartPos) transformed into the given frame
// Leaf: GetStartPos
// Retail: Fable.exe (TLC, VC7.1) function @ 0x000075CD30, length 81 bytes (`ret 0Ch`: hidden result + two pointers).
// Callees: CThing::GetPosition() const @ 0x004C73D0 (const C3DVector&), CTCCameraPointDefinitionBase::TransformPosition(const
//   C3DVector&, const C3DVector*, const CRightHandedSet*) const @ 0x0075C420 (landed; returns through this function's own
//   hidden result pointer).
// Label: Ego_r.pdb names the adjacent pair ?GetStartPos / ?GetEndPos@CTCCameraPointScripted@@QBE?AVC3DVector@@PBV2@PBVCRightHandedSet@@@Z
//   (two pointer parameters); 0x0075CD30 reads +0x40..+0x48 = StartPos and 0x0075CD90 reads +0x4C..+0x54 = EndPos (PDB
//   CTCCameraPointScripted StartPos +0x40, EndPos +0x4C, LookPos +0x58; CTCCameraPointDefinitionBase is 0x40 bytes), so the
//   catalog's "GetEndPos" label on 0x0075CD30 is GetStartPos.
// Layout: CTCCameraPointDefinitionBase as the landed TransformPosition 0x0075C420 model (packed, 0x3D bytes used, size 0x40).
// Shape: the sum is a 12-byte frame temporary built by an inline free operator+ (three-float constructor); the parent thing
//   is reached through an inline GetParentThing() accessor -- with the plain member VC7.1 loads the position's X first
//   (`fld [eax] ; fadd [esi+N]`), through the accessor the member first as retail.
class C3DVector { public:
    float X;
    float Y;
    float Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
};

inline C3DVector operator+(const C3DVector& a, const C3DVector& b)
{
    return C3DVector(a.X + b.X, a.Y + b.Y, a.Z + b.Z);
}

class CRightHandedSet;

class CThing { public:
    const C3DVector& GetPosition() const;           // 0x004C73D0
};

#pragma pack(push, 1)
class CTCCameraPointDefinitionBase {
public:
    virtual void v00();
    CThing*       ParentThing;                      // +0x04 (CTCBase)
    unsigned char _pad08[0x3d - 0x08];              // +0x08 CTCBase flags .. Activated (+0x3c)
    C3DVector TransformPosition(const C3DVector& position, const C3DVector* origin, const CRightHandedSet* axes) const;   // 0x0075C420
};
class CTCCameraPointScripted : public CTCCameraPointDefinitionBase {
public:
    unsigned char _pad3d[3];                        // +0x3d (base padded to 0x40)
    C3DVector     StartPos;                         // +0x40
    C3DVector     EndPos;                           // +0x4c
    C3DVector     LookPos;                          // +0x58
    CThing* GetParentThing() const { return ParentThing; }
    C3DVector GetStartPos(const C3DVector* origin, const CRightHandedSet* axes) const;
};
#pragma pack(pop)

C3DVector CTCCameraPointScripted::GetStartPos(const C3DVector* origin, const CRightHandedSet* axes) const
{
    return TransformPosition(GetParentThing()->GetPosition() + StartPos, origin, axes);
}