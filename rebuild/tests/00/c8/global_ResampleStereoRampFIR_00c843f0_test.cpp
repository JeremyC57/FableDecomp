// Behaviour test for ResampleStereoRampFIR 0x00C843F0: the reconstructed kernel (copied from the source) against a scalar
// statement of the ramped filter for every increment 0..7, three steps, fade-in / fade-out / saturating ramps, two pan gains.
#include <cstdio>
#include <cstddef>
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

void ResampleStereoRampFIR(const short* source, unsigned long phase, unsigned long increment, unsigned long count,
                           long* first, long* second, unsigned short pan, short startGain, short gainStep)
{
    // per-phase gains in the low word of each 32-bit lane (pmaddwd by them scales the low word of each sum)
    Is16vec8 gain(0, (signed short)(startGain + 3 * gainStep), 0, (signed short)(startGain + 2 * gainStep),
                  0, (signed short)(startGain + gainStep), 0, (signed short)startGain);
    Is16vec8 step(0, (signed short)(4 * gainStep), 0, (signed short)(4 * gainStep),
                  0, (signed short)(4 * gainStep), 0, (signed short)(4 * gainStep));
    // {pan, 0} in every 32-bit lane: pmaddwd by it scales the low word of each (halved) sum by the pan gain
    Is16vec8 panGain(0, (signed short)pan, 0, (signed short)pan, 0, (signed short)pan, 0, (signed short)pan);
    const Is16vec8* in = (const Is16vec8*)(source - 40);      // window: five 8-sample quads behind the read point
    const Is16vec8* c = (const Is16vec8*)phase;               // 40 coefficient quads, 5 per output phase
    Is32vec4* out1 = (Is32vec4*)first;                        // 16-byte aligned mix buffers, four longs per quad
    Is32vec4* out2 = (Is32vec4*)second;
    switch (increment)
    {
    case 0:
        for (unsigned long i = 0; i < count; ++i)
        {
            Is32vec4 s0 = _mm_madd_epi16(in[1], c[21]);
            Is32vec4 s1 = _mm_madd_epi16(in[1], c[26]);
            Is32vec4 s2 = _mm_madd_epi16(in[1], c[31]);
            Is32vec4 s3 = _mm_madd_epi16(in[1], c[36]);
            s0 += Is32vec4(_mm_madd_epi16(in[2], c[22]));
            s1 += Is32vec4(_mm_madd_epi16(in[2], c[27]));
            s2 += Is32vec4(_mm_madd_epi16(in[2], c[32]));
            s3 += Is32vec4(_mm_madd_epi16(in[2], c[37]));
            s0 += _mm_madd_epi16(in[3], c[23]);
            s1 += _mm_madd_epi16(in[3], c[28]);
            s2 += _mm_madd_epi16(in[3], c[33]);
            s3 += _mm_madd_epi16(in[3], c[38]);
            s0 += _mm_madd_epi16(in[4], c[24]);
            s1 += _mm_madd_epi16(in[4], c[29]);
            s2 += _mm_madd_epi16(in[4], c[34]);
            s3 += _mm_madd_epi16(in[4], c[39]);
            Is32vec4 a = _mm_add_epi32(Is32vec4(_mm_unpacklo_epi32(s0, s2)), Is32vec4(_mm_unpackhi_epi32(s0, s2)));
            Is32vec4 b = _mm_add_epi32(_mm_unpacklo_epi32(s1, s3), _mm_unpackhi_epi32(s1, s3));
            Is32vec4 sum = _mm_add_epi32(Is32vec4(_mm_unpacklo_epi32(a, b)), Is32vec4(_mm_unpackhi_epi32(a, b)));
            sum >>= 15;
            sum = Is32vec4(_mm_madd_epi16(Is16vec8(sum), gain)) >> 14;
            *out1 = Is32vec4(_mm_add_epi32(*out1, sum));
            ++out1;
            gain = Is16vec8(_mm_adds_epi16(gain, step));
            sum >>= 1;
            *out2 = Is32vec4(_mm_add_epi32(*out2, Is32vec4(_mm_madd_epi16(Is16vec8(sum), panGain)) >> 14));
            ++out2;
            Is32vec4 t0 = _mm_madd_epi16(in[2], c[1]);
            Is32vec4 t1 = _mm_madd_epi16(in[2], c[6]);
            Is32vec4 t2 = _mm_madd_epi16(in[2], c[11]);
            Is32vec4 t3 = _mm_madd_epi16(in[2], c[16]);
            t0 = Is32vec4(_mm_add_epi32(t0, _mm_madd_epi16(in[3], c[2])));
            t1 = Is32vec4(_mm_add_epi32(t1, _mm_madd_epi16(in[3], c[7])));
            t2 = Is32vec4(_mm_add_epi32(t2, _mm_madd_epi16(in[3], c[12])));
            t3 = Is32vec4(_mm_add_epi32(t3, _mm_madd_epi16(in[3], c[17])));
            t0 = Is32vec4(_mm_add_epi32(t0, _mm_madd_epi16(in[4], c[3])));
            t1 = Is32vec4(_mm_add_epi32(t1, _mm_madd_epi16(in[4], c[8])));
            t2 = Is32vec4(_mm_add_epi32(t2, _mm_madd_epi16(in[4], c[13])));
            t3 = Is32vec4(_mm_add_epi32(t3, _mm_madd_epi16(in[4], c[18])));
            t0 += _mm_madd_epi16(in[5], c[4]);
            t1 += _mm_madd_epi16(in[5], c[9]);
            t2 += _mm_madd_epi16(in[5], c[14]);
            t3 += _mm_madd_epi16(in[5], c[19]);
            Is32vec4 e = _mm_add_epi32(Is32vec4(_mm_unpacklo_epi32(t0, t2)), Is32vec4(_mm_unpackhi_epi32(t0, t2)));
            Is32vec4 f = _mm_add_epi32(Is32vec4(_mm_unpacklo_epi32(t1, t3)), Is32vec4(_mm_unpackhi_epi32(t1, t3)));
            Is32vec4 sum2 = _mm_add_epi32(Is32vec4(_mm_unpacklo_epi32(e, f)), Is32vec4(_mm_unpackhi_epi32(e, f)));
            sum2 >>= 15;
            sum2 = Is32vec4(_mm_madd_epi16(Is16vec8(sum2), gain)) >> 14;
            *out1 = Is32vec4(_mm_add_epi32(*out1, sum2));
            ++out1;
            gain = Is16vec8(_mm_adds_epi16(gain, step));
            sum2 >>= 1;
            *out2 = Is32vec4(_mm_add_epi32(*out2, Is32vec4(_mm_madd_epi16(Is16vec8(sum2), panGain)) >> 14));
            ++out2;
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
            s0 += Is32vec4(_mm_madd_epi16(in[2], c[22]));
            s1 += Is32vec4(_mm_madd_epi16(in[2], c[27]));
            s2 += Is32vec4(_mm_madd_epi16(in[2], c[32]));
            s3 += Is32vec4(_mm_madd_epi16(in[2], c[37]));
            s0 += Is32vec4(_mm_madd_epi16(in[3], c[23]));
            s1 += Is32vec4(_mm_madd_epi16(in[3], c[28]));
            s2 += Is32vec4(_mm_madd_epi16(in[3], c[33]));
            s3 += Is32vec4(_mm_madd_epi16(in[3], c[38]));
            s0 += _mm_madd_epi16(in[4], c[24]);
            s1 += _mm_madd_epi16(in[4], c[29]);
            s2 += _mm_madd_epi16(in[4], c[34]);
            s3 += _mm_madd_epi16(in[4], c[39]);
            Is32vec4 a = _mm_add_epi32(Is32vec4(_mm_unpacklo_epi32(s0, s2)), Is32vec4(_mm_unpackhi_epi32(s0, s2)));
            Is32vec4 b = _mm_add_epi32(_mm_unpacklo_epi32(s1, s3), _mm_unpackhi_epi32(s1, s3));
            Is32vec4 sum = _mm_add_epi32(Is32vec4(_mm_unpacklo_epi32(a, b)), Is32vec4(_mm_unpackhi_epi32(a, b)));
            sum >>= 15;
            sum = Is32vec4(_mm_madd_epi16(Is16vec8(sum), gain)) >> 14;
            *out1 = Is32vec4(_mm_add_epi32(*out1, sum));
            ++out1;
            gain = Is16vec8(_mm_adds_epi16(gain, step));
            sum >>= 1;
            *out2 = Is32vec4(_mm_add_epi32(*out2, Is32vec4(_mm_madd_epi16(Is16vec8(sum), panGain)) >> 14));
            ++out2;
            Is32vec4 t0 = _mm_madd_epi16(in[1], c[0]);
            t0 += _mm_madd_epi16(in[2], c[1]);
            Is32vec4 t1 = _mm_madd_epi16(in[2], c[6]);
            Is32vec4 t2 = _mm_madd_epi16(in[2], c[11]);
            Is32vec4 t3 = _mm_madd_epi16(in[2], c[16]);
            t0 = Is32vec4(_mm_add_epi32(t0, _mm_madd_epi16(in[3], c[2])));
            t1 = Is32vec4(_mm_add_epi32(t1, _mm_madd_epi16(in[3], c[7])));
            t2 = Is32vec4(_mm_add_epi32(t2, _mm_madd_epi16(in[3], c[12])));
            t3 = Is32vec4(_mm_add_epi32(t3, _mm_madd_epi16(in[3], c[17])));
            t0 = Is32vec4(_mm_add_epi32(t0, _mm_madd_epi16(in[4], c[3])));
            t1 = Is32vec4(_mm_add_epi32(t1, _mm_madd_epi16(in[4], c[8])));
            t2 = Is32vec4(_mm_add_epi32(t2, _mm_madd_epi16(in[4], c[13])));
            t3 = Is32vec4(_mm_add_epi32(t3, _mm_madd_epi16(in[4], c[18])));
            t1 += _mm_madd_epi16(in[5], c[9]);
            t2 += _mm_madd_epi16(in[5], c[14]);
            t3 += _mm_madd_epi16(in[5], c[19]);
            Is32vec4 e = _mm_add_epi32(Is32vec4(_mm_unpacklo_epi32(t0, t2)), Is32vec4(_mm_unpackhi_epi32(t0, t2)));
            Is32vec4 f = _mm_add_epi32(Is32vec4(_mm_unpacklo_epi32(t1, t3)), Is32vec4(_mm_unpackhi_epi32(t1, t3)));
            Is32vec4 sum2 = _mm_add_epi32(Is32vec4(_mm_unpacklo_epi32(e, f)), Is32vec4(_mm_unpackhi_epi32(e, f)));
            sum2 >>= 15;
            sum2 = Is32vec4(_mm_madd_epi16(Is16vec8(sum2), gain)) >> 14;
            *out1 = Is32vec4(_mm_add_epi32(*out1, sum2));
            ++out1;
            gain = Is16vec8(_mm_adds_epi16(gain, step));
            sum2 >>= 1;
            *out2 = Is32vec4(_mm_add_epi32(*out2, Is32vec4(_mm_madd_epi16(Is16vec8(sum2), panGain)) >> 14));
            ++out2;
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
            s0 += Is32vec4(_mm_madd_epi16(in[2], c[22]));
            s1 += Is32vec4(_mm_madd_epi16(in[2], c[27]));
            s2 += Is32vec4(_mm_madd_epi16(in[2], c[32]));
            s3 += Is32vec4(_mm_madd_epi16(in[2], c[37]));
            s0 += Is32vec4(_mm_madd_epi16(in[3], c[23]));
            s1 += Is32vec4(_mm_madd_epi16(in[3], c[28]));
            s2 += Is32vec4(_mm_madd_epi16(in[3], c[33]));
            s3 += Is32vec4(_mm_madd_epi16(in[3], c[38]));
            s0 += _mm_madd_epi16(in[4], c[24]);
            s1 += _mm_madd_epi16(in[4], c[29]);
            s2 += _mm_madd_epi16(in[4], c[34]);
            s3 += _mm_madd_epi16(in[4], c[39]);
            Is32vec4 a = _mm_add_epi32(Is32vec4(_mm_unpacklo_epi32(s0, s2)), Is32vec4(_mm_unpackhi_epi32(s0, s2)));
            Is32vec4 b = _mm_add_epi32(_mm_unpacklo_epi32(s1, s3), _mm_unpackhi_epi32(s1, s3));
            Is32vec4 sum = _mm_add_epi32(Is32vec4(_mm_unpacklo_epi32(a, b)), Is32vec4(_mm_unpackhi_epi32(a, b)));
            sum >>= 15;
            sum = Is32vec4(_mm_madd_epi16(Is16vec8(sum), gain)) >> 14;
            *out1 = Is32vec4(_mm_add_epi32(*out1, sum));
            ++out1;
            gain = Is16vec8(_mm_adds_epi16(gain, step));
            sum >>= 1;
            *out2 = Is32vec4(_mm_add_epi32(*out2, Is32vec4(_mm_madd_epi16(Is16vec8(sum), panGain)) >> 14));
            ++out2;
            Is32vec4 t0 = _mm_madd_epi16(in[1], c[0]);
            Is32vec4 t1 = _mm_madd_epi16(in[1], c[5]);
            t0 += _mm_madd_epi16(in[2], c[1]);
            t1 += _mm_madd_epi16(in[2], c[6]);
            Is32vec4 t2 = _mm_madd_epi16(in[2], c[11]);
            Is32vec4 t3 = _mm_madd_epi16(in[2], c[16]);
            t0 = Is32vec4(_mm_add_epi32(t0, _mm_madd_epi16(in[3], c[2])));
            t1 = Is32vec4(_mm_add_epi32(t1, _mm_madd_epi16(in[3], c[7])));
            t2 = Is32vec4(_mm_add_epi32(t2, _mm_madd_epi16(in[3], c[12])));
            t3 = Is32vec4(_mm_add_epi32(t3, _mm_madd_epi16(in[3], c[17])));
            t0 = Is32vec4(_mm_add_epi32(t0, _mm_madd_epi16(in[4], c[3])));
            t1 = Is32vec4(_mm_add_epi32(t1, _mm_madd_epi16(in[4], c[8])));
            t2 = Is32vec4(_mm_add_epi32(t2, _mm_madd_epi16(in[4], c[13])));
            t3 = Is32vec4(_mm_add_epi32(t3, _mm_madd_epi16(in[4], c[18])));
            t2 += _mm_madd_epi16(in[5], c[14]);
            t3 += _mm_madd_epi16(in[5], c[19]);
            Is32vec4 e = _mm_add_epi32(Is32vec4(_mm_unpacklo_epi32(t0, t2)), Is32vec4(_mm_unpackhi_epi32(t0, t2)));
            Is32vec4 f = _mm_add_epi32(Is32vec4(_mm_unpacklo_epi32(t1, t3)), Is32vec4(_mm_unpackhi_epi32(t1, t3)));
            Is32vec4 sum2 = _mm_add_epi32(Is32vec4(_mm_unpacklo_epi32(e, f)), Is32vec4(_mm_unpackhi_epi32(e, f)));
            sum2 >>= 15;
            sum2 = Is32vec4(_mm_madd_epi16(Is16vec8(sum2), gain)) >> 14;
            *out1 = Is32vec4(_mm_add_epi32(*out1, sum2));
            ++out1;
            gain = Is16vec8(_mm_adds_epi16(gain, step));
            sum2 >>= 1;
            *out2 = Is32vec4(_mm_add_epi32(*out2, Is32vec4(_mm_madd_epi16(Is16vec8(sum2), panGain)) >> 14));
            ++out2;
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
            s0 += Is32vec4(_mm_madd_epi16(in[2], c[22]));
            s1 += Is32vec4(_mm_madd_epi16(in[2], c[27]));
            s2 += Is32vec4(_mm_madd_epi16(in[2], c[32]));
            s3 += Is32vec4(_mm_madd_epi16(in[2], c[37]));
            s0 += Is32vec4(_mm_madd_epi16(in[3], c[23]));
            s1 += Is32vec4(_mm_madd_epi16(in[3], c[28]));
            s2 += Is32vec4(_mm_madd_epi16(in[3], c[33]));
            s3 += Is32vec4(_mm_madd_epi16(in[3], c[38]));
            s0 += _mm_madd_epi16(in[4], c[24]);
            s1 += _mm_madd_epi16(in[4], c[29]);
            s2 += _mm_madd_epi16(in[4], c[34]);
            s3 += _mm_madd_epi16(in[4], c[39]);
            Is32vec4 a = _mm_add_epi32(Is32vec4(_mm_unpacklo_epi32(s0, s2)), Is32vec4(_mm_unpackhi_epi32(s0, s2)));
            Is32vec4 b = _mm_add_epi32(_mm_unpacklo_epi32(s1, s3), _mm_unpackhi_epi32(s1, s3));
            Is32vec4 sum = _mm_add_epi32(Is32vec4(_mm_unpacklo_epi32(a, b)), Is32vec4(_mm_unpackhi_epi32(a, b)));
            sum >>= 15;
            sum = Is32vec4(_mm_madd_epi16(Is16vec8(sum), gain)) >> 14;
            *out1 = Is32vec4(_mm_add_epi32(*out1, sum));
            ++out1;
            gain = Is16vec8(_mm_adds_epi16(gain, step));
            sum >>= 1;
            *out2 = Is32vec4(_mm_add_epi32(*out2, Is32vec4(_mm_madd_epi16(Is16vec8(sum), panGain)) >> 14));
            ++out2;
            Is32vec4 t0 = _mm_madd_epi16(in[1], c[0]);
            Is32vec4 t1 = _mm_madd_epi16(in[1], c[5]);
            Is32vec4 t2 = _mm_madd_epi16(in[1], c[10]);
            t0 += _mm_madd_epi16(in[2], c[1]);
            t1 += _mm_madd_epi16(in[2], c[6]);
            t2 += _mm_madd_epi16(in[2], c[11]);
            Is32vec4 t3 = _mm_madd_epi16(in[2], c[16]);
            t0 = Is32vec4(_mm_add_epi32(t0, _mm_madd_epi16(in[3], c[2])));
            t1 = Is32vec4(_mm_add_epi32(t1, _mm_madd_epi16(in[3], c[7])));
            t2 = Is32vec4(_mm_add_epi32(t2, _mm_madd_epi16(in[3], c[12])));
            t3 = Is32vec4(_mm_add_epi32(t3, _mm_madd_epi16(in[3], c[17])));
            t0 = Is32vec4(_mm_add_epi32(t0, _mm_madd_epi16(in[4], c[3])));
            t1 = Is32vec4(_mm_add_epi32(t1, _mm_madd_epi16(in[4], c[8])));
            t2 = Is32vec4(_mm_add_epi32(t2, _mm_madd_epi16(in[4], c[13])));
            t3 = Is32vec4(_mm_add_epi32(t3, _mm_madd_epi16(in[4], c[18])));
            t3 += _mm_madd_epi16(in[5], c[19]);
            Is32vec4 e = _mm_add_epi32(Is32vec4(_mm_unpacklo_epi32(t0, t2)), Is32vec4(_mm_unpackhi_epi32(t0, t2)));
            Is32vec4 f = _mm_add_epi32(Is32vec4(_mm_unpacklo_epi32(t1, t3)), Is32vec4(_mm_unpackhi_epi32(t1, t3)));
            Is32vec4 sum2 = _mm_add_epi32(Is32vec4(_mm_unpacklo_epi32(e, f)), Is32vec4(_mm_unpackhi_epi32(e, f)));
            sum2 >>= 15;
            sum2 = Is32vec4(_mm_madd_epi16(Is16vec8(sum2), gain)) >> 14;
            *out1 = Is32vec4(_mm_add_epi32(*out1, sum2));
            ++out1;
            gain = Is16vec8(_mm_adds_epi16(gain, step));
            sum2 >>= 1;
            *out2 = Is32vec4(_mm_add_epi32(*out2, Is32vec4(_mm_madd_epi16(Is16vec8(sum2), panGain)) >> 14));
            ++out2;
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
            s0 += Is32vec4(_mm_madd_epi16(in[2], c[22]));
            s1 += Is32vec4(_mm_madd_epi16(in[2], c[27]));
            s2 += Is32vec4(_mm_madd_epi16(in[2], c[32]));
            s3 += Is32vec4(_mm_madd_epi16(in[2], c[37]));
            s0 += Is32vec4(_mm_madd_epi16(in[3], c[23]));
            s1 += Is32vec4(_mm_madd_epi16(in[3], c[28]));
            s2 += Is32vec4(_mm_madd_epi16(in[3], c[33]));
            s3 += Is32vec4(_mm_madd_epi16(in[3], c[38]));
            s0 += _mm_madd_epi16(in[4], c[24]);
            s1 += _mm_madd_epi16(in[4], c[29]);
            s2 += _mm_madd_epi16(in[4], c[34]);
            s3 += _mm_madd_epi16(in[4], c[39]);
            Is32vec4 a = _mm_add_epi32(Is32vec4(_mm_unpacklo_epi32(s0, s2)), Is32vec4(_mm_unpackhi_epi32(s0, s2)));
            Is32vec4 b = _mm_add_epi32(_mm_unpacklo_epi32(s1, s3), _mm_unpackhi_epi32(s1, s3));
            Is32vec4 sum = _mm_add_epi32(Is32vec4(_mm_unpacklo_epi32(a, b)), Is32vec4(_mm_unpackhi_epi32(a, b)));
            sum >>= 15;
            sum = Is32vec4(_mm_madd_epi16(Is16vec8(sum), gain)) >> 14;
            *out1 = Is32vec4(_mm_add_epi32(*out1, sum));
            ++out1;
            gain = Is16vec8(_mm_adds_epi16(gain, step));
            sum >>= 1;
            *out2 = Is32vec4(_mm_add_epi32(*out2, Is32vec4(_mm_madd_epi16(Is16vec8(sum), panGain)) >> 14));
            ++out2;
            Is32vec4 t0 = _mm_madd_epi16(in[1], c[0]);
            Is32vec4 t1 = _mm_madd_epi16(in[1], c[5]);
            Is32vec4 t2 = _mm_madd_epi16(in[1], c[10]);
            Is32vec4 t3 = _mm_madd_epi16(in[1], c[15]);
            t0 += _mm_madd_epi16(in[2], c[1]);
            t1 += _mm_madd_epi16(in[2], c[6]);
            t2 += _mm_madd_epi16(in[2], c[11]);
            t3 += _mm_madd_epi16(in[2], c[16]);
            t0 = Is32vec4(_mm_add_epi32(t0, _mm_madd_epi16(in[3], c[2])));
            t1 = Is32vec4(_mm_add_epi32(t1, _mm_madd_epi16(in[3], c[7])));
            t2 = Is32vec4(_mm_add_epi32(t2, _mm_madd_epi16(in[3], c[12])));
            t3 = Is32vec4(_mm_add_epi32(t3, _mm_madd_epi16(in[3], c[17])));
            t0 = Is32vec4(_mm_add_epi32(t0, _mm_madd_epi16(in[4], c[3])));
            t1 = Is32vec4(_mm_add_epi32(t1, _mm_madd_epi16(in[4], c[8])));
            t2 = Is32vec4(_mm_add_epi32(t2, _mm_madd_epi16(in[4], c[13])));
            t3 = Is32vec4(_mm_add_epi32(t3, _mm_madd_epi16(in[4], c[18])));
            Is32vec4 e = _mm_add_epi32(Is32vec4(_mm_unpacklo_epi32(t0, t2)), Is32vec4(_mm_unpackhi_epi32(t0, t2)));
            Is32vec4 f = _mm_add_epi32(Is32vec4(_mm_unpacklo_epi32(t1, t3)), Is32vec4(_mm_unpackhi_epi32(t1, t3)));
            Is32vec4 sum2 = _mm_add_epi32(Is32vec4(_mm_unpacklo_epi32(e, f)), Is32vec4(_mm_unpackhi_epi32(e, f)));
            sum2 >>= 15;
            sum2 = Is32vec4(_mm_madd_epi16(Is16vec8(sum2), gain)) >> 14;
            *out1 = Is32vec4(_mm_add_epi32(*out1, sum2));
            ++out1;
            gain = Is16vec8(_mm_adds_epi16(gain, step));
            sum2 >>= 1;
            *out2 = Is32vec4(_mm_add_epi32(*out2, Is32vec4(_mm_madd_epi16(Is16vec8(sum2), panGain)) >> 14));
            ++out2;
            ++in;
        }
        break;
    case 5:
        for (unsigned long i = 0; i < count; ++i)
        {
            Is32vec4 s0 = _mm_madd_epi16(in[0], c[20]);
            s0 += Is32vec4(_mm_madd_epi16(in[1], c[21]));
            Is32vec4 s1 = _mm_madd_epi16(in[1], c[26]);
            Is32vec4 s2 = _mm_madd_epi16(in[1], c[31]);
            Is32vec4 s3 = _mm_madd_epi16(in[1], c[36]);
            s0 += Is32vec4(_mm_madd_epi16(in[2], c[22]));
            s1 += Is32vec4(_mm_madd_epi16(in[2], c[27]));
            s2 += Is32vec4(_mm_madd_epi16(in[2], c[32]));
            s3 += Is32vec4(_mm_madd_epi16(in[2], c[37]));
            s0 += _mm_madd_epi16(in[3], c[23]);
            s1 += _mm_madd_epi16(in[3], c[28]);
            s2 += _mm_madd_epi16(in[3], c[33]);
            s3 += _mm_madd_epi16(in[3], c[38]);
            s1 += _mm_madd_epi16(in[4], c[29]);
            s2 += _mm_madd_epi16(in[4], c[34]);
            s3 += _mm_madd_epi16(in[4], c[39]);
            Is32vec4 a = _mm_add_epi32(Is32vec4(_mm_unpacklo_epi32(s0, s2)), Is32vec4(_mm_unpackhi_epi32(s0, s2)));
            Is32vec4 b = _mm_add_epi32(_mm_unpacklo_epi32(s1, s3), _mm_unpackhi_epi32(s1, s3));
            Is32vec4 sum = _mm_add_epi32(Is32vec4(_mm_unpacklo_epi32(a, b)), Is32vec4(_mm_unpackhi_epi32(a, b)));
            sum >>= 15;
            sum = Is32vec4(_mm_madd_epi16(Is16vec8(sum), gain)) >> 14;
            *out1 = Is32vec4(_mm_add_epi32(*out1, sum));
            ++out1;
            gain = Is16vec8(_mm_adds_epi16(gain, step));
            sum >>= 1;
            *out2 = Is32vec4(_mm_add_epi32(*out2, Is32vec4(_mm_madd_epi16(Is16vec8(sum), panGain)) >> 14));
            ++out2;
            Is32vec4 t0 = _mm_madd_epi16(in[1], c[0]);
            Is32vec4 t1 = _mm_madd_epi16(in[1], c[5]);
            Is32vec4 t2 = _mm_madd_epi16(in[1], c[10]);
            Is32vec4 t3 = _mm_madd_epi16(in[1], c[15]);
            t0 += _mm_madd_epi16(in[2], c[1]);
            t1 += _mm_madd_epi16(in[2], c[6]);
            t2 += _mm_madd_epi16(in[2], c[11]);
            t3 += _mm_madd_epi16(in[2], c[16]);
            t0 = Is32vec4(_mm_add_epi32(t0, _mm_madd_epi16(in[3], c[2])));
            t1 = Is32vec4(_mm_add_epi32(t1, _mm_madd_epi16(in[3], c[7])));
            t2 = Is32vec4(_mm_add_epi32(t2, _mm_madd_epi16(in[3], c[12])));
            t3 = Is32vec4(_mm_add_epi32(t3, _mm_madd_epi16(in[3], c[17])));
            t0 = Is32vec4(_mm_add_epi32(t0, _mm_madd_epi16(in[4], c[3])));
            t1 = Is32vec4(_mm_add_epi32(t1, _mm_madd_epi16(in[4], c[8])));
            t2 = Is32vec4(_mm_add_epi32(t2, _mm_madd_epi16(in[4], c[13])));
            t3 = Is32vec4(_mm_add_epi32(t3, _mm_madd_epi16(in[4], c[18])));
            Is32vec4 e = _mm_add_epi32(Is32vec4(_mm_unpacklo_epi32(t0, t2)), Is32vec4(_mm_unpackhi_epi32(t0, t2)));
            Is32vec4 f = _mm_add_epi32(Is32vec4(_mm_unpacklo_epi32(t1, t3)), Is32vec4(_mm_unpackhi_epi32(t1, t3)));
            Is32vec4 sum2 = _mm_add_epi32(Is32vec4(_mm_unpacklo_epi32(e, f)), Is32vec4(_mm_unpackhi_epi32(e, f)));
            sum2 >>= 15;
            sum2 = Is32vec4(_mm_madd_epi16(Is16vec8(sum2), gain)) >> 14;
            *out1 = Is32vec4(_mm_add_epi32(*out1, sum2));
            ++out1;
            gain = Is16vec8(_mm_adds_epi16(gain, step));
            sum2 >>= 1;
            *out2 = Is32vec4(_mm_add_epi32(*out2, Is32vec4(_mm_madd_epi16(Is16vec8(sum2), panGain)) >> 14));
            ++out2;
            ++in;
        }
        break;
    case 6:
        for (unsigned long i = 0; i < count; ++i)
        {
            Is32vec4 s0 = _mm_madd_epi16(in[0], c[20]);
            Is32vec4 s1 = _mm_madd_epi16(in[0], c[25]);
            s0 += Is32vec4(_mm_madd_epi16(in[1], c[21]));
            s1 += Is32vec4(_mm_madd_epi16(in[1], c[26]));
            Is32vec4 s2 = _mm_madd_epi16(in[1], c[31]);
            Is32vec4 s3 = _mm_madd_epi16(in[1], c[36]);
            s0 += Is32vec4(_mm_madd_epi16(in[2], c[22]));
            s1 += Is32vec4(_mm_madd_epi16(in[2], c[27]));
            s2 += Is32vec4(_mm_madd_epi16(in[2], c[32]));
            s3 += Is32vec4(_mm_madd_epi16(in[2], c[37]));
            s0 += _mm_madd_epi16(in[3], c[23]);
            s1 += _mm_madd_epi16(in[3], c[28]);
            s2 += _mm_madd_epi16(in[3], c[33]);
            s3 += _mm_madd_epi16(in[3], c[38]);
            s2 += _mm_madd_epi16(in[4], c[34]);
            s3 += _mm_madd_epi16(in[4], c[39]);
            Is32vec4 a = _mm_add_epi32(Is32vec4(_mm_unpacklo_epi32(s0, s2)), Is32vec4(_mm_unpackhi_epi32(s0, s2)));
            Is32vec4 b = _mm_add_epi32(_mm_unpacklo_epi32(s1, s3), _mm_unpackhi_epi32(s1, s3));
            Is32vec4 sum = _mm_add_epi32(Is32vec4(_mm_unpacklo_epi32(a, b)), Is32vec4(_mm_unpackhi_epi32(a, b)));
            sum >>= 15;
            sum = Is32vec4(_mm_madd_epi16(Is16vec8(sum), gain)) >> 14;
            *out1 = Is32vec4(_mm_add_epi32(*out1, sum));
            ++out1;
            gain = Is16vec8(_mm_adds_epi16(gain, step));
            sum >>= 1;
            *out2 = Is32vec4(_mm_add_epi32(*out2, Is32vec4(_mm_madd_epi16(Is16vec8(sum), panGain)) >> 14));
            ++out2;
            Is32vec4 t0 = _mm_madd_epi16(in[1], c[0]);
            Is32vec4 t1 = _mm_madd_epi16(in[1], c[5]);
            Is32vec4 t2 = _mm_madd_epi16(in[1], c[10]);
            Is32vec4 t3 = _mm_madd_epi16(in[1], c[15]);
            t0 += _mm_madd_epi16(in[2], c[1]);
            t1 += _mm_madd_epi16(in[2], c[6]);
            t2 += _mm_madd_epi16(in[2], c[11]);
            t3 += _mm_madd_epi16(in[2], c[16]);
            t0 = Is32vec4(_mm_add_epi32(t0, _mm_madd_epi16(in[3], c[2])));
            t1 = Is32vec4(_mm_add_epi32(t1, _mm_madd_epi16(in[3], c[7])));
            t2 = Is32vec4(_mm_add_epi32(t2, _mm_madd_epi16(in[3], c[12])));
            t3 = Is32vec4(_mm_add_epi32(t3, _mm_madd_epi16(in[3], c[17])));
            t0 = Is32vec4(_mm_add_epi32(t0, _mm_madd_epi16(in[4], c[3])));
            t1 = Is32vec4(_mm_add_epi32(t1, _mm_madd_epi16(in[4], c[8])));
            t2 = Is32vec4(_mm_add_epi32(t2, _mm_madd_epi16(in[4], c[13])));
            t3 = Is32vec4(_mm_add_epi32(t3, _mm_madd_epi16(in[4], c[18])));
            Is32vec4 e = _mm_add_epi32(Is32vec4(_mm_unpacklo_epi32(t0, t2)), Is32vec4(_mm_unpackhi_epi32(t0, t2)));
            Is32vec4 f = _mm_add_epi32(Is32vec4(_mm_unpacklo_epi32(t1, t3)), Is32vec4(_mm_unpackhi_epi32(t1, t3)));
            Is32vec4 sum2 = _mm_add_epi32(Is32vec4(_mm_unpacklo_epi32(e, f)), Is32vec4(_mm_unpackhi_epi32(e, f)));
            sum2 >>= 15;
            sum2 = Is32vec4(_mm_madd_epi16(Is16vec8(sum2), gain)) >> 14;
            *out1 = Is32vec4(_mm_add_epi32(*out1, sum2));
            ++out1;
            gain = Is16vec8(_mm_adds_epi16(gain, step));
            sum2 >>= 1;
            *out2 = Is32vec4(_mm_add_epi32(*out2, Is32vec4(_mm_madd_epi16(Is16vec8(sum2), panGain)) >> 14));
            ++out2;
            ++in;
        }
        break;
    case 7:
        for (unsigned long i = 0; i < count; ++i)
        {
            Is32vec4 s0 = _mm_madd_epi16(in[0], c[20]);
            Is32vec4 s1 = _mm_madd_epi16(in[0], c[25]);
            Is32vec4 s2 = _mm_madd_epi16(in[0], c[30]);
            s0 += Is32vec4(_mm_madd_epi16(in[1], c[21]));
            s1 += Is32vec4(_mm_madd_epi16(in[1], c[26]));
            s2 += Is32vec4(_mm_madd_epi16(in[1], c[31]));
            Is32vec4 s3 = _mm_madd_epi16(in[1], c[36]);
            s0 += Is32vec4(_mm_madd_epi16(in[2], c[22]));
            s1 += Is32vec4(_mm_madd_epi16(in[2], c[27]));
            s2 += Is32vec4(_mm_madd_epi16(in[2], c[32]));
            s3 += Is32vec4(_mm_madd_epi16(in[2], c[37]));
            s0 += _mm_madd_epi16(in[3], c[23]);
            s1 += _mm_madd_epi16(in[3], c[28]);
            s2 += _mm_madd_epi16(in[3], c[33]);
            s3 += _mm_madd_epi16(in[3], c[38]);
            s3 += _mm_madd_epi16(in[4], c[39]);
            Is32vec4 a = _mm_add_epi32(Is32vec4(_mm_unpacklo_epi32(s0, s2)), Is32vec4(_mm_unpackhi_epi32(s0, s2)));
            Is32vec4 b = _mm_add_epi32(_mm_unpacklo_epi32(s1, s3), _mm_unpackhi_epi32(s1, s3));
            Is32vec4 sum = _mm_add_epi32(Is32vec4(_mm_unpacklo_epi32(a, b)), Is32vec4(_mm_unpackhi_epi32(a, b)));
            sum >>= 15;
            sum = Is32vec4(_mm_madd_epi16(Is16vec8(sum), gain)) >> 14;
            *out1 = Is32vec4(_mm_add_epi32(*out1, sum));
            ++out1;
            gain = Is16vec8(_mm_adds_epi16(gain, step));
            sum >>= 1;
            *out2 = Is32vec4(_mm_add_epi32(*out2, Is32vec4(_mm_madd_epi16(Is16vec8(sum), panGain)) >> 14));
            ++out2;
            Is32vec4 t0 = _mm_madd_epi16(in[1], c[0]);
            Is32vec4 t1 = _mm_madd_epi16(in[1], c[5]);
            Is32vec4 t2 = _mm_madd_epi16(in[1], c[10]);
            Is32vec4 t3 = _mm_madd_epi16(in[1], c[15]);
            t0 += _mm_madd_epi16(in[2], c[1]);
            t1 += _mm_madd_epi16(in[2], c[6]);
            t2 += _mm_madd_epi16(in[2], c[11]);
            t3 += _mm_madd_epi16(in[2], c[16]);
            t0 = Is32vec4(_mm_add_epi32(t0, _mm_madd_epi16(in[3], c[2])));
            t1 = Is32vec4(_mm_add_epi32(t1, _mm_madd_epi16(in[3], c[7])));
            t2 = Is32vec4(_mm_add_epi32(t2, _mm_madd_epi16(in[3], c[12])));
            t3 = Is32vec4(_mm_add_epi32(t3, _mm_madd_epi16(in[3], c[17])));
            t0 = Is32vec4(_mm_add_epi32(t0, _mm_madd_epi16(in[4], c[3])));
            t1 = Is32vec4(_mm_add_epi32(t1, _mm_madd_epi16(in[4], c[8])));
            t2 = Is32vec4(_mm_add_epi32(t2, _mm_madd_epi16(in[4], c[13])));
            t3 = Is32vec4(_mm_add_epi32(t3, _mm_madd_epi16(in[4], c[18])));
            Is32vec4 e = _mm_add_epi32(Is32vec4(_mm_unpacklo_epi32(t0, t2)), Is32vec4(_mm_unpackhi_epi32(t0, t2)));
            Is32vec4 f = _mm_add_epi32(Is32vec4(_mm_unpacklo_epi32(t1, t3)), Is32vec4(_mm_unpackhi_epi32(t1, t3)));
            Is32vec4 sum2 = _mm_add_epi32(Is32vec4(_mm_unpacklo_epi32(e, f)), Is32vec4(_mm_unpackhi_epi32(e, f)));
            sum2 >>= 15;
            sum2 = Is32vec4(_mm_madd_epi16(Is16vec8(sum2), gain)) >> 14;
            *out1 = Is32vec4(_mm_add_epi32(*out1, sum2));
            ++out1;
            gain = Is16vec8(_mm_adds_epi16(gain, step));
            sum2 >>= 1;
            *out2 = Is32vec4(_mm_add_epi32(*out2, Is32vec4(_mm_madd_epi16(Is16vec8(sum2), panGain)) >> 14));
            ++out2;
            ++in;
        }
        break;
    }
}
// ---- behaviour test: the SSE2 kernel above against a scalar statement of the same ramped filter ----
static __m128i g_source[5 + 4 + 8];        // 40 shorts of history + (5 + count) quads of window
static __m128i g_coef[40];                 // 40 coefficient quads
static __m128i g_first[8], g_second[8];    // 2 quads per step
static int g_refFirst[32], g_refSecond[32];   // the kernel adds 32-bit lanes (read through int: `long` is 64-bit on LP64 hosts)

static int Dot(const short* x, const short* y)
{
    int s = 0;
    for (int e = 0; e < 8; ++e)
        s += (int)x[e] * (int)y[e];
    return s;
}

// scalar reference: rows 4..7 (block 1, window quads from 1) then rows 0..3 (block 2, from 2); a row r below
// `increment` takes its four quads one window quad earlier with coefficient quads 5r..5r+3, else 5r+1..5r+4
static int Saturate16(int v)
{
    return v > 32767 ? 32767 : (v < -32768 ? -32768 : v);
}

static void ReferenceStereoRampFIR(const short* source, const short* coef, unsigned int increment, unsigned int count,
                               int* first, int* second, unsigned short pan, short startGain, short gainStep)
{
    const short* window = source - 40;
    short gains[4];
    for (int lane = 0; lane < 4; ++lane)
        gains[lane] = (short)(startGain + lane * gainStep);     // truncated to 16 bits, as the word stores do
    short step = (short)(4 * gainStep);
    for (unsigned int i = 0; i < count; ++i)
    {
        for (int blk = 0; blk < 2; ++blk)
        {
            for (int lane = 0; lane < 4; ++lane)
            {
                int row = blk == 0 ? lane + 4 : lane;
                int base = blk == 0 ? 1 : 2;
                int sum = 0;
                for (int t = 0; t < 4; ++t)
                {
                    int k = (unsigned int)row < increment ? base - 1 + t : base + t;
                    int j = (unsigned int)row < increment ? 5 * row + t : 5 * row + 1 + t;
                    sum += Dot(window + 8 * (i + k), coef + 8 * j);
                }
                int scaled = ((int)(short)(sum >> 15) * (int)gains[lane]) >> 14;
                first[(2 * i + blk) * 4 + lane] += scaled;
                int half = scaled >> 1;
                second[(2 * i + blk) * 4 + lane] += ((int)(short)half * (int)(short)pan) >> 14;
            }
            for (int lane = 0; lane < 4; ++lane)
                gains[lane] = (short)Saturate16(gains[lane] + step);   // paddsw after every block
        }
    }
}

int main()
{
    const unsigned int count = 3;
    const unsigned short pans[2] = { 0x5A82, 0xC123 };
    const short ramps[3][2] = { { 0, 0x100 }, { 0x7FFF, -0x100 }, { 0x7000, 0x0C00 } };   // fade in, fade out, saturating
    unsigned int seed = 12345;
    short* src = (short*)g_source;
    short* coef = (short*)g_coef;
    for (int n = 0; n < (int)(sizeof(g_source) / 2); ++n) { seed = seed * 1103515245u + 12345u; src[n] = (short)((int)((seed >> 16) & 0x7FFF) - 0x4000); }
    for (int n = 0; n < 40 * 8; ++n) { seed = seed * 1103515245u + 12345u; coef[n] = (short)((int)((seed >> 16) & 0x3FFF) - 0x2000); }
    for (int q = 0; q < 6; ++q)
    {
        int p = q & 1;
        short startGain = ramps[q >> 1][0], gainStep = ramps[q >> 1][1];
        for (unsigned int inc = 0; inc < 8; ++inc)
        {
            int* first = (int*)g_first;
            int* second = (int*)g_second;
            for (int n = 0; n < 32; ++n) { first[n] = g_refFirst[n] = 1000 * n - 7; second[n] = g_refSecond[n] = -333 * n + 5; }
            ResampleStereoRampFIR(src + 40, (unsigned long)(size_t)coef, inc, count, (long*)g_first, (long*)g_second, pans[p], startGain, gainStep);
            ReferenceStereoRampFIR(src + 40, coef, inc, count, g_refFirst, g_refSecond, pans[p], startGain, gainStep);
            for (int n = 0; n < 32; ++n)
            {
                if (first[n] != g_refFirst[n] || second[n] != g_refSecond[n])
                {
                    std::printf("00c843f0_TEST FAIL\n");
                    return 1;
                }
            }
        }
    }
    std::printf("00c843f0_TEST PASS\n");
    return 0;
}