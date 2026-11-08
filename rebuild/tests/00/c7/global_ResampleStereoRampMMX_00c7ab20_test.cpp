// Behaviour test for 00c7ab20: ResampleStereoRampMMX (libpk MMX polyphase resampler) against a scalar reference FIR
#include <cstdio>
#include <cstring>
#include <mmintrin.h>

// The mix buffers are 32-bit lanes: `long` in the VC7.1 source, spelled Mix32 here so the copy also runs on LP64 g++.
typedef int Mix32;
#ifdef _MSC_VER
#define ALIGN16 __declspec(align(16))
#else
#define ALIGN16 __attribute__((aligned(16)))
#endif

// copy of the source (ivec.h model + function)
class M64 {
protected:
    __m64 vec;
public:
    M64() {}
    M64(__m64 mm) { vec = mm; }
    operator __m64() const { return vec; }
};
class I32vec2 : public M64 {
public:
    I32vec2() {}
    I32vec2(__m64 mm) : M64(mm) {}
};
class Is32vec2 : public I32vec2 {
public:
    Is32vec2() {}
    Is32vec2(__m64 mm) : I32vec2(mm) {}
    Is32vec2& operator+=(const I32vec2& a) { return *this = (Is32vec2)_m_paddd(vec, a); }
    Is32vec2 operator>>(int count) { return _m_psradi(vec, count); }
    Is32vec2& operator>>=(int count) { return *this = (Is32vec2)_m_psradi(vec, count); }
};
class I16vec4 : public M64 {
public:
    I16vec4() {}
    I16vec4(__m64 mm) : M64(mm) {}
};
class Is16vec4 : public I16vec4 {
public:
    Is16vec4() {}
    Is16vec4(__m64 mm) : I16vec4(mm) {}
    Is16vec4(short i3, short i2, short i1, short i0)
    {
        ((short*)&vec)[0] = i0;
        ((short*)&vec)[1] = i1;
        ((short*)&vec)[2] = i2;
        ((short*)&vec)[3] = i3;
    }
};
inline Is32vec2 operator+(const Is32vec2& a, const Is32vec2& b) { return _m_paddd(a, b); }
inline Is32vec2 unpack_low(const Is32vec2& a, const Is32vec2& b) { return _m_punpckldq(a, b); }
inline Is32vec2 unpack_high(const Is32vec2& a, const Is32vec2& b) { return _m_punpckhdq(a, b); }
inline Is16vec4 unpack_low(const Is16vec4& a, const Is16vec4& b) { return _m_punpcklwd(a, b); }
inline Is16vec4 unpack_high(const Is16vec4& a, const Is16vec4& b) { return _m_punpckhwd(a, b); }
inline Is16vec4 pack_sat(const Is32vec2& a, const Is32vec2& b) { return _m_packssdw(a, b); }
inline Is16vec4 mul_high(const Is16vec4& a, const Is16vec4& b) { return _m_pmulhw(a, b); }
inline Is16vec4 sat_add(const Is16vec4& a, const Is16vec4& b) { return _m_paddsw(a, b); }
inline Is32vec2 mul_add(const Is16vec4& a, const Is16vec4& b) { return _m_pmaddwd(a, b); }

void ResampleStereoRampMMX(const short* source, unsigned long phase, unsigned long increment, unsigned long count, Mix32* output1, Mix32* output2, unsigned short panGain, short startGain, short gainStep)
{
    ALIGN16 Is16vec4 gain((short)(startGain + 3 * gainStep), (short)(startGain + 2 * gainStep), (short)(startGain + gainStep), (short)startGain);
    ALIGN16 Is16vec4 step((short)(4 * gainStep), (short)(4 * gainStep), (short)(4 * gainStep), (short)(4 * gainStep));
    ALIGN16 Is16vec4 pan(0, (short)panGain, 0, (short)panGain);
    Is32vec2* first = (Is32vec2*)output1;
    Is32vec2* second = (Is32vec2*)output2;
    const Is16vec4* in = (const Is16vec4*)(source - 32);
    const Is16vec4* c = (const Is16vec4*)phase;

    switch (increment)
    {
    case 0:
        for (unsigned long i = 0; i < count; ++i)
        {
            Is32vec2 s0 = mul_add(in[1], c[1]) + mul_add(in[2], c[2]) + mul_add(in[3], c[3]) + mul_add(in[4], c[4]) + mul_add(in[5], c[5]) + mul_add(in[6], c[6]) + mul_add(in[7], c[7]);
            Is32vec2 s1 = mul_add(in[1], c[9]) + mul_add(in[2], c[10]) + mul_add(in[3], c[11]) + mul_add(in[4], c[12]) + mul_add(in[5], c[13]) + mul_add(in[6], c[14]) + mul_add(in[7], c[15]);
            Is32vec2 s2 = mul_add(in[1], c[17]) + mul_add(in[2], c[18]) + mul_add(in[3], c[19]) + mul_add(in[4], c[20]) + mul_add(in[5], c[21]) + mul_add(in[6], c[22]) + mul_add(in[7], c[23]);
            Is32vec2 s3 = mul_add(in[1], c[25]) + mul_add(in[2], c[26]) + mul_add(in[3], c[27]) + mul_add(in[4], c[28]) + mul_add(in[5], c[29]) + mul_add(in[6], c[30]) + mul_add(in[7], c[31]);
            Is16vec4 r = mul_high(pack_sat((unpack_low(s0, s1) + unpack_high(s0, s1)) >> 15, (unpack_low(s2, s3) + unpack_high(s2, s3)) >> 15), gain);
            gain = sat_add(gain, step);
            Is32vec2 a = Is32vec2(_m_punpcklwd(_mm_setzero_si64(), r)) >> 14;
            *first++ += a;
            *second++ += mul_add(Is16vec4(a >> 1), pan) >> 14;
            Is32vec2 b = Is32vec2(_m_punpckhwd(_mm_setzero_si64(), r)) >> 14;
            *first++ += b;
            *second++ += mul_add(Is16vec4(b >> 1), pan) >> 14;
            ++in;
        }
        break;
    case 1:
        for (unsigned long i = 0; i < count; ++i)
        {
            Is32vec2 s0 = mul_add(in[0], c[0]) + mul_add(in[1], c[1]) + mul_add(in[2], c[2]) + mul_add(in[3], c[3]) + mul_add(in[4], c[4]) + mul_add(in[5], c[5]) + mul_add(in[6], c[6]);
            Is32vec2 s1 = mul_add(in[1], c[9]) + mul_add(in[2], c[10]) + mul_add(in[3], c[11]) + mul_add(in[4], c[12]) + mul_add(in[5], c[13]) + mul_add(in[6], c[14]) + mul_add(in[7], c[15]);
            Is32vec2 s2 = mul_add(in[1], c[17]) + mul_add(in[2], c[18]) + mul_add(in[3], c[19]) + mul_add(in[4], c[20]) + mul_add(in[5], c[21]) + mul_add(in[6], c[22]) + mul_add(in[7], c[23]);
            Is32vec2 s3 = mul_add(in[1], c[25]) + mul_add(in[2], c[26]) + mul_add(in[3], c[27]) + mul_add(in[4], c[28]) + mul_add(in[5], c[29]) + mul_add(in[6], c[30]) + mul_add(in[7], c[31]);
            Is16vec4 r = mul_high(pack_sat((unpack_low(s0, s1) + unpack_high(s0, s1)) >> 15, (unpack_low(s2, s3) + unpack_high(s2, s3)) >> 15), gain);
            gain = sat_add(gain, step);
            Is32vec2 a = Is32vec2(_m_punpcklwd(_mm_setzero_si64(), r)) >> 14;
            *first++ += a;
            *second++ += mul_add(Is16vec4(a >> 1), pan) >> 14;
            Is32vec2 b = Is32vec2(_m_punpckhwd(_mm_setzero_si64(), r)) >> 14;
            *first++ += b;
            *second++ += mul_add(Is16vec4(b >> 1), pan) >> 14;
            ++in;
        }
        break;
    case 2:
        for (unsigned long i = 0; i < count; ++i)
        {
            Is32vec2 s0 = mul_add(in[0], c[0]) + mul_add(in[1], c[1]) + mul_add(in[2], c[2]) + mul_add(in[3], c[3]) + mul_add(in[4], c[4]) + mul_add(in[5], c[5]) + mul_add(in[6], c[6]);
            Is32vec2 s1 = mul_add(in[0], c[8]) + mul_add(in[1], c[9]) + mul_add(in[2], c[10]) + mul_add(in[3], c[11]) + mul_add(in[4], c[12]) + mul_add(in[5], c[13]) + mul_add(in[6], c[14]);
            Is32vec2 s2 = mul_add(in[1], c[17]) + mul_add(in[2], c[18]) + mul_add(in[3], c[19]) + mul_add(in[4], c[20]) + mul_add(in[5], c[21]) + mul_add(in[6], c[22]) + mul_add(in[7], c[23]);
            Is32vec2 s3 = mul_add(in[1], c[25]) + mul_add(in[2], c[26]) + mul_add(in[3], c[27]) + mul_add(in[4], c[28]) + mul_add(in[5], c[29]) + mul_add(in[6], c[30]) + mul_add(in[7], c[31]);
            Is16vec4 r = mul_high(pack_sat((unpack_low(s0, s1) + unpack_high(s0, s1)) >> 15, (unpack_low(s2, s3) + unpack_high(s2, s3)) >> 15), gain);
            gain = sat_add(gain, step);
            Is32vec2 a = Is32vec2(_m_punpcklwd(_mm_setzero_si64(), r)) >> 14;
            *first++ += a;
            *second++ += mul_add(Is16vec4(a >> 1), pan) >> 14;
            Is32vec2 b = Is32vec2(_m_punpckhwd(_mm_setzero_si64(), r)) >> 14;
            *first++ += b;
            *second++ += mul_add(Is16vec4(b >> 1), pan) >> 14;
            ++in;
        }
        break;
    case 3:
        for (unsigned long i = 0; i < count; ++i)
        {
            Is32vec2 s0 = mul_add(in[0], c[0]) + mul_add(in[1], c[1]) + mul_add(in[2], c[2]) + mul_add(in[3], c[3]) + mul_add(in[4], c[4]) + mul_add(in[5], c[5]) + mul_add(in[6], c[6]);
            Is32vec2 s1 = mul_add(in[0], c[8]) + mul_add(in[1], c[9]) + mul_add(in[2], c[10]) + mul_add(in[3], c[11]) + mul_add(in[4], c[12]) + mul_add(in[5], c[13]) + mul_add(in[6], c[14]);
            Is32vec2 s2 = mul_add(in[0], c[16]) + mul_add(in[1], c[17]) + mul_add(in[2], c[18]) + mul_add(in[3], c[19]) + mul_add(in[4], c[20]) + mul_add(in[5], c[21]) + mul_add(in[6], c[22]);
            Is32vec2 s3 = mul_add(in[1], c[25]) + mul_add(in[2], c[26]) + mul_add(in[3], c[27]) + mul_add(in[4], c[28]) + mul_add(in[5], c[29]) + mul_add(in[6], c[30]) + mul_add(in[7], c[31]);
            Is16vec4 r = mul_high(pack_sat((unpack_low(s0, s1) + unpack_high(s0, s1)) >> 15, (unpack_low(s2, s3) + unpack_high(s2, s3)) >> 15), gain);
            gain = sat_add(gain, step);
            Is32vec2 a = Is32vec2(_m_punpcklwd(_mm_setzero_si64(), r)) >> 14;
            *first++ += a;
            *second++ += mul_add(Is16vec4(a >> 1), pan) >> 14;
            Is32vec2 b = Is32vec2(_m_punpckhwd(_mm_setzero_si64(), r)) >> 14;
            *first++ += b;
            *second++ += mul_add(Is16vec4(b >> 1), pan) >> 14;
            ++in;
        }
        break;
    }
    _mm_empty();
}


static unsigned long g_seed = 12345;
static int Rand(int range) { g_seed = g_seed * 1103515245UL + 12345UL; return (int)((g_seed >> 8) % (unsigned long)(2 * range + 1)) - range; }
static int Sat16(int v) { return v > 32767 ? 32767 : (v < -32768 ? -32768 : v); }

static __m64 g_src[8 + 16 + 8];
static __m64 g_coef[32];
static Mix32 g_out[4 * 16];
static Mix32 g_ref[4 * 16];
static Mix32 g_out2[4 * 16];
static Mix32 g_ref2[4 * 16];

static int Madd(const short* a, const short* b, int lane) { return a[2 * lane] * b[2 * lane] + a[2 * lane + 1] * b[2 * lane + 1]; }

// Scalar model: output step i reads window quads in[i+b .. i+b+6] (in = source - 32 shorts) against coefficient row p
// (quads 8p+b .. 8p+b+6), b = 0 for the first `increment` rows and 1 for the rest; the two pmaddwd lanes of each
// row sum fold into one 32-bit result per row.
static void Reference(const short* source, const short* coef, unsigned long increment, unsigned long count, Mix32* out,
                      int ramp, short startGain, short gainStep, Mix32* out2, unsigned short panGain)
{
    const short* in = source - 32;
    short gain[4];
    int k;
    for (k = 0; k < 4; ++k) gain[k] = (short)(startGain + k * gainStep);
    for (unsigned long i = 0; i < count; ++i) {
        int row[4];
        for (int p = 0; p < 4; ++p) {
            int b = (unsigned long)p < increment ? 0 : 1;
            int lo = 0, hi = 0;
            for (int t = 0; t < 7; ++t) {
                const short* x = in + 4 * (i + b + t);
                const short* c = coef + 4 * (8 * p + b + t);
                lo += Madd(x, c, 0); hi += Madd(x, c, 1);
            }
            row[p] = lo + hi;
        }
        for (k = 0; k < 4; ++k) {
            int v;
            if (ramp) {
                int r = Sat16(row[k] >> 15);
                int m = (r * gain[k]) >> 16;
                v = m * 4;
            } else
                v = row[k] >> 14;
            out[4 * i + k] += v;
            if (out2)       // second output: the low word of (v >> 1) times the pan gain, >> 14 (pmaddwd against {pan, 0})
                out2[4 * i + k] += ((int)(short)((v >> 1) & 0xffff) * (int)(short)panGain) >> 14;
        }
        if (ramp)
            for (k = 0; k < 4; ++k) gain[k] = (short)Sat16(gain[k] + (short)(4 * gainStep));
    }
}

static int RunCase(unsigned long increment, unsigned long count, int range, short startGain, short gainStep, unsigned short panGain)
{
    short* s = (short*)g_src;
    short* c = (short*)g_coef;
    int k;
    for (k = 0; k < 4 * (8 + 16 + 8); ++k) s[k] = (short)Rand(range);
    for (k = 0; k < 4 * 32; ++k) c[k] = (short)Rand(range);
    for (k = 0; k < 4 * 16; ++k) { g_out[k] = g_ref[k] = Rand(100000); g_out2[k] = g_ref2[k] = Rand(100000); }
    const short* source = s + 32;
    ResampleStereoRampMMX(source, (unsigned long)c, increment, count, g_out, g_out2, panGain, startGain, gainStep);
    Reference(source, c, increment, count, g_ref, 1, startGain, gainStep, g_ref2, panGain);
    for (k = 0; k < 4 * 16; ++k)
        if (g_out[k] != g_ref[k] || g_out2[k] != g_ref2[k]) { std::printf("mismatch inc=%lu k=%d got %d/%d want %d/%d\n", increment, k, g_out[k], g_out2[k], g_ref[k], g_ref2[k]); return 0; }
    return 1;
}

int main()
{
    int ok = 1;
    for (unsigned long inc = 0; inc < 4; ++inc) {
        ok &= RunCase(inc, 13, 4000, 0, 0x100, 0x4000);
        ok &= RunCase(inc, 16, 16000, 30000, 1000, 0x7fff);
        ok &= RunCase(inc, 9, 16000, -20000, -3000, 0x9000);
    }
    ok &= RunCase(2, 0, 16000, 5, 5, 0x1234);               // count 0 leaves the output untouched
    if (!ok) { std::printf("00c7ab20_TEST FAIL\n"); return 1; }
    std::printf("00c7ab20_TEST PASS\n");
    return 0;
}