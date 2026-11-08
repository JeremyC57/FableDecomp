// ResampleStereoFIR(const short* source, unsigned long phase, unsigned long increment, unsigned long count, long* first, long* second, unsigned short pan) -- libpk software mixer: SSE2 polyphase FIR resampler for a mono 16-bit voice into a stereo pair: for each of `count` steps the sample window is filtered through the 40-quad coefficient table at `phase` into eight output phases (rows 4..7 then 0..3, two 16-tap blocks); each block's four 32-bit sums are shifted down 14 bits and ADDED into `first`, then shifted down one more bit, weighted by the 16-bit `pan` (pmaddwd against {pan, 0} lanes), shifted down 14 and ADDED into `second`; the window then moves on one quad, and `increment` (0..7) selects which rows already take the next window quad
// Leaf: ?ResampleStereoFIR@@YAXPBFKKKPAJ1G@Z
// Flags: /GS
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C812F0, 12532 bytes = code to 0x00C843C3 + the 8-entry case table at 0x00C843C4
// (the catalog's 12544 includes 12 bytes of int3 padding; one function, ResampleStereoRampFIR 0x00C843F0 follows);
// `and esp,-16 ; sub esp,254h` frame with the /GS cookie (0x0139C8A8, __security_check_cookie 0x00BFE9F9).
// Identity: the landed MixFilteredVoice 0x00C70D60 calls it twice (0x00C70DC1 / 0x00C70E00, front/rear swapped by the
// voice flag) with the declaration `ResampleStereoFIR(const short*, unsigned long phase, unsigned long increment,
// unsigned long count, long* first, long* second, unsigned short pan) // 0x00C812F0` (reloc_bindings.tsv binds
// ?ResampleStereoFIR@@YAXPBFKKKPAJ1G@Z here); the body is ResampleMonoFIR 0x00C7BAF0 with the stereo output stage of the
// landed MMX twin ResampleStereoRampMMX 0x00C7AB20 (`>> 1`, pmaddwd by {pan, 0}, `>> 14` into the second buffer).
// Descriptive name (libpk, no PDB/RTTI); the catalog's "Audio_ConvolveFilter_LongComplex" is propagated.
// Callees: Is32vec4::operator+= 0x00C7B870, Is32vec4::operator>>(int) 0x00C7B8B0 and Is32vec4::operator>>=(int)
// 0x00C7B8F0 once the inline budget is spent; the rest are SSE2 intrinsics (pmaddwd / paddd / punpck[lh]dq / psrad).
// The classes are the Intel dvec.h model of the landed 0x00C7B7D0..0x00C7B8F0 rows (RTM VC7.1 ships no dvec.h).
// Shape: the pan vector is a dvec.h Is16vec8 eight-short constructor (the four `pan` and four zero word stores before
// the case jump). VC7.1 expands the plain-inline operators per call site until the function's inline budget runs out,
// which retail does at the second `+=` of case 0's last window quad (0x00C816A3): case 0 is inline up to there and calls
// `+=` / `>>=` from there on (its last `>> 14` stays inline), cases 1..7 call every `+=`, `>>=` and `>>` except the
// third and fourth window quad of the second block, which are `_mm_add_epi32` intrinsics in every case. The budget is
// spent by the inlined ctor chains: the explicit `Is32vec4(...)` around the second block's intrinsic sums and around
// both blocks' final unpacks place the split exactly (found by enumerating the 128 combinations of seven wrap
// features: four match; this one is the wrap set of ResampleMonoRampFIR 0x00C7E420). The buffer adds are intrinsics
// in every case.
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

void ResampleStereoFIR(const short* source, unsigned long phase, unsigned long increment, unsigned long count,
                       long* first, long* second, unsigned short pan)
{
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
            sum >>= 14;
            *out1 = Is32vec4(_mm_add_epi32(*out1, sum));
            ++out1;
            sum >>= 1;
            *out2 = Is32vec4(_mm_add_epi32(*out2, Is32vec4(_mm_madd_epi16(sum, panGain)) >> 14));
            ++out2;
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
            sum2 >>= 14;
            *out1 = Is32vec4(_mm_add_epi32(*out1, sum2));
            ++out1;
            sum2 >>= 1;
            *out2 = Is32vec4(_mm_add_epi32(*out2, Is32vec4(_mm_madd_epi16(sum2, panGain)) >> 14));
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
            sum >>= 14;
            *out1 = Is32vec4(_mm_add_epi32(*out1, sum));
            ++out1;
            sum >>= 1;
            *out2 = Is32vec4(_mm_add_epi32(*out2, Is32vec4(_mm_madd_epi16(sum, panGain)) >> 14));
            ++out2;
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
            sum2 >>= 14;
            *out1 = Is32vec4(_mm_add_epi32(*out1, sum2));
            ++out1;
            sum2 >>= 1;
            *out2 = Is32vec4(_mm_add_epi32(*out2, Is32vec4(_mm_madd_epi16(sum2, panGain)) >> 14));
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
            sum >>= 14;
            *out1 = Is32vec4(_mm_add_epi32(*out1, sum));
            ++out1;
            sum >>= 1;
            *out2 = Is32vec4(_mm_add_epi32(*out2, Is32vec4(_mm_madd_epi16(sum, panGain)) >> 14));
            ++out2;
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
            sum2 >>= 14;
            *out1 = Is32vec4(_mm_add_epi32(*out1, sum2));
            ++out1;
            sum2 >>= 1;
            *out2 = Is32vec4(_mm_add_epi32(*out2, Is32vec4(_mm_madd_epi16(sum2, panGain)) >> 14));
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
            sum >>= 14;
            *out1 = Is32vec4(_mm_add_epi32(*out1, sum));
            ++out1;
            sum >>= 1;
            *out2 = Is32vec4(_mm_add_epi32(*out2, Is32vec4(_mm_madd_epi16(sum, panGain)) >> 14));
            ++out2;
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
            sum2 >>= 14;
            *out1 = Is32vec4(_mm_add_epi32(*out1, sum2));
            ++out1;
            sum2 >>= 1;
            *out2 = Is32vec4(_mm_add_epi32(*out2, Is32vec4(_mm_madd_epi16(sum2, panGain)) >> 14));
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
            sum >>= 14;
            *out1 = Is32vec4(_mm_add_epi32(*out1, sum));
            ++out1;
            sum >>= 1;
            *out2 = Is32vec4(_mm_add_epi32(*out2, Is32vec4(_mm_madd_epi16(sum, panGain)) >> 14));
            ++out2;
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
            sum2 >>= 14;
            *out1 = Is32vec4(_mm_add_epi32(*out1, sum2));
            ++out1;
            sum2 >>= 1;
            *out2 = Is32vec4(_mm_add_epi32(*out2, Is32vec4(_mm_madd_epi16(sum2, panGain)) >> 14));
            ++out2;
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
            sum >>= 14;
            *out1 = Is32vec4(_mm_add_epi32(*out1, sum));
            ++out1;
            sum >>= 1;
            *out2 = Is32vec4(_mm_add_epi32(*out2, Is32vec4(_mm_madd_epi16(sum, panGain)) >> 14));
            ++out2;
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
            sum2 >>= 14;
            *out1 = Is32vec4(_mm_add_epi32(*out1, sum2));
            ++out1;
            sum2 >>= 1;
            *out2 = Is32vec4(_mm_add_epi32(*out2, Is32vec4(_mm_madd_epi16(sum2, panGain)) >> 14));
            ++out2;
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
            sum >>= 14;
            *out1 = Is32vec4(_mm_add_epi32(*out1, sum));
            ++out1;
            sum >>= 1;
            *out2 = Is32vec4(_mm_add_epi32(*out2, Is32vec4(_mm_madd_epi16(sum, panGain)) >> 14));
            ++out2;
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
            sum2 >>= 14;
            *out1 = Is32vec4(_mm_add_epi32(*out1, sum2));
            ++out1;
            sum2 >>= 1;
            *out2 = Is32vec4(_mm_add_epi32(*out2, Is32vec4(_mm_madd_epi16(sum2, panGain)) >> 14));
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
            sum >>= 14;
            *out1 = Is32vec4(_mm_add_epi32(*out1, sum));
            ++out1;
            sum >>= 1;
            *out2 = Is32vec4(_mm_add_epi32(*out2, Is32vec4(_mm_madd_epi16(sum, panGain)) >> 14));
            ++out2;
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
            sum2 >>= 14;
            *out1 = Is32vec4(_mm_add_epi32(*out1, sum2));
            ++out1;
            sum2 >>= 1;
            *out2 = Is32vec4(_mm_add_epi32(*out2, Is32vec4(_mm_madd_epi16(sum2, panGain)) >> 14));
            ++out2;
            ++in;
        }
        break;
    }
}