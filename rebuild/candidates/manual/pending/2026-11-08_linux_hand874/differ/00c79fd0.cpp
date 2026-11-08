// ResampleStereoMMX(const short* source, unsigned long phase, unsigned long increment, unsigned long count, long* output1, long* output2, unsigned short panGain) -- MMX 8-tap polyphase resampler into a stereo pair at constant gain: each 32-bit sample (four 7-tap pmaddwd sums folded, >>14) is added into output1, and the sample >>1 weighted by the pan gain (pmaddwd against {panGain, 0}) >>14 into output2
// Residue: DIFFER 2884v2884, same length, same slots, same integer code -- only an MMX register swap in all four cases: retail sums the last s3 term into the chain temp and copies it out (`paddd mm0,mm1 ; movq mm1,mm0`, s3 in mm1, the pan reload and the s1 reload in mm2); VC7.1 forwards the chain temp into s3 (`movq mm2,mm0 ; paddd mm2,mm1`, s3 in mm2, pan/s1 in mm1) -- c2prio: s3 is local #40 prio 168 key 118, coloured after s1 (#37, prio 168 key 198) and lands on reg60. The mono sibling 0x00C78A10 has the forwarded shape in retail too (matched), so this is the copy-forwarding/colouring of one live range. Probe: 54 grades -- 12 pan/output spellings (sA), scope of s0..s3 (sB), 8 raw-intrinsic tails (sC), 6 inline-pressure tails (sD), += accumulation / __m64 / I32vec2 s3 (sE), loop forms (sF), /d2inlT40/T120/S5/S15 (sG): every C++ spelling compiles to the identical 199-line swap.
// Next: the stereo-ramp sibling (0x00C7AB20, matched) has the same tail; find which declaration order or extra live value pins the s3 copy (e.g. a named `Is32vec2 last = mul_add(in[7], c[31])` forced across the fold); otherwise park as an MMX colouring tie.
// Leaf: ?ResampleStereoMMX@@YAXPBFKKKPAJ1G@Z
// Flags: /GS
#include <mmintrin.h>

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

void ResampleStereoMMX(const short* source, unsigned long phase, unsigned long increment, unsigned long count, long* output1, long* output2, unsigned short panGain)
{
    __declspec(align(16)) Is16vec4 pan(0, (short)panGain, 0, (short)panGain);
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
            Is32vec2 a = (unpack_low(s0, s1) + unpack_high(s0, s1)) >> 14;
            *first++ += a;
            *second++ += mul_add(Is16vec4(a >> 1), pan) >> 14;
            Is32vec2 b = (unpack_low(s2, s3) + unpack_high(s2, s3)) >> 14;
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
            Is32vec2 a = (unpack_low(s0, s1) + unpack_high(s0, s1)) >> 14;
            *first++ += a;
            *second++ += mul_add(Is16vec4(a >> 1), pan) >> 14;
            Is32vec2 b = (unpack_low(s2, s3) + unpack_high(s2, s3)) >> 14;
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
            Is32vec2 a = (unpack_low(s0, s1) + unpack_high(s0, s1)) >> 14;
            *first++ += a;
            *second++ += mul_add(Is16vec4(a >> 1), pan) >> 14;
            Is32vec2 b = (unpack_low(s2, s3) + unpack_high(s2, s3)) >> 14;
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
            Is32vec2 a = (unpack_low(s0, s1) + unpack_high(s0, s1)) >> 14;
            *first++ += a;
            *second++ += mul_add(Is16vec4(a >> 1), pan) >> 14;
            Is32vec2 b = (unpack_low(s2, s3) + unpack_high(s2, s3)) >> 14;
            *first++ += b;
            *second++ += mul_add(Is16vec4(b >> 1), pan) >> 14;
            ++in;
        }
        break;
    }
    _mm_empty();
}
