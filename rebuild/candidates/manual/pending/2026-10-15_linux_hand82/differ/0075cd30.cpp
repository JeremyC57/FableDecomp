// CTCCameraPointScripted::GetEndPos(const C3DVector& origin, const CRightHandedSet& axes) const -- the scripted camera's end position: (parent position + EndPos) transformed into the given frame
//
// Retail 0x0075CD30, 81 bytes (`ret 0Ch`: hidden result + two references). Callees: CThing::
// GetPosition() const @ 0x004C73D0 (returns a const C3DVector&), CTCCameraPointDefinitionBase::
// TransformPosition(const C3DVector&, const C3DVector&, const CRightHandedSet&) const @ 0x0075C420
// (returns a C3DVector through its own hidden pointer, which is this function's result). Called
// twice from the CameraUseCameraPoint script functions.
// Layout: EndPos is the C3DVector at +0x40 (Ego_r.pdb CTCCameraPointScripted +0x4c; the retail
// CTCCameraPointDefinitionBase sits 0xc lower).
// Grade: DIFFER(81v81) -- scheduling only: retail computes the X sum as `fld [esi+40h] ; fadd [eax]`
//        (EndPos.X first) and stores it after two pushes, the lane loads `fld [eax] ; fadd [esi+40h]`
//        and stores X after three pushes. A member `operator+` on `pos + EndPos` fixes X and the store
//        order but loads EndPos first for Z and Y; `EndPos + pos` is this file's result again.
// Shape: the inline C3DVector `operator+` builds the sum in a 12-byte frame temporary (Z, Y, X computed
// on the FPU before the argument pushes and stored between them), the temporary's address and the
// two caller references are forwarded, and the hidden result pointer (edi) is handed straight to
// TransformPosition and returned.

struct C3DVector
{
    float X, Y, Z;
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
};

inline C3DVector operator+(const C3DVector& a, const C3DVector& b)
{
    return C3DVector(a.X + b.X, a.Y + b.Y, a.Z + b.Z);
}

struct CRightHandedSet;

struct CThing
{
    const C3DVector& GetPosition() const;           // 0x004C73D0
};

#pragma pack(push, 1)
struct CTCCameraPointScripted
{
    void*         __vftable;                        // +0x00 (CTCBase)
    CThing*       ParentThing;                      // +0x04 (CTCBase)
    unsigned char _pad_0x08[0x2c];                  // +0x08 (CTCBase flags, CTCCameraPointDefinitionBase)
    C3DVector     StartPos;                         // +0x34
    C3DVector     EndPos;                           // +0x40

    C3DVector TransformPosition(const C3DVector& position, const C3DVector& origin, const CRightHandedSet& axes) const;   // 0x0075C420
    C3DVector GetEndPos(const C3DVector& origin, const CRightHandedSet& axes) const;
};
#pragma pack(pop)

C3DVector CTCCameraPointScripted::GetEndPos(const C3DVector& origin, const CRightHandedSet& axes) const
{
    return TransformPosition(ParentThing->GetPosition() + EndPos, origin, axes);
}
