// CQuaternion::IsIdentity() -- true when X, Y and Z are within 0.0001 of zero and W within 0.0001 of one
// Naming wave 18: the shared const float 1e-4f at 0x0129BA3C is ?DELTA_NOTIONAL_ZERO@@3MB (raw Ego_r public + typed data record; was EPSILON).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A88CB0, length 83 bytes. No callees.
// Shape: `fld [this+n] ; fabs ; fcomp [eps] ; fnstsw ; test ah,5 ; jp false` for X, Y, Z, then
// `fld W ; fsub [1.0f] ; fabs ; fcomp [eps]` (the 1.0f constant at 0x0122DED8, the 0.0001f one at 0x0129BA3C),
// `mov al,1 ; ret` and a shared `xor al,al ; ret` false exit: the `if (&&) return true; return false;` form.
#include <math.h>
#pragma intrinsic(fabs)

extern const float DELTA_NOTIONAL_ZERO;            // 0x0129BA3C (0.0001f)
extern const float g_One;                 // 0x0122DED8 (1.0f)

class CQuaternion { public:
    float X, Y, Z, W;
    bool IsIdentity() const;
};

bool CQuaternion::IsIdentity() const
{
    if (fabs(X) < DELTA_NOTIONAL_ZERO &&
        fabs(Y) < DELTA_NOTIONAL_ZERO &&
        fabs(Z) < DELTA_NOTIONAL_ZERO &&
        fabs(W - g_One) < DELTA_NOTIONAL_ZERO)
        return true;
    return false;
}