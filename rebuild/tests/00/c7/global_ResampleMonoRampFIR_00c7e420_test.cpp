// Behaviour test for 0x00C7E420 ResampleMonoRampFIR: runs every increment case for fade-in, fade-out and a saturating
// ramp and checks each accumulated output phase against a scalar model (16-tap dot product, >> 15, low 16 bits times
// the lane gain, >> 14; gains advance by 4*step per block with signed 16-bit saturation). C++03; g++ LP64 and VC7.1 /Od.
#include <cstdio>
#include <emmintrin.h>

#define _MM_8W(element, vector) (*((signed short*)&(vector) + (element)))

class M128 {
protected:
    __m128i vec;                                        // +0x00
public:
    M128() {}
    M128(__m128i mm) { vec = mm; }
    operator __m128i() const { return vec; }
};
class I32vec4 : public M128 {
public:
    I32vec4() {}
    I32vec4(__m128i mm) : M128(mm) {}
    I32vec4& operator=(const M128& a) { return *this = (I32vec4)a; }
    I32vec4& operator+=(const I32vec4& a) { return *this = (I32vec4)_mm_add_epi32(vec, a); }
};
class Is32vec4 : public I32vec4 {
public:
    Is32vec4() {}
    Is32vec4(__m128i mm) : I32vec4(mm) {}
    Is32vec4& operator=(const M128& a) { return *this = (Is32vec4)a; }
    Is32vec4& operator+=(const I32vec4& a) { return *this = (Is32vec4)_mm_add_epi32(vec, a); }     // 0x00C7B870
    Is32vec4 operator>>(int count) { return _mm_srai_epi32(vec, count); }                           // 0x00C7B8B0
    Is32vec4& operator>>=(int count) { return *this = (Is32vec4)_mm_srai_epi32(vec, count); }       // 0x00C7B8F0
};
class I16vec8 : public M128 {
public:
    I16vec8() {}
    I16vec8(__m128i mm) : M128(mm) {}
    I16vec8& operator=(const M128& a) { return *this = (I16vec8)a; }
};
class Is16vec8 : public I16vec8 {
public:
    Is16vec8() {}
    Is16vec8(__m128i mm) : I16vec8(mm) {}
    Is16vec8(signed short s7, signed short s6, signed short s5, signed short s4,
             signed short s3, signed short s2, signed short s1, signed short s0)
    {
        _MM_8W(0, vec) = s0; _MM_8W(1, vec) = s1; _MM_8W(2, vec) = s2; _MM_8W(3, vec) = s3;
        _MM_8W(4, vec) = s4; _MM_8W(5, vec) = s5; _MM_8W(6, vec) = s6; _MM_8W(7, vec) = s7;
    }
    Is16vec8& operator=(const M128& a) { return *this = (Is16vec8)a; }
};

void ResampleMonoRampFIR(const short* source, unsigned long phase, unsigned long increment, unsigned long count, long* output,
                         short startGain, short gainStep)
{
    Is16vec8 gain(0, (signed short)(startGain + 3 * gainStep), 0, (signed short)(startGain + 2 * gainStep),
                  0, (signed short)(startGain + gainStep), 0, (signed short)startGain);
    Is16vec8 step(0, (signed short)(4 * gainStep), 0, (signed short)(4 * gainStep),
                  0, (signed short)(4 * gainStep), 0, (signed short)(4 * gainStep));
    const Is16vec8* in = (const Is16vec8*)(source - 40);      // window: five 8-sample quads behind the read point
    const Is16vec8* c = (const Is16vec8*)phase;               // 40 coefficient quads, 5 per output phase
    Is32vec4* out = (Is32vec4*)output;
    switch (increment)
    {
    case 0:
        for (unsigned long i = 0; i < count; ++i)
        {
            Is32vec4 s0 = _mm_madd_epi16(in[1], c[21]);
            Is32vec4 s1 = _mm_madd_epi16(in[1], c[26]);
            Is32vec4 s2 = _mm_madd_epi16(in[1], c[31]);
            Is32vec4 s3 = _mm_madd_epi16(in[1], c[36]);
            s0 += _mm_madd_epi16(in[2], c[22]);
            s1 += _mm_madd_epi16(in[2], c[27]);
            s2 += _mm_madd_epi16(in[2], c[32]);
            s3 += _mm_madd_epi16(in[2], c[37]);
            s0 += _mm_madd_epi16(in[3], c[23]);
            s1 += _mm_madd_epi16(in[3], c[28]);
            s2 += _mm_madd_epi16(in[3], c[33]);
            s3 += _mm_madd_epi16(in[3], c[38]);
            s0 += _mm_madd_epi16(in[4], c[24]);
            s1 += _mm_madd_epi16(in[4], c[29]);
            s2 += _mm_madd_epi16(in[4], c[34]);
            s3 += _mm_madd_epi16(in[4], c[39]);
            Is32vec4 a = _mm_add_epi32(_mm_unpacklo_epi32(s0, s2), _mm_unpackhi_epi32(s0, s2));
            Is32vec4 b = _mm_add_epi32(_mm_unpacklo_epi32(s1, s3), _mm_unpackhi_epi32(s1, s3));
            Is32vec4 sum = _mm_add_epi32(Is32vec4(_mm_unpacklo_epi32(a, b)), Is32vec4(_mm_unpackhi_epi32(a, b)));
            sum >>= 15;
            *out = Is32vec4(_mm_add_epi32(*out, Is32vec4(_mm_madd_epi16(sum, gain)) >> 14));
            gain = Is16vec8(_mm_adds_epi16(gain, step));
            ++out;
            Is32vec4 t0 = _mm_madd_epi16(in[2], c[1]);
            Is32vec4 t1 = _mm_madd_epi16(in[2], c[6]);
            Is32vec4 t2 = _mm_madd_epi16(in[2], c[11]);
            Is32vec4 t3 = _mm_madd_epi16(in[2], c[16]);
            t0 = Is32vec4(_mm_add_epi32(t0, Is32vec4(_mm_madd_epi16(in[3], c[2]))));
            t1 = Is32vec4(_mm_add_epi32(t1, Is32vec4(_mm_madd_epi16(in[3], c[7]))));
            t2 = Is32vec4(_mm_add_epi32(t2, Is32vec4(_mm_madd_epi16(in[3], c[12]))));
            t3 = Is32vec4(_mm_add_epi32(t3, Is32vec4(_mm_madd_epi16(in[3], c[17]))));
            t0 = Is32vec4(_mm_add_epi32(t0, Is32vec4(_mm_madd_epi16(in[4], c[3]))));
            t1 = Is32vec4(_mm_add_epi32(t1, Is32vec4(_mm_madd_epi16(in[4], c[8]))));
            t2 = Is32vec4(_mm_add_epi32(t2, Is32vec4(_mm_madd_epi16(in[4], c[13]))));
            t3 = Is32vec4(_mm_add_epi32(t3, Is32vec4(_mm_madd_epi16(in[4], c[18]))));
            t0 += _mm_madd_epi16(in[5], c[4]);
            t1 += _mm_madd_epi16(in[5], c[9]);
            t2 += _mm_madd_epi16(in[5], c[14]);
            t3 += _mm_madd_epi16(in[5], c[19]);
            Is32vec4 e = _mm_add_epi32(_mm_unpacklo_epi32(t0, t2), _mm_unpackhi_epi32(t0, t2));
            Is32vec4 f = _mm_add_epi32(_mm_unpacklo_epi32(t1, t3), _mm_unpackhi_epi32(t1, t3));
            Is32vec4 sum2 = _mm_add_epi32(Is32vec4(_mm_unpacklo_epi32(e, f)), Is32vec4(_mm_unpackhi_epi32(e, f)));
            sum2 >>= 15;
            *out = Is32vec4(_mm_add_epi32(*out, Is32vec4(_mm_madd_epi16(sum2, gain)) >> 14));
            gain = Is16vec8(_mm_adds_epi16(gain, step));
            ++out;
            ++in;
        }
        break;
    case 1:
        for (unsigned long i = 0; i < count; ++i)
        {
            Is32vec4 s0 = _mm_madd_epi16(in[1], c[21]);
            Is32vec4 s1 = _mm_madd_epi16(in[1], c[26]);
            Is32vec4 s2 = _mm_madd_epi16(in[1], c[31]);
            Is32vec4 s3 = _mm_madd_epi16(in[1], c[36]);
            s0 += _mm_madd_epi16(in[2], c[22]);
            s1 += _mm_madd_epi16(in[2], c[27]);
            s2 += _mm_madd_epi16(in[2], c[32]);
            s3 += _mm_madd_epi16(in[2], c[37]);
            s0 += _mm_madd_epi16(in[3], c[23]);
            s1 += _mm_madd_epi16(in[3], c[28]);
            s2 += _mm_madd_epi16(in[3], c[33]);
            s3 += _mm_madd_epi16(in[3], c[38]);
            s0 += _mm_madd_epi16(in[4], c[24]);
            s1 += _mm_madd_epi16(in[4], c[29]);
            s2 += _mm_madd_epi16(in[4], c[34]);
            s3 += _mm_madd_epi16(in[4], c[39]);
            Is32vec4 a = _mm_add_epi32(_mm_unpacklo_epi32(s0, s2), _mm_unpackhi_epi32(s0, s2));
            Is32vec4 b = _mm_add_epi32(_mm_unpacklo_epi32(s1, s3), _mm_unpackhi_epi32(s1, s3));
            Is32vec4 sum = _mm_add_epi32(Is32vec4(_mm_unpacklo_epi32(a, b)), Is32vec4(_mm_unpackhi_epi32(a, b)));
            sum >>= 15;
            *out = Is32vec4(_mm_add_epi32(*out, Is32vec4(_mm_madd_epi16(sum, gain)) >> 14));
            gain = Is16vec8(_mm_adds_epi16(gain, step));
            ++out;
            Is32vec4 t0 = _mm_madd_epi16(in[1], c[0]);
            t0 += _mm_madd_epi16(in[2], c[1]);
            Is32vec4 t1 = _mm_madd_epi16(in[2], c[6]);
            Is32vec4 t2 = _mm_madd_epi16(in[2], c[11]);
            Is32vec4 t3 = _mm_madd_epi16(in[2], c[16]);
            t0 = Is32vec4(_mm_add_epi32(t0, Is32vec4(_mm_madd_epi16(in[3], c[2]))));
            t1 = Is32vec4(_mm_add_epi32(t1, Is32vec4(_mm_madd_epi16(in[3], c[7]))));
            t2 = Is32vec4(_mm_add_epi32(t2, Is32vec4(_mm_madd_epi16(in[3], c[12]))));
            t3 = Is32vec4(_mm_add_epi32(t3, Is32vec4(_mm_madd_epi16(in[3], c[17]))));
            t0 = Is32vec4(_mm_add_epi32(t0, Is32vec4(_mm_madd_epi16(in[4], c[3]))));
            t1 = Is32vec4(_mm_add_epi32(t1, Is32vec4(_mm_madd_epi16(in[4], c[8]))));
            t2 = Is32vec4(_mm_add_epi32(t2, Is32vec4(_mm_madd_epi16(in[4], c[13]))));
            t3 = Is32vec4(_mm_add_epi32(t3, Is32vec4(_mm_madd_epi16(in[4], c[18]))));
            t1 += _mm_madd_epi16(in[5], c[9]);
            t2 += _mm_madd_epi16(in[5], c[14]);
            t3 += _mm_madd_epi16(in[5], c[19]);
            Is32vec4 e = _mm_add_epi32(_mm_unpacklo_epi32(t0, t2), _mm_unpackhi_epi32(t0, t2));
            Is32vec4 f = _mm_add_epi32(_mm_unpacklo_epi32(t1, t3), _mm_unpackhi_epi32(t1, t3));
            Is32vec4 sum2 = _mm_add_epi32(Is32vec4(_mm_unpacklo_epi32(e, f)), Is32vec4(_mm_unpackhi_epi32(e, f)));
            sum2 >>= 15;
            *out = Is32vec4(_mm_add_epi32(*out, Is32vec4(_mm_madd_epi16(sum2, gain)) >> 14));
            gain = Is16vec8(_mm_adds_epi16(gain, step));
            ++out;
            ++in;
        }
        break;
    case 2:
        for (unsigned long i = 0; i < count; ++i)
        {
            Is32vec4 s0 = _mm_madd_epi16(in[1], c[21]);
            Is32vec4 s1 = _mm_madd_epi16(in[1], c[26]);
            Is32vec4 s2 = _mm_madd_epi16(in[1], c[31]);
            Is32vec4 s3 = _mm_madd_epi16(in[1], c[36]);
            s0 += _mm_madd_epi16(in[2], c[22]);
            s1 += _mm_madd_epi16(in[2], c[27]);
            s2 += _mm_madd_epi16(in[2], c[32]);
            s3 += _mm_madd_epi16(in[2], c[37]);
            s0 += _mm_madd_epi16(in[3], c[23]);
            s1 += _mm_madd_epi16(in[3], c[28]);
            s2 += _mm_madd_epi16(in[3], c[33]);
            s3 += _mm_madd_epi16(in[3], c[38]);
            s0 += _mm_madd_epi16(in[4], c[24]);
            s1 += _mm_madd_epi16(in[4], c[29]);
            s2 += _mm_madd_epi16(in[4], c[34]);
            s3 += _mm_madd_epi16(in[4], c[39]);
            Is32vec4 a = _mm_add_epi32(_mm_unpacklo_epi32(s0, s2), _mm_unpackhi_epi32(s0, s2));
            Is32vec4 b = _mm_add_epi32(_mm_unpacklo_epi32(s1, s3), _mm_unpackhi_epi32(s1, s3));
            Is32vec4 sum = _mm_add_epi32(Is32vec4(_mm_unpacklo_epi32(a, b)), Is32vec4(_mm_unpackhi_epi32(a, b)));
            sum >>= 15;
            *out = Is32vec4(_mm_add_epi32(*out, Is32vec4(_mm_madd_epi16(sum, gain)) >> 14));
            gain = Is16vec8(_mm_adds_epi16(gain, step));
            ++out;
            Is32vec4 t0 = _mm_madd_epi16(in[1], c[0]);
            Is32vec4 t1 = _mm_madd_epi16(in[1], c[5]);
            t0 += _mm_madd_epi16(in[2], c[1]);
            t1 += _mm_madd_epi16(in[2], c[6]);
            Is32vec4 t2 = _mm_madd_epi16(in[2], c[11]);
            Is32vec4 t3 = _mm_madd_epi16(in[2], c[16]);
            t0 = Is32vec4(_mm_add_epi32(t0, Is32vec4(_mm_madd_epi16(in[3], c[2]))));
            t1 = Is32vec4(_mm_add_epi32(t1, Is32vec4(_mm_madd_epi16(in[3], c[7]))));
            t2 = Is32vec4(_mm_add_epi32(t2, Is32vec4(_mm_madd_epi16(in[3], c[12]))));
            t3 = Is32vec4(_mm_add_epi32(t3, Is32vec4(_mm_madd_epi16(in[3], c[17]))));
            t0 = Is32vec4(_mm_add_epi32(t0, Is32vec4(_mm_madd_epi16(in[4], c[3]))));
            t1 = Is32vec4(_mm_add_epi32(t1, Is32vec4(_mm_madd_epi16(in[4], c[8]))));
            t2 = Is32vec4(_mm_add_epi32(t2, Is32vec4(_mm_madd_epi16(in[4], c[13]))));
            t3 = Is32vec4(_mm_add_epi32(t3, Is32vec4(_mm_madd_epi16(in[4], c[18]))));
            t2 += _mm_madd_epi16(in[5], c[14]);
            t3 += _mm_madd_epi16(in[5], c[19]);
            Is32vec4 e = _mm_add_epi32(_mm_unpacklo_epi32(t0, t2), _mm_unpackhi_epi32(t0, t2));
            Is32vec4 f = _mm_add_epi32(_mm_unpacklo_epi32(t1, t3), _mm_unpackhi_epi32(t1, t3));
            Is32vec4 sum2 = _mm_add_epi32(Is32vec4(_mm_unpacklo_epi32(e, f)), Is32vec4(_mm_unpackhi_epi32(e, f)));
            sum2 >>= 15;
            *out = Is32vec4(_mm_add_epi32(*out, Is32vec4(_mm_madd_epi16(sum2, gain)) >> 14));
            gain = Is16vec8(_mm_adds_epi16(gain, step));
            ++out;
            ++in;
        }
        break;
    case 3:
        for (unsigned long i = 0; i < count; ++i)
        {
            Is32vec4 s0 = _mm_madd_epi16(in[1], c[21]);
            Is32vec4 s1 = _mm_madd_epi16(in[1], c[26]);
            Is32vec4 s2 = _mm_madd_epi16(in[1], c[31]);
            Is32vec4 s3 = _mm_madd_epi16(in[1], c[36]);
            s0 += _mm_madd_epi16(in[2], c[22]);
            s1 += _mm_madd_epi16(in[2], c[27]);
            s2 += _mm_madd_epi16(in[2], c[32]);
            s3 += _mm_madd_epi16(in[2], c[37]);
            s0 += _mm_madd_epi16(in[3], c[23]);
            s1 += _mm_madd_epi16(in[3], c[28]);
            s2 += _mm_madd_epi16(in[3], c[33]);
            s3 += _mm_madd_epi16(in[3], c[38]);
            s0 += _mm_madd_epi16(in[4], c[24]);
            s1 += _mm_madd_epi16(in[4], c[29]);
            s2 += _mm_madd_epi16(in[4], c[34]);
            s3 += _mm_madd_epi16(in[4], c[39]);
            Is32vec4 a = _mm_add_epi32(_mm_unpacklo_epi32(s0, s2), _mm_unpackhi_epi32(s0, s2));
            Is32vec4 b = _mm_add_epi32(_mm_unpacklo_epi32(s1, s3), _mm_unpackhi_epi32(s1, s3));
            Is32vec4 sum = _mm_add_epi32(Is32vec4(_mm_unpacklo_epi32(a, b)), Is32vec4(_mm_unpackhi_epi32(a, b)));
            sum >>= 15;
            *out = Is32vec4(_mm_add_epi32(*out, Is32vec4(_mm_madd_epi16(sum, gain)) >> 14));
            gain = Is16vec8(_mm_adds_epi16(gain, step));
            ++out;
            Is32vec4 t0 = _mm_madd_epi16(in[1], c[0]);
            Is32vec4 t1 = _mm_madd_epi16(in[1], c[5]);
            Is32vec4 t2 = _mm_madd_epi16(in[1], c[10]);
            t0 += _mm_madd_epi16(in[2], c[1]);
            t1 += _mm_madd_epi16(in[2], c[6]);
            t2 += _mm_madd_epi16(in[2], c[11]);
            Is32vec4 t3 = _mm_madd_epi16(in[2], c[16]);
            t0 = Is32vec4(_mm_add_epi32(t0, Is32vec4(_mm_madd_epi16(in[3], c[2]))));
            t1 = Is32vec4(_mm_add_epi32(t1, Is32vec4(_mm_madd_epi16(in[3], c[7]))));
            t2 = Is32vec4(_mm_add_epi32(t2, Is32vec4(_mm_madd_epi16(in[3], c[12]))));
            t3 = Is32vec4(_mm_add_epi32(t3, Is32vec4(_mm_madd_epi16(in[3], c[17]))));
            t0 = Is32vec4(_mm_add_epi32(t0, Is32vec4(_mm_madd_epi16(in[4], c[3]))));
            t1 = Is32vec4(_mm_add_epi32(t1, Is32vec4(_mm_madd_epi16(in[4], c[8]))));
            t2 = Is32vec4(_mm_add_epi32(t2, Is32vec4(_mm_madd_epi16(in[4], c[13]))));
            t3 = Is32vec4(_mm_add_epi32(t3, Is32vec4(_mm_madd_epi16(in[4], c[18]))));
            t3 += _mm_madd_epi16(in[5], c[19]);
            Is32vec4 e = _mm_add_epi32(_mm_unpacklo_epi32(t0, t2), _mm_unpackhi_epi32(t0, t2));
            Is32vec4 f = _mm_add_epi32(_mm_unpacklo_epi32(t1, t3), _mm_unpackhi_epi32(t1, t3));
            Is32vec4 sum2 = _mm_add_epi32(Is32vec4(_mm_unpacklo_epi32(e, f)), Is32vec4(_mm_unpackhi_epi32(e, f)));
            sum2 >>= 15;
            *out = Is32vec4(_mm_add_epi32(*out, Is32vec4(_mm_madd_epi16(sum2, gain)) >> 14));
            gain = Is16vec8(_mm_adds_epi16(gain, step));
            ++out;
            ++in;
        }
        break;
    case 4:
        for (unsigned long i = 0; i < count; ++i)
        {
            Is32vec4 s0 = _mm_madd_epi16(in[1], c[21]);
            Is32vec4 s1 = _mm_madd_epi16(in[1], c[26]);
            Is32vec4 s2 = _mm_madd_epi16(in[1], c[31]);
            Is32vec4 s3 = _mm_madd_epi16(in[1], c[36]);
            s0 += _mm_madd_epi16(in[2], c[22]);
            s1 += _mm_madd_epi16(in[2], c[27]);
            s2 += _mm_madd_epi16(in[2], c[32]);
            s3 += _mm_madd_epi16(in[2], c[37]);
            s0 += _mm_madd_epi16(in[3], c[23]);
            s1 += _mm_madd_epi16(in[3], c[28]);
            s2 += _mm_madd_epi16(in[3], c[33]);
            s3 += _mm_madd_epi16(in[3], c[38]);
            s0 += _mm_madd_epi16(in[4], c[24]);
            s1 += _mm_madd_epi16(in[4], c[29]);
            s2 += _mm_madd_epi16(in[4], c[34]);
            s3 += _mm_madd_epi16(in[4], c[39]);
            Is32vec4 a = _mm_add_epi32(_mm_unpacklo_epi32(s0, s2), _mm_unpackhi_epi32(s0, s2));
            Is32vec4 b = _mm_add_epi32(_mm_unpacklo_epi32(s1, s3), _mm_unpackhi_epi32(s1, s3));
            Is32vec4 sum = _mm_add_epi32(Is32vec4(_mm_unpacklo_epi32(a, b)), Is32vec4(_mm_unpackhi_epi32(a, b)));
            sum >>= 15;
            *out = Is32vec4(_mm_add_epi32(*out, Is32vec4(_mm_madd_epi16(sum, gain)) >> 14));
            gain = Is16vec8(_mm_adds_epi16(gain, step));
            ++out;
            Is32vec4 t0 = _mm_madd_epi16(in[1], c[0]);
            Is32vec4 t1 = _mm_madd_epi16(in[1], c[5]);
            Is32vec4 t2 = _mm_madd_epi16(in[1], c[10]);
            Is32vec4 t3 = _mm_madd_epi16(in[1], c[15]);
            t0 += _mm_madd_epi16(in[2], c[1]);
            t1 += _mm_madd_epi16(in[2], c[6]);
            t2 += _mm_madd_epi16(in[2], c[11]);
            t3 += _mm_madd_epi16(in[2], c[16]);
            t0 = Is32vec4(_mm_add_epi32(t0, Is32vec4(_mm_madd_epi16(in[3], c[2]))));
            t1 = Is32vec4(_mm_add_epi32(t1, Is32vec4(_mm_madd_epi16(in[3], c[7]))));
            t2 = Is32vec4(_mm_add_epi32(t2, Is32vec4(_mm_madd_epi16(in[3], c[12]))));
            t3 = Is32vec4(_mm_add_epi32(t3, Is32vec4(_mm_madd_epi16(in[3], c[17]))));
            t0 = Is32vec4(_mm_add_epi32(t0, Is32vec4(_mm_madd_epi16(in[4], c[3]))));
            t1 = Is32vec4(_mm_add_epi32(t1, Is32vec4(_mm_madd_epi16(in[4], c[8]))));
            t2 = Is32vec4(_mm_add_epi32(t2, Is32vec4(_mm_madd_epi16(in[4], c[13]))));
            t3 = Is32vec4(_mm_add_epi32(t3, Is32vec4(_mm_madd_epi16(in[4], c[18]))));
            Is32vec4 e = _mm_add_epi32(_mm_unpacklo_epi32(t0, t2), _mm_unpackhi_epi32(t0, t2));
            Is32vec4 f = _mm_add_epi32(_mm_unpacklo_epi32(t1, t3), _mm_unpackhi_epi32(t1, t3));
            Is32vec4 sum2 = _mm_add_epi32(Is32vec4(_mm_unpacklo_epi32(e, f)), Is32vec4(_mm_unpackhi_epi32(e, f)));
            sum2 >>= 15;
            *out = Is32vec4(_mm_add_epi32(*out, Is32vec4(_mm_madd_epi16(sum2, gain)) >> 14));
            gain = Is16vec8(_mm_adds_epi16(gain, step));
            ++out;
            ++in;
        }
        break;
    case 5:
        for (unsigned long i = 0; i < count; ++i)
        {
            Is32vec4 s0 = _mm_madd_epi16(in[0], c[20]);
            s0 += _mm_madd_epi16(in[1], c[21]);
            Is32vec4 s1 = _mm_madd_epi16(in[1], c[26]);
            Is32vec4 s2 = _mm_madd_epi16(in[1], c[31]);
            Is32vec4 s3 = _mm_madd_epi16(in[1], c[36]);
            s0 += _mm_madd_epi16(in[2], c[22]);
            s1 += _mm_madd_epi16(in[2], c[27]);
            s2 += _mm_madd_epi16(in[2], c[32]);
            s3 += _mm_madd_epi16(in[2], c[37]);
            s0 += _mm_madd_epi16(in[3], c[23]);
            s1 += _mm_madd_epi16(in[3], c[28]);
            s2 += _mm_madd_epi16(in[3], c[33]);
            s3 += _mm_madd_epi16(in[3], c[38]);
            s1 += _mm_madd_epi16(in[4], c[29]);
            s2 += _mm_madd_epi16(in[4], c[34]);
            s3 += _mm_madd_epi16(in[4], c[39]);
            Is32vec4 a = _mm_add_epi32(_mm_unpacklo_epi32(s0, s2), _mm_unpackhi_epi32(s0, s2));
            Is32vec4 b = _mm_add_epi32(_mm_unpacklo_epi32(s1, s3), _mm_unpackhi_epi32(s1, s3));
            Is32vec4 sum = _mm_add_epi32(Is32vec4(_mm_unpacklo_epi32(a, b)), Is32vec4(_mm_unpackhi_epi32(a, b)));
            sum >>= 15;
            *out = Is32vec4(_mm_add_epi32(*out, Is32vec4(_mm_madd_epi16(sum, gain)) >> 14));
            gain = Is16vec8(_mm_adds_epi16(gain, step));
            ++out;
            Is32vec4 t0 = _mm_madd_epi16(in[1], c[0]);
            Is32vec4 t1 = _mm_madd_epi16(in[1], c[5]);
            Is32vec4 t2 = _mm_madd_epi16(in[1], c[10]);
            Is32vec4 t3 = _mm_madd_epi16(in[1], c[15]);
            t0 += _mm_madd_epi16(in[2], c[1]);
            t1 += _mm_madd_epi16(in[2], c[6]);
            t2 += _mm_madd_epi16(in[2], c[11]);
            t3 += _mm_madd_epi16(in[2], c[16]);
            t0 = Is32vec4(_mm_add_epi32(t0, Is32vec4(_mm_madd_epi16(in[3], c[2]))));
            t1 = Is32vec4(_mm_add_epi32(t1, Is32vec4(_mm_madd_epi16(in[3], c[7]))));
            t2 = Is32vec4(_mm_add_epi32(t2, Is32vec4(_mm_madd_epi16(in[3], c[12]))));
            t3 = Is32vec4(_mm_add_epi32(t3, Is32vec4(_mm_madd_epi16(in[3], c[17]))));
            t0 = Is32vec4(_mm_add_epi32(t0, Is32vec4(_mm_madd_epi16(in[4], c[3]))));
            t1 = Is32vec4(_mm_add_epi32(t1, Is32vec4(_mm_madd_epi16(in[4], c[8]))));
            t2 = Is32vec4(_mm_add_epi32(t2, Is32vec4(_mm_madd_epi16(in[4], c[13]))));
            t3 = Is32vec4(_mm_add_epi32(t3, Is32vec4(_mm_madd_epi16(in[4], c[18]))));
            Is32vec4 e = _mm_add_epi32(_mm_unpacklo_epi32(t0, t2), _mm_unpackhi_epi32(t0, t2));
            Is32vec4 f = _mm_add_epi32(_mm_unpacklo_epi32(t1, t3), _mm_unpackhi_epi32(t1, t3));
            Is32vec4 sum2 = _mm_add_epi32(Is32vec4(_mm_unpacklo_epi32(e, f)), Is32vec4(_mm_unpackhi_epi32(e, f)));
            sum2 >>= 15;
            *out = Is32vec4(_mm_add_epi32(*out, Is32vec4(_mm_madd_epi16(sum2, gain)) >> 14));
            gain = Is16vec8(_mm_adds_epi16(gain, step));
            ++out;
            ++in;
        }
        break;
    case 6:
        for (unsigned long i = 0; i < count; ++i)
        {
            Is32vec4 s0 = _mm_madd_epi16(in[0], c[20]);
            Is32vec4 s1 = _mm_madd_epi16(in[0], c[25]);
            s0 += _mm_madd_epi16(in[1], c[21]);
            s1 += _mm_madd_epi16(in[1], c[26]);
            Is32vec4 s2 = _mm_madd_epi16(in[1], c[31]);
            Is32vec4 s3 = _mm_madd_epi16(in[1], c[36]);
            s0 += _mm_madd_epi16(in[2], c[22]);
            s1 += _mm_madd_epi16(in[2], c[27]);
            s2 += _mm_madd_epi16(in[2], c[32]);
            s3 += _mm_madd_epi16(in[2], c[37]);
            s0 += _mm_madd_epi16(in[3], c[23]);
            s1 += _mm_madd_epi16(in[3], c[28]);
            s2 += _mm_madd_epi16(in[3], c[33]);
            s3 += _mm_madd_epi16(in[3], c[38]);
            s2 += _mm_madd_epi16(in[4], c[34]);
            s3 += _mm_madd_epi16(in[4], c[39]);
            Is32vec4 a = _mm_add_epi32(_mm_unpacklo_epi32(s0, s2), _mm_unpackhi_epi32(s0, s2));
            Is32vec4 b = _mm_add_epi32(_mm_unpacklo_epi32(s1, s3), _mm_unpackhi_epi32(s1, s3));
            Is32vec4 sum = _mm_add_epi32(Is32vec4(_mm_unpacklo_epi32(a, b)), Is32vec4(_mm_unpackhi_epi32(a, b)));
            sum >>= 15;
            *out = Is32vec4(_mm_add_epi32(*out, Is32vec4(_mm_madd_epi16(sum, gain)) >> 14));
            gain = Is16vec8(_mm_adds_epi16(gain, step));
            ++out;
            Is32vec4 t0 = _mm_madd_epi16(in[1], c[0]);
            Is32vec4 t1 = _mm_madd_epi16(in[1], c[5]);
            Is32vec4 t2 = _mm_madd_epi16(in[1], c[10]);
            Is32vec4 t3 = _mm_madd_epi16(in[1], c[15]);
            t0 += _mm_madd_epi16(in[2], c[1]);
            t1 += _mm_madd_epi16(in[2], c[6]);
            t2 += _mm_madd_epi16(in[2], c[11]);
            t3 += _mm_madd_epi16(in[2], c[16]);
            t0 = Is32vec4(_mm_add_epi32(t0, Is32vec4(_mm_madd_epi16(in[3], c[2]))));
            t1 = Is32vec4(_mm_add_epi32(t1, Is32vec4(_mm_madd_epi16(in[3], c[7]))));
            t2 = Is32vec4(_mm_add_epi32(t2, Is32vec4(_mm_madd_epi16(in[3], c[12]))));
            t3 = Is32vec4(_mm_add_epi32(t3, Is32vec4(_mm_madd_epi16(in[3], c[17]))));
            t0 = Is32vec4(_mm_add_epi32(t0, Is32vec4(_mm_madd_epi16(in[4], c[3]))));
            t1 = Is32vec4(_mm_add_epi32(t1, Is32vec4(_mm_madd_epi16(in[4], c[8]))));
            t2 = Is32vec4(_mm_add_epi32(t2, Is32vec4(_mm_madd_epi16(in[4], c[13]))));
            t3 = Is32vec4(_mm_add_epi32(t3, Is32vec4(_mm_madd_epi16(in[4], c[18]))));
            Is32vec4 e = _mm_add_epi32(_mm_unpacklo_epi32(t0, t2), _mm_unpackhi_epi32(t0, t2));
            Is32vec4 f = _mm_add_epi32(_mm_unpacklo_epi32(t1, t3), _mm_unpackhi_epi32(t1, t3));
            Is32vec4 sum2 = _mm_add_epi32(Is32vec4(_mm_unpacklo_epi32(e, f)), Is32vec4(_mm_unpackhi_epi32(e, f)));
            sum2 >>= 15;
            *out = Is32vec4(_mm_add_epi32(*out, Is32vec4(_mm_madd_epi16(sum2, gain)) >> 14));
            gain = Is16vec8(_mm_adds_epi16(gain, step));
            ++out;
            ++in;
        }
        break;
    case 7:
        for (unsigned long i = 0; i < count; ++i)
        {
            Is32vec4 s0 = _mm_madd_epi16(in[0], c[20]);
            Is32vec4 s1 = _mm_madd_epi16(in[0], c[25]);
            Is32vec4 s2 = _mm_madd_epi16(in[0], c[30]);
            s0 += _mm_madd_epi16(in[1], c[21]);
            s1 += _mm_madd_epi16(in[1], c[26]);
            s2 += _mm_madd_epi16(in[1], c[31]);
            Is32vec4 s3 = _mm_madd_epi16(in[1], c[36]);
            s0 += _mm_madd_epi16(in[2], c[22]);
            s1 += _mm_madd_epi16(in[2], c[27]);
            s2 += _mm_madd_epi16(in[2], c[32]);
            s3 += _mm_madd_epi16(in[2], c[37]);
            s0 += _mm_madd_epi16(in[3], c[23]);
            s1 += _mm_madd_epi16(in[3], c[28]);
            s2 += _mm_madd_epi16(in[3], c[33]);
            s3 += _mm_madd_epi16(in[3], c[38]);
            s3 += _mm_madd_epi16(in[4], c[39]);
            Is32vec4 a = _mm_add_epi32(_mm_unpacklo_epi32(s0, s2), _mm_unpackhi_epi32(s0, s2));
            Is32vec4 b = _mm_add_epi32(_mm_unpacklo_epi32(s1, s3), _mm_unpackhi_epi32(s1, s3));
            Is32vec4 sum = _mm_add_epi32(Is32vec4(_mm_unpacklo_epi32(a, b)), Is32vec4(_mm_unpackhi_epi32(a, b)));
            sum >>= 15;
            *out = Is32vec4(_mm_add_epi32(*out, Is32vec4(_mm_madd_epi16(sum, gain)) >> 14));
            gain = Is16vec8(_mm_adds_epi16(gain, step));
            ++out;
            Is32vec4 t0 = _mm_madd_epi16(in[1], c[0]);
            Is32vec4 t1 = _mm_madd_epi16(in[1], c[5]);
            Is32vec4 t2 = _mm_madd_epi16(in[1], c[10]);
            Is32vec4 t3 = _mm_madd_epi16(in[1], c[15]);
            t0 += _mm_madd_epi16(in[2], c[1]);
            t1 += _mm_madd_epi16(in[2], c[6]);
            t2 += _mm_madd_epi16(in[2], c[11]);
            t3 += _mm_madd_epi16(in[2], c[16]);
            t0 = Is32vec4(_mm_add_epi32(t0, Is32vec4(_mm_madd_epi16(in[3], c[2]))));
            t1 = Is32vec4(_mm_add_epi32(t1, Is32vec4(_mm_madd_epi16(in[3], c[7]))));
            t2 = Is32vec4(_mm_add_epi32(t2, Is32vec4(_mm_madd_epi16(in[3], c[12]))));
            t3 = Is32vec4(_mm_add_epi32(t3, Is32vec4(_mm_madd_epi16(in[3], c[17]))));
            t0 = Is32vec4(_mm_add_epi32(t0, Is32vec4(_mm_madd_epi16(in[4], c[3]))));
            t1 = Is32vec4(_mm_add_epi32(t1, Is32vec4(_mm_madd_epi16(in[4], c[8]))));
            t2 = Is32vec4(_mm_add_epi32(t2, Is32vec4(_mm_madd_epi16(in[4], c[13]))));
            t3 = Is32vec4(_mm_add_epi32(t3, Is32vec4(_mm_madd_epi16(in[4], c[18]))));
            Is32vec4 e = _mm_add_epi32(_mm_unpacklo_epi32(t0, t2), _mm_unpackhi_epi32(t0, t2));
            Is32vec4 f = _mm_add_epi32(_mm_unpacklo_epi32(t1, t3), _mm_unpackhi_epi32(t1, t3));
            Is32vec4 sum2 = _mm_add_epi32(Is32vec4(_mm_unpacklo_epi32(e, f)), Is32vec4(_mm_unpackhi_epi32(e, f)));
            sum2 >>= 15;
            *out = Is32vec4(_mm_add_epi32(*out, Is32vec4(_mm_madd_epi16(sum2, gain)) >> 14));
            gain = Is16vec8(_mm_adds_epi16(gain, step));
            ++out;
            ++in;
        }
        break;
    }
}

union UShorts { __m128i align; short v[8 * 16]; };
union UTaps   { __m128i align; short v[8 * 40]; };
union UOut    { __m128i align; int v[8 * 4]; };
static UShorts g_window;
static UTaps g_taps;
static UOut g_out;

static int Dot(int n, int step, int r)
{
    int base = r >= 4 ? 1 : 2;
    bool early = r < n;
    int sum = 0;
    for (int t = 0; t < 4; ++t)
    {
        int k = (early ? base - 1 : base) + t + step;
        int j = early ? 5 * r + t : 5 * r + 1 + t;
        for (int w = 0; w < 8; ++w)
            sum += g_window.v[8 * k + w] * g_taps.v[8 * j + w];
    }
    return sum;
}

static short SaturatingAdd(int a, int b)
{
    int v = a + b;
    if (v > 32767) v = 32767;
    if (v < -32768) v = -32768;
    return (short)v;
}

static bool Check(long startGain, long gainStep)
{
    for (int n = 0; n < 8; ++n)
    {
        const unsigned long count = 4;
        int i;
        for (i = 0; i < 8 * 4; ++i)
            g_out.v[i] = i * 5 - 70;
        ResampleMonoRampFIR(g_window.v + 40, (unsigned long)g_taps.v, (unsigned long)n, count, (long*)g_out.v,
                            startGain, gainStep);
        short lane[4];
        for (int l = 0; l < 4; ++l)
            lane[l] = (short)(startGain + l * gainStep);
        short step4 = (short)(4 * gainStep);
        for (int s = 0; s < (int)count; ++s)
            for (int b = 0; b < 2; ++b)
            {
                for (int l = 0; l < 4; ++l)
                {
                    int r = b == 0 ? 4 + l : l;
                    int slot = 8 * s + 4 * b + l;
                    short low = (short)(Dot(n, s, r) >> 15);
                    int expect = slot * 5 - 70 + ((low * lane[l]) >> 14);
                    if (g_out.v[slot] != expect)
                        return false;
                }
                for (int l = 0; l < 4; ++l)
                    lane[l] = SaturatingAdd(lane[l], step4);
            }
    }
    return true;
}

int main()
{
    int i;
    for (i = 0; i < 8 * 16; ++i)
        g_window.v[i] = (short)((i * 7919) % 8001 - 4000);
    for (i = 0; i < 8 * 40; ++i)
        g_taps.v[i] = (short)((i * 104729) % 32001 - 16000);
    if (!Check(0, 0x100) || !Check(0x7FFF, -0x100) || !Check(0x7E00, 0x180) || !Check(-0x7E00, -0x180))
    {
        std::printf("00c7e420_TEST FAIL\n");
        return 1;
    }
    for (i = 0; i < 8 * 4; ++i)
        g_out.v[i] = 9;
    ResampleMonoRampFIR(g_window.v + 40, (unsigned long)g_taps.v, 9, 4, (long*)g_out.v, 0, 0x100);
    for (i = 0; i < 8 * 4; ++i)
        if (g_out.v[i] != 9) { std::printf("00c7e420_TEST FAIL\n"); return 1; }
    std::printf("00c7e420_TEST PASS\n");
    return 0;
}