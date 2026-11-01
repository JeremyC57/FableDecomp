// CQuaternion::Equals(const CQuaternion&) -- true when every component differs from the other quaternion's by less than 0.0001
// Naming wave 18: the shared const float 1e-4f at 0x0129BA3C is ?DELTA_NOTIONAL_ZERO@@3MB (raw Ego_r public + typed data record; was EPSILON).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A88C50, length 96 bytes. No callees.
// Shape: four `fld [this+n] ; fsub [q+n] ; fabs ; fcomp [eps] ; fnstsw ; test ah,5 ; jp false` blocks (the ordered
// `<` of each |difference| against the 0.0001f constant at 0x0129BA3C), then `mov al,1 ; ret 4` and a shared
// `xor al,al ; ret 4` false exit: `if (a && b && c && d) return true; return false;` over the double fabs
// intrinsic. CQuaternion members X/Y/Z/W from the PDB (16 bytes).
#include <math.h>
#pragma intrinsic(fabs)

extern const float DELTA_NOTIONAL_ZERO;            // 0x0129BA3C (0.0001f)

class CQuaternion { public:
    float X, Y, Z, W;
    bool Equals(const CQuaternion& quaternion) const;
};

bool CQuaternion::Equals(const CQuaternion& quaternion) const
{
    if (fabs(X - quaternion.X) < DELTA_NOTIONAL_ZERO &&
        fabs(Y - quaternion.Y) < DELTA_NOTIONAL_ZERO &&
        fabs(Z - quaternion.Z) < DELTA_NOTIONAL_ZERO &&
        fabs(W - quaternion.W) < DELTA_NOTIONAL_ZERO)
        return true;
    return false;
}