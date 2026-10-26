// CTCBulletTime::GetBulletTimeTicksFromAbilityLevel(float value) -- ceil(value) as a long through the engine's inline-assembly FPU ceiling helper (the standalone copy of the helper inlined into 0x0079C5F0 / 0x0079C680 / 0x0079C6D0)
// Leaf: ?GetBulletTimeTicksFromAbilityLevel@CTCBulletTime@@QAEJM@Z
// Flags: /O2 /Oy /W3
// Retail-asm: 0x004FA9B4..0x004FA9C3 `fld [ebp+8] ; fadd [ebp-0Ch] ; fistp dword ptr [ebp-4] ; fild [ebp-4] ; fsub [ebp-8] ; fstp [ebp-10h]` on named memory locals (0.5f / 1.0f stored as frame constants, a bare dword fistp into a named long): VC7.1 emits no bare dword fistp from C++ (/QIfist gives `fistp qword`), so the six-instruction block is the original's `__asm`; the fix-up (`cmp eax,[ebp-10h] ; jne ; dec dword ptr [ebp-4]`) is C++ on the same memory locals.
// Retail 0x004FA9A0, 58 bytes (`ret 4`, `this` unused; no direct callers). The ceiling is the classic FPU trick: round
// (x + 0.5) to nearest with the default rounding mode, then step back one when (result - 1) equals x exactly
// (x was already integral and the half rounded up). The inline asm forces the ebp frame (`push ebp ; mov ebp,esp ; sub esp,10h`).
#pragma optimize("y", off)
class CTCBulletTime
{ public:
    long GetBulletTimeTicksFromAbilityLevel(float value);
};

long CTCBulletTime::GetBulletTimeTicksFromAbilityLevel(float value)
{
    long  result;
    float one;
    float half;
    float back;
    half = 0.5f;
    one = 1.0f;
    __asm fld value
    __asm fadd half
    __asm fistp result
    __asm fild result
    __asm fsub one
    __asm fstp back
    if (*(long*)&value == *(long*)&back)
        --result;
    __asm mov eax, result
}