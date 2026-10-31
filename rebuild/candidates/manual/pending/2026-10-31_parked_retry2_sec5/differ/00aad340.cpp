// CWaveFile::ConvertSamplingRate(unsigned long newRate) -- linear-interpolation resample of the 16-bit sample vector to a new rate
// Grade: DIFFER(606v584) -- retail_asm candidate: the C++ stand-in floor replaced by the landed __forceinline
//   FloorToLong inline-asm helper (bare dword fistp, 0.5/1.0 frame constants, Retail-asm evidence line); 136 diff lines
//   from 217; residue: frame slot assignment of the three helper expansions and surrounding x87 scheduling (+22 bytes)
// Retail-asm: 0x00AAD381..0x00AAD39B `fld [ebp-20h] ; fsub [ebp-24h] ; fistp dword ptr [ebp-18h] ; fild [ebp-18h] ; fadd [ebp-28h] ; fstp [ebp-1Ch] ; mov eax,[ebp-20h] ; cmp eax,[ebp-1Ch] ; jne ; inc dword ptr [ebp-18h]` -- the engine's inline-assembly floor helper expanded in place (same block as GFFloatToLongFloor 0x0048D940 and the landed FloorToLong of CThingPhysical::ModifyHealth 0x006A68F0); VC7.1 emits no bare dword fistp from C++.
// Retail: Fable.exe (TLC, VC7.1) @ 0x00AAD340, 584 bytes, `ret 4`.
// Callees: vector<short>(n) ctor 0x00AADD80, vector<short>::swap-assign 0x00AADE80, free 0x00BFEA14.
// Layout: CWaveFile { unsigned long Channels +0; unsigned long NumSamples +4; unsigned long SampleRate +8; vector<short> Samples +0xc (12 bytes) }.
#include <stdlib.h>
struct CShortVector {
    short* _M_start; short* _M_finish; short* _M_end_of_storage;
    CShortVector(unsigned long n);          // 0x00AADD80
    void Assign(CShortVector& other);       // 0x00AADE80
    ~CShortVector() { if (_M_start) free(_M_start); }
};
// The engine's inline-assembly FPU floor helper (standalone copy GFFloatToLongFloor 0x0048D940; the same __forceinline
// FloorToLong as the landed CThingPhysical::ModifyHealth 0x006A68F0 / CStatBar::DrawStatBarOnLeft 0x00646010).
__forceinline long FloorToLong(float value)
{
    long  result;
    float one;
    float half;
    float next;
    half = 0.5f;
    one = 1.0f;
    __asm {
        fld   value
        fsub  half
        fistp result
        fild  result
        fadd  one
        fstp  next
        mov   eax, value
        cmp   eax, next
        jne   done
        inc   result
    done:
    }
    return result;
}
struct CWaveFile {
    unsigned long Channels;
    unsigned long NumSamples;
    unsigned long SampleRate;
    CShortVector Samples;
    void ConvertSamplingRate(unsigned long newRate);
};
void CWaveFile::ConvertSamplingRate(unsigned long newRate)
{
    float ratio = (float)SampleRate / (float)newRate;
    unsigned long step = FloorToLong(ratio);
    unsigned long fracStep = FloorToLong((ratio - (float)step) * 65536.0f);
    unsigned long newCount = FloorToLong((float)((double)NumSamples * (double)newRate / (double)SampleRate));
    CShortVector out(Channels * newCount);
    unsigned long pos = 0, frac = 0, inv = 0x10000, next = 1;
    short* dst = out._M_finish - out._M_start > 0 ? out._M_start : (short*)&out;
    for (unsigned long i = 0; i < newCount; ++i) {
        unsigned long hi = next;
        if (hi > NumSamples - 1) hi = NumSamples - 1;
        for (unsigned long c = 0; c < Channels; ++c) {
            long v = ((long)Samples._M_start[Channels * hi + c] * (long)frac + (long)Samples._M_start[Channels * pos + c] * (long)inv) >> 16;
            float f = (float)v;
            *dst++ = (short)FloorToLong(f);
        }
        pos += step; frac += fracStep; next += step; inv -= fracStep;
        if (frac > 0x10000) { frac -= 0x10000; ++pos; ++next; inv += 0x10000; }
    }
    NumSamples = newCount;
    SampleRate = newRate;
    Samples.Assign(out);
}
