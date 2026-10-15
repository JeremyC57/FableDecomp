// CTCCameraPointScripted::GetEndPos(long a, long b) -- returns TransformPosition(parent position + EndPos, a, b)
// Grade: DIFFER(81v81) -- same instructions; the X sum is `fld [eax] ; fadd [esi+4Ch]` where retail loads EndPos.X first (`fld [esi+4Ch] ; fadd [eax]`), and one `fstp`/`push` pair is scheduled differently
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0075CD90, length 81 bytes.
// Callees: CThing::GetPosition @ 0x004C73D0, CTCCameraPointDefinitionBase::TransformPosition @ 0x0075C420
// (manifest; `__thiscall`, hidden C3DVector result + const C3DVector& + two pass-through arguments, `ret 10h`).
// Called from 0x0089E274.
// Layout: engine/CTCBase.h (ParentThing +4); Ego_r.pdb CTCCameraPointScripted EndPos (C3DVector) at +0x4c.
// Shape: the sum is a 12-byte frame temporary built by an inline `operator+` (three-float constructor,
// arguments evaluated Z, Y, X); the hidden result pointer goes in edi and is returned.
// Tried: free inline operator+ (a.X + b.X), the X operands swapped (b.X + a.X: the lane still loads
// [eax] first), a member operator+ over a named position reference (worse: Y/Z then load the member
// first and store each component immediately). Next: lane gap (commutative fadd operand order chosen
// by the scheduler).
#include "engine/CTCBase.h"

struct C3DVector {
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

struct CThing {
    const C3DVector& GetPosition() const;           // 0x004C73D0
};

struct CTCCameraPointScripted : CTCBase {
    unsigned char _pad0c[0x4c - 0x0c];
    C3DVector EndPos;                               // +0x4c
    C3DVector TransformPosition(const C3DVector& pos, long a, long b) const;   // 0x0075C420
    C3DVector GetEndPos(long a, long b) const;
};

C3DVector CTCCameraPointScripted::GetEndPos(long a, long b) const
{
    return TransformPosition(ParentThing->GetPosition() + EndPos, a, b);
}
