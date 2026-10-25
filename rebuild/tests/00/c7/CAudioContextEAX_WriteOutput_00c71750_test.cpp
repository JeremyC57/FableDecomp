#include <cstdio>
#include <cstring>
#ifndef _MSC_VER
#define __cdecl
#define __stdcall
#define __fastcall
#define __declspec(x)
typedef long long __int64;
#endif
class CAudioContextBase {                              // vtable 0x012B95C4
public:
    virtual void v00();
    unsigned char _pad004[0x218];
    long*         m_pMix;                              // +0x21c 16-byte-aligned 128-sample mix block
};

class CAudioContextEAX : public CAudioContextBase {    // vtable 0x012BEDF4
public:
    virtual void v00();
    void WriteOutput(short* output);
};

inline short ClampSample(long value)
{
    if (value > 0x7fff)
        return 0x7fff;
    if (value < -0x8000)
        return -0x8000;
    return (short)value;
}

void CAudioContextEAX::WriteOutput(short* output)
{
    for (unsigned long i = 0; i < 128; i += 4) {
        output[i + 0] = ClampSample(m_pMix[i + 0]);
        output[i + 1] = ClampSample(m_pMix[i + 1]);
        output[i + 2] = ClampSample(m_pMix[i + 2]);
        output[i + 3] = ClampSample(m_pMix[i + 3]);
    }
}
static int fail() { std::printf("00c71750_TEST FAIL\n"); return 1; }
static bool nearly(float a, float b) { float d = a - b; if (d < 0) d = -d; return d < 1e-3f; }
void CAudioContextBase::v00() { }
void CAudioContextEAX::v00() { }
int main()
{
    static long mix[128];
    static short out[130];
    for (int i = 0; i < 128; ++i) mix[i] = (i - 64) * 1000;      // -64000 .. 63000
    mix[5] = 32767; mix[6] = 32768; mix[7] = -32768; mix[8] = -32769; mix[9] = 0;
    out[128] = 0x1234;
    CAudioContextEAX eax;
    eax.m_pMix = mix;
    eax.WriteOutput(out);
    for (int i = 0; i < 128; ++i) {
        long v = mix[i]; long want = v > 32767 ? 32767 : v < -32768 ? -32768 : v;
        if (out[i] != (short)want) return fail();
    }
    if (out[5] != 32767 || out[6] != 32767 || out[7] != -32768 || out[8] != -32768 || out[128] != 0x1234) return fail();
    std::printf("00c71750_TEST PASS\n");
    return 0;
}