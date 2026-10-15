// CTCBulletTime::GetBulletTimeTicksFromAbilityLevel(float value) -- the standalone copy of the inline-asm ceiling helper: ceil(value) as a long
// Flags: /QIfist
// Grade: DIFFER(62v58) -- lane gap: inline __asm; the retail body is a frame-pointer function whose rounding
//        is an inline-asm block on memory locals (`mov [ebp-0Ch],0.5f ; mov [ebp-8],1.0f ; fld x ;
//        fadd [ebp-0Ch] ; fistp dword [ebp-4] ; fild [ebp-4] ; fsub [ebp-8] ; fstp [ebp-14h]`, then an
//        integer compare of the float bits and `dec dword ptr [ebp-4]`); compiler-emitted C++ cannot
//        produce a bare `fistp dword` into a named local (/QIfist gives `fistp qword` + copies).
//
// Retail 0x004FA9A0, 58 bytes (`ret 4`, `this` unused; no direct callers). The ceiling is the classic FPU trick: round
// (x + 0.5) to nearest with the default rounding mode, then step back one when (result - 1) equals x
// exactly (x was already integral and the half rounded up). The same inline-asm helper is inlined
// into 0x0079C5F0 (ticks from the duration), 0x0079C680 (remaining percentage) and 0x0079C6D0
// (elapsed ticks); this is the standalone copy taking the float argument. No byte-pure reconstruction is possible: kept as the C model.

inline long CeilToLong(float value)                 // inline __asm FPU helper in the original
{
    long result = (long)(value + 0.5f);             // fistp: round to nearest
    float back = (float)result - 1.0f;
    if (*(long*)&value == *(long*)&back)
        --result;
    return result;
}

struct CTCBulletTime
{
    long GetBulletTimeTicksFromAbilityLevel(float value);
};

long CTCBulletTime::GetBulletTimeTicksFromAbilityLevel(float value)
{
    return CeilToLong(value);
}
