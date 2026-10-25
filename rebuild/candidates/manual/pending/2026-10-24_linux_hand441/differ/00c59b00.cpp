// SecondsToMixBlocks(float seconds) -- convert a time in seconds to whole mix blocks of the output: seconds * (44100 derived from the performance-counter frequency) / the block size (the configured block frames, else 128), rounded down through the retail inline-asm `fistp` (x - 0.5 rounded to nearest, plus one for a negative result)
// Grade: DIFFER(129v155) -- retail_asm candidate: `fstp [esp+18h] ; fld [esp+18h] ; fistp dword [esp+8]` (0x00C59B80..0x00C59B88) is the engine's inline-asm ROUND helper homed in the dead `seconds` slot; this C++ reference rounds through __ftol2 calls instead
// Leaf: ?SecondsToMixBlocks@@YAJM@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C59B00, length 155 bytes (+ int3 padding); `ret` (__cdecl).
// Callees: __allmul 0x00BFEB50 (frequency * 44100), __alldiv 0x00F3FC50 (/ frequency). Globals: the block-size switch
// 0x0139CA14, the block frames 0x0139C9F0, the QueryPerformanceFrequency result 0x0139CA20 (__int64, written by the
// CODA system init 0x00C3FD70), 2^32 @ 0x0122DCB4, 0.5f @ 0x0122F59C.
// Callers (15): the envelope / vibrato / pitch-envelope starts and ramps (0x00C59C37, 0x00C59C9E, 0x00C59D45, ...).
// Label: the manifest's "Graphics_ConvertTimeToPixels" is propagated; descriptive name (GameCODA, no PDB/RTTI).
// Shape: the block size is one unsigned ternary converted to float (the global arm `fild dword ; jge ; fadd 2^32`,
// the constant arm `mov [esp+4],80h ; mov [esp+8],0 ; fild qword`); the 64-bit product/quotient is converted as an
// unsigned long. Retail-asm evidence: the bare `fistp dword` with the store/reload of its by-value copy (the
// $S/w22/fistp ROUND idiom); a minimal `__asm fld value / __asm fistp result` helper would be grade retail_asm.
// C++ stand-in for the retail inline-asm rounding helper (`fld value ; fistp result`, round to nearest even): the
// retail row cannot come from C++ (VC7.1 `(long)x` calls __ftol2, /QIfist emits `fistp qword`).
static long RoundToNearest(float value)
{
    float down = (float)(long)value;
    if (value < 0.0f && down != value)
        down -= 1.0f;
    float frac = value - down;
    long result = (long)down;
    if (frac > 0.5f || (frac == 0.5f && (result & 1)))
        ++result;
    return result;
}

extern unsigned long g_useBlockFrames;                  // 0x0139CA14
extern unsigned long g_blockFrames;                     // 0x0139C9F0
extern __int64 g_performanceFrequency;                  // 0x0139CA20

long __cdecl SecondsToMixBlocks(float seconds)
{
    float blockFrames = (float)(g_useBlockFrames ? g_blockFrames : 128u);
    unsigned long rate = (unsigned long)(g_performanceFrequency * 44100 / g_performanceFrequency);
    seconds = (float)rate * seconds / blockFrames - 0.5f;
    long blocks = RoundToNearest(seconds);
    if (blocks < 0)
        ++blocks;
    return blocks;
}
