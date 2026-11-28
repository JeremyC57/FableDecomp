// Flags: 
// 00c59b00 CSalBaseVoice::TimeToFrames
// Leaf: ?TimeToFrames@CSalBaseVoice@@KAHM@Z
// Retail-asm: 0x00C59B84 `fld dword ptr [esp+18h]` ; 0x00C59B88 `fistp dword ptr [esp+8]` -- libpk's inline-assembly float-to-long helper (rasm1 family (a)): the value less 0.5 is stored to the dead `seconds` argument slot (the helper's by-value parameter) and reloaded for a bare dword fistp into a frame word, then the `< 0 ? +1` fix-up (`test eax,eax ; jge ; inc eax`) in C++ turns the round-to-nearest of x-0.5 into truncation toward zero; VC7.1 C++ `(long)x` calls __ftol2, /QIfist emits `fistp qword` and /arch:SSE2 cvttss2si (rasm1 probe family (a)). The block-length select, the 64-bit rate arithmetic (__allmul 0x00BFEB50, __alldiv 0x00F3FC50) and the unsigned-to-float conversions are C++.
// Retail-asm-helper: libpk/FtoL.h supplies the dword conversion and portable C++ path.
#include "engine/CSalBaseVoice.h"
#include "engine/CodaCaps.h"
#include "fable_compiler.h"
#include "libpk/FtoL.h"
// Inline-budget: the original call sites contain the rounding and negative-result adjustment in place.
FABLE_FORCEINLINE long FloatToLongTrunc(float value)
{
    long result = FtoL(value - 0.5f);
    if (result < 0)
        ++result;
    return result;
}
static const __int64 kDefaultBlockFrames = 128;         // a 64-bit constant: VC7.1 does not fold its float conversion (fild qword of 128)
int CSalBaseVoice::TimeToFrames(float fTime)
{
    float fFrameSamples = g_SalCaps.nSWAllVoices ? (float)g_SalCaps.SWMixer.nFrameStepSamples : (float)kDefaultBlockFrames;
    unsigned long rate = (unsigned long)(g_SalCaps.RefTimeTicksPerSec * 44100 / g_SalCaps.RefTimeTicksPerSec);
    float samples = (float)rate * fTime;              // a named product: VC7.1 otherwise re-associates `a * b / c` into `a / c * b`
    return FloatToLongTrunc(samples / fFrameSamples);
}
