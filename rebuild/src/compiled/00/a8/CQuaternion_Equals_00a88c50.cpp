// CQuaternion::Equals(const CQuaternion&) -- true when every component differs from the other quaternion's by less than 0.0001
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A88C50, length 96 bytes. No callees.
// Shape: four `fld [this+n] ; fsub [q+n] ; fabs ; fcomp [eps] ; fnstsw ; test ah,5 ; jp false` blocks (the ordered
// `<` of each |difference| against the 0.0001f constant at 0x0129BA3C), then `mov al,1 ; ret 4` and a shared
// `xor al,al ; ret 4` false exit: `if (a && b && c && d) return true; return false;` over the double fabs
// intrinsic. CQuaternion members X/Y/Z/W from the PDB (16 bytes).
#include <math.h>
#pragma intrinsic(fabs)

extern const float g_QuaternionEpsilon;   // 0x0129BA3C (0.0001f)

struct CQuaternion {
    float X, Y, Z, W;
    bool Equals(const CQuaternion& q) const;
};

bool CQuaternion::Equals(const CQuaternion& q) const
{
    if (fabs(X - q.X) < g_QuaternionEpsilon &&
        fabs(Y - q.Y) < g_QuaternionEpsilon &&
        fabs(Z - q.Z) < g_QuaternionEpsilon &&
        fabs(W - q.W) < g_QuaternionEpsilon)
        return true;
    return false;
}