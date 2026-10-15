// CNavigationInput::operator==(const CNavigationInput&) const -- two navigation requests are equal when their movement types match and the proximity, the destination and (for movement type 0) the speed agree within 1e-4
// Leaf: ??8CNavigationInput
// Retail: Fable.exe (TLC, VC7.1) @ 0x006CCF70, 134 bytes (`ret 4`).
// Callees: none (x87 `fabs` intrinsic); tolerance 1e-4f at 0x0129BA3C.
// Layout = Ego_r.pdb CNavigationInput (24 bytes): Destination +0, Proximity +0xc, MovementType +0x10, Speed +0x14.
// Shape: the scalar compares are `fabs(a - b) < tol` (`test ah,5 ; jp`); the destination goes through C3DVector's inline
//   `__forceinline` tolerance compare, which tests `<=` per component (`test ah,41h ; jp`); `if (...) return true; return false;`
//   gives the byte exits (`return a && b` widens to eax).
#include <math.h>
#pragma intrinsic(fabs)
struct C3DVector {
    float X, Y, Z;
    __forceinline bool IsEqual(const C3DVector& other, float tolerance) const
    {
        return fabs(X - other.X) <= tolerance && fabs(Y - other.Y) <= tolerance && fabs(Z - other.Z) <= tolerance;
    }
};
enum EMovementType { MOVEMENT_TYPE_0 = 0 };
struct CNavigationInput {
    C3DVector     Destination;                     // +0x00
    float         Proximity;                       // +0x0c
    EMovementType MovementType;                    // +0x10
    float         Speed;                           // +0x14
    bool operator==(const CNavigationInput& other) const;
};

bool CNavigationInput::operator==(const CNavigationInput& other) const
{
    if (MovementType == other.MovementType
        && fabs(Proximity - other.Proximity) < 0.0001f
        && Destination.IsEqual(other.Destination, 0.0001f)
        && (MovementType != MOVEMENT_TYPE_0 || fabs(Speed - other.Speed) < 0.0001f))
        return true;
    return false;
}
