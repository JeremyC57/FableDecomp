// SecondsToMixBlocks(float seconds) -- libpk mixer: how many mix blocks a time span covers at 44.1 kHz, truncated toward zero (block length from the configured block frame count, 128 when it is not in use)
// Leaf: ?SecondsToMixBlocks@@
// Retail-asm: 0x00C59B84 `fld dword ptr [esp+18h]` ; 0x00C59B88 `fistp dword ptr [esp+8]` -- libpk's inline-assembly float-to-long helper (rasm1 family (a)): the value less 0.5 is stored to the dead `seconds` argument slot (the helper's by-value parameter) and reloaded for a bare dword fistp into a frame word, then the `< 0 ? +1` fix-up (`test eax,eax ; jge ; inc eax`) in C++ turns the round-to-nearest of x-0.5 into truncation toward zero; VC7.1 C++ `(long)x` calls __ftol2, /QIfist emits `fistp qword` and /arch:SSE2 cvttss2si (rasm1 probe family (a)). The block-length select, the 64-bit rate arithmetic (__allmul 0x00BFEB50, __alldiv 0x00F3FC50) and the unsigned-to-float conversions are C++.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C59B00, 155 bytes (+5 int3), __cdecl, plain `ret`. Globals: block-frames-in-use
//   flag 0x0139CA14, block frames 0x0139C9F0, performance frequency (64-bit) 0x0139CA20; constants 2^32 @ 0x0122DCB4, 0.5f @
//   0x0122F59C. The catalog label "Graphics_ConvertTimeToPixels" is descriptive.
// libpk's inline-assembly truncation helper: round-to-nearest of (x - 0.5) by a bare fistp, corrected to truncation for negatives
__forceinline long FloatToLongTrunc(float value)
{
    long result;
    value = value - 0.5f;
    __asm fld value
    __asm fistp result
    if (result < 0)
        ++result;
    return result;
}
static const __int64 kDefaultBlockFrames = 128;         // a 64-bit constant: VC7.1 does not fold its float conversion (fild qword of 128)
extern unsigned long g_useBlockFrames;                  // 0x0139CA14
extern unsigned long g_blockFrames;                     // 0x0139C9F0
extern __int64 g_performanceFrequency;                  // 0x0139CA20
long __cdecl SecondsToMixBlocks(float seconds)
{
    float blockFrames = g_useBlockFrames ? (float)g_blockFrames : (float)kDefaultBlockFrames;
    unsigned long rate = (unsigned long)(g_performanceFrequency * 44100 / g_performanceFrequency);
    float samples = (float)rate * seconds;              // a named product: VC7.1 otherwise re-associates `a * b / c` into `a / c * b`
    return FloatToLongTrunc(samples / blockFrames);
}