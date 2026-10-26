// CQuaternion::IsIdentity() -- true when X, Y and Z are within 0.0001 of zero and W within 0.0001 of one
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A88CB0, length 83 bytes. No callees.
// Shape: `fld [this+n] ; fabs ; fcomp [eps] ; fnstsw ; test ah,5 ; jp false` for X, Y, Z, then
// `fld W ; fsub [1.0f] ; fabs ; fcomp [eps]` (the 1.0f constant at 0x0122DED8, the 0.0001f one at 0x0129BA3C),
// `mov al,1 ; ret` and a shared `xor al,al ; ret` false exit: the `if (&&) return true; return false;` form.
#include <math.h>
#pragma intrinsic(fabs)

extern const float g_QuaternionEpsilon;   // 0x0129BA3C (0.0001f)
extern const float g_One;                 // 0x0122DED8 (1.0f)

class CQuaternion { public:
    float X, Y, Z, W;
    bool IsIdentity() const;
};

bool CQuaternion::IsIdentity() const
{
    if (fabs(X) < g_QuaternionEpsilon &&
        fabs(Y) < g_QuaternionEpsilon &&
        fabs(Z) < g_QuaternionEpsilon &&
        fabs(W - g_One) < g_QuaternionEpsilon)
        return true;
    return false;
}