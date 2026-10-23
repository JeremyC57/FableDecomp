// Grade: DIFFER(613v584) -- lane gap: retail inlines an inline-asm fistp floor helper (fld; fsub 0.5; fistp; fild+1 compare) three times in an ebp frame; plain C++ calls _ftol instead
// CWaveFile::ConvertSamplingRate(unsigned long newRate) -- linear-interpolation resample of the 16-bit sample vector to a new rate
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
static inline long FloorToLong(float x)
{
    long r = (long)(x - 0.5f);
    if (x == (float)r + 1.0f) ++r;
    return r;
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
