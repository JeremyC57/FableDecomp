// CThingPhysical::ModifyHealth(float amount, bool fromHit) -- rounds the health change to the nearest whole number (ties away from zero) and applies it through InternalModifyHealth when it is not (near) zero
// Grade: DIFFER(156v159) lane gap -- retail rounds with bare dword `fistp [ebp+8]` / `fild` sequences (an inline-asm style round helper with
// 0.5f/1.0f frame constants and an exact-tie fix-up `cmp eax,[ebp-10h] ; jne ; inc/dec [ebp+8]`); the lane compiles (long) casts to
// _ftol calls (known lane gap: bare dword fistp, see the PackVertexNormal family)
// Leaf: ?ModifyHealth@CThingPhysical
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006A68F0, length 159 bytes (`ret 8`), frame pointer (`optimize("y", off)` TU).
// Callees: CThingPhysical::InternalModifyHealth(float, bool) @ 0x006A62E0.
// Globals: float 0.0f @ 0x0122DEDC, float 1e-4f @ 0x0129BA3C.
#pragma optimize("y", off)
#include <math.h>
struct CThingPhysical {
    void InternalModifyHealth(float amount, bool fromHit);   // 0x006A62E0
    void ModifyHealth(float amount, bool fromHit);
};

static inline long RoundPositive(float v)
{
    float half = 0.5f;
    float one = 1.0f;
    long r = (long)(v + half);
    float back = (float)r - one;
    if (*(long*)&v == *(long*)&back)
        --r;
    return r;
}

static inline long RoundNegative(float v)
{
    float half = 0.5f;
    float one = 1.0f;
    long r = (long)(v - half);
    float back = (float)r + one;
    if (*(long*)&v == *(long*)&back)
        ++r;
    return r;
}

void CThingPhysical::ModifyHealth(float amount, bool fromHit)
{
    long rounded = amount >= 0.0f ? RoundPositive(amount) : RoundNegative(amount);
    amount = (float)rounded;
    if ((float)fabs(amount) > 1e-4f)
        InternalModifyHealth(amount, fromHit);
}
