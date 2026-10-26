// NCameraModes::CScriptModuleGoToLookInDirection::Update() -- put the owning camera at GoTo and aim it at GoTo + LookDirection
// Leaf: Update
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00851E70, length 91 bytes. No callees.
// Shape: the owner (POwner, +0x38) gets the plain C3DVector GoTo copied as a whole through re-based
// pointers (`add edx,54h`, three dword moves via edi/esi); then the inline `operator+` builds its result
// field by field in a NAMED local (x, y, z in order: `fld LookDirection.x ; fadd GoTo.x`, the right operand
// loaded first), which lands in a 12-byte frame temporary and is assigned to the owner's look-at (+0x10)
// through integer registers, the owner pointer re-read after the first store (`mov eax,[ecx+38h]`).
// A three-float value constructor would evaluate its arguments right to left (z first).
// Layout: PDB NCameraModes::CScriptModuleGoToLookInDirection (0x64 bytes): GoTo +0x4c, LookDirection
// +0x58 over the 0x4c-byte CScriptModuleBase (POwner +0x38, typed CBase* in the PDB; the object written
// here holds the look-at at +0x10 and the position at +0x54 and is modelled locally as CCameraState).
class C3DVector { public:
    float X, Y, Z;
    C3DVector() { }
    C3DVector operator+(const C3DVector& other) const
    {
        C3DVector result;
        result.X = X + other.X;
        result.Y = Y + other.Y;
        result.Z = Z + other.Z;
        return result;
    }
};

struct CCameraState {
    unsigned char _pad00[0x10];
    C3DVector     LookAt;                            // +0x10
    unsigned char _pad1c[0x38];
    C3DVector     Position;                          // +0x54
};

namespace NCameraModes {

struct CScriptModuleGoToLookInDirection {
    virtual void v00();
    unsigned char _pad04[0x34];
    CCameraState* POwner;                            // +0x38
    unsigned char _pad3c[0x10];
    C3DVector     GoTo;                              // +0x4c
    C3DVector     LookDirection;                     // +0x58
    void Update();
};

void CScriptModuleGoToLookInDirection::Update()
{
    POwner->Position = GoTo;
    POwner->LookAt = GoTo + LookDirection;
}

}