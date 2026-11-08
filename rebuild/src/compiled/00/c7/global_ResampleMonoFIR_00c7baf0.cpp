// ResampleMonoFIR(const short* source, unsigned long phase, unsigned long increment, unsigned long count, long* output) -- libpk software mixer: SSE2 polyphase FIR resampler for a mono 16-bit voice; for each of `count` steps it filters the sample window through the 40-quad coefficient table at `phase` into eight output phases (rows 4..7 then 0..3), each a 32-bit dot product of 16 taps shifted down 14 bits and ADDED into the 16-byte aligned mix buffer, then moves the window one quad on; `increment` (0..7) selects which rows already take the next window quad
// Leaf: ?ResampleMonoFIR@@YAXPBFKKKPAJ@Z
// Flags: /GS
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C7BAF0, 10544 bytes = code to 0x00C7E3FF + the 8-entry case table at 0x00C7E400
// (one function; ResampleMonoRampFIR 0x00C7E420 follows); `and esp,-16 ; sub esp,2D4h` frame with the /GS cookie
// (0x0139C8A8, __security_check_cookie 0x00BFE9F9).
// Identity: the landed MixFilteredVoice 0x00C70D60 / MixFilteredVoiceFadeIn 0x00C70C10 / MixFilteredVoiceFadeOut
// 0x00C70CB0 call it (their declaration `ResampleMonoFIR(const short*, unsigned long phase, unsigned long increment,
// unsigned long count, long* output) // 0x00C7BAF0`); the body is the SSE2 twin of the landed ResampleMonoMMX 0x00C78A10.
// Descriptive name (libpk, no PDB/RTTI); the catalog's "Graphics_RenderTextConditional_ProcessRows" is propagated.
// Callees: Is32vec4::operator+= 0x00C7B870 and Is32vec4::operator>>(int) 0x00C7B8B0 (out of line once the inline budget
// is spent), the rest are SSE2 intrinsics (pmaddwd / paddd / punpck[lh]dq / psrad). The classes are the Intel dvec.h
// model the landed 0x00C7B7D0..0x00C7B8F0 rows use (RTM VC7.1 ships no dvec.h).
// Shape: one `for` per case, each the same two 16-tap blocks. VC7.1 expands the plain-inline operators per call site
// until the function's inline budget runs out, which retail does at the last `+=` of case 1's first block (0x00C7C053):
// case 0 is fully inline, case 1 calls `+=` from there on, cases 2..7 call every `+=` and `>>` except the third and
// fourth window quad of the second block, which are `_mm_add_epi32` intrinsics (inline in every case). The budget is
// spent by the inlined ctor chains, so the explicit `Is32vec4(...)` around each tap product, around the intrinsic sums
// and around the first block's final unpacks are load-bearing (without them the calls start one case late, and the
// two named sums give the 0x2D4 frame).
#include <emmintrin.h>

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
    Is16vec8& operator=(const M128& a) { return *this = (Is16vec8)a; }
};

void ResampleMonoFIR(const short* source, unsigned long phase, unsigned long increment, unsigned long count, long* output)
{
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
            s0 += Is32vec4(_mm_madd_epi16(in[2], c[22]));
            s1 += Is32vec4(_mm_madd_epi16(in[2], c[27]));
            s2 += Is32vec4(_mm_madd_epi16(in[2], c[32]));
            s3 += Is32vec4(_mm_madd_epi16(in[2], c[37]));
            s0 += Is32vec4(_mm_madd_epi16(in[3], c[23]));
            s1 += Is32vec4(_mm_madd_epi16(in[3], c[28]));
            s2 += Is32vec4(_mm_madd_epi16(in[3], c[33]));
            s3 += Is32vec4(_mm_madd_epi16(in[3], c[38]));
            s0 += Is32vec4(_mm_madd_epi16(in[4], c[24]));
            s1 += Is32vec4(_mm_madd_epi16(in[4], c[29]));
            s2 += Is32vec4(_mm_madd_epi16(in[4], c[34]));
            s3 += Is32vec4(_mm_madd_epi16(in[4], c[39]));
            Is32vec4 a = _mm_add_epi32(_mm_unpacklo_epi32(s0, s2), _mm_unpackhi_epi32(s0, s2));
            Is32vec4 b = _mm_add_epi32(_mm_unpacklo_epi32(s1, s3), _mm_unpackhi_epi32(s1, s3));
            Is32vec4 sum = _mm_add_epi32(Is32vec4(_mm_unpacklo_epi32(a, b)), Is32vec4(_mm_unpackhi_epi32(a, b)));
            *out++ += sum >> 14;
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
            t0 += Is32vec4(_mm_madd_epi16(in[5], c[4]));
            t1 += Is32vec4(_mm_madd_epi16(in[5], c[9]));
            t2 += Is32vec4(_mm_madd_epi16(in[5], c[14]));
            t3 += Is32vec4(_mm_madd_epi16(in[5], c[19]));
            Is32vec4 e = _mm_add_epi32(_mm_unpacklo_epi32(t0, t2), _mm_unpackhi_epi32(t0, t2));
            Is32vec4 f = _mm_add_epi32(_mm_unpacklo_epi32(t1, t3), _mm_unpackhi_epi32(t1, t3));
            Is32vec4 sum2 = _mm_add_epi32(_mm_unpacklo_epi32(e, f), _mm_unpackhi_epi32(e, f));
            *out++ += sum2 >> 14;
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
            s0 += Is32vec4(_mm_madd_epi16(in[4], c[24]));
            s1 += Is32vec4(_mm_madd_epi16(in[4], c[29]));
            s2 += Is32vec4(_mm_madd_epi16(in[4], c[34]));
            s3 += Is32vec4(_mm_madd_epi16(in[4], c[39]));
            Is32vec4 a = _mm_add_epi32(_mm_unpacklo_epi32(s0, s2), _mm_unpackhi_epi32(s0, s2));
            Is32vec4 b = _mm_add_epi32(_mm_unpacklo_epi32(s1, s3), _mm_unpackhi_epi32(s1, s3));
            Is32vec4 sum = _mm_add_epi32(Is32vec4(_mm_unpacklo_epi32(a, b)), Is32vec4(_mm_unpackhi_epi32(a, b)));
            *out++ += sum >> 14;
            Is32vec4 t0 = _mm_madd_epi16(in[1], c[0]);
            t0 += Is32vec4(_mm_madd_epi16(in[2], c[1]));
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
            t1 += Is32vec4(_mm_madd_epi16(in[5], c[9]));
            t2 += Is32vec4(_mm_madd_epi16(in[5], c[14]));
            t3 += Is32vec4(_mm_madd_epi16(in[5], c[19]));
            Is32vec4 e = _mm_add_epi32(_mm_unpacklo_epi32(t0, t2), _mm_unpackhi_epi32(t0, t2));
            Is32vec4 f = _mm_add_epi32(_mm_unpacklo_epi32(t1, t3), _mm_unpackhi_epi32(t1, t3));
            Is32vec4 sum2 = _mm_add_epi32(_mm_unpacklo_epi32(e, f), _mm_unpackhi_epi32(e, f));
            *out++ += sum2 >> 14;
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
            s0 += Is32vec4(_mm_madd_epi16(in[4], c[24]));
            s1 += Is32vec4(_mm_madd_epi16(in[4], c[29]));
            s2 += Is32vec4(_mm_madd_epi16(in[4], c[34]));
            s3 += Is32vec4(_mm_madd_epi16(in[4], c[39]));
            Is32vec4 a = _mm_add_epi32(_mm_unpacklo_epi32(s0, s2), _mm_unpackhi_epi32(s0, s2));
            Is32vec4 b = _mm_add_epi32(_mm_unpacklo_epi32(s1, s3), _mm_unpackhi_epi32(s1, s3));
            Is32vec4 sum = _mm_add_epi32(Is32vec4(_mm_unpacklo_epi32(a, b)), Is32vec4(_mm_unpackhi_epi32(a, b)));
            *out++ += sum >> 14;
            Is32vec4 t0 = _mm_madd_epi16(in[1], c[0]);
            Is32vec4 t1 = _mm_madd_epi16(in[1], c[5]);
            t0 += Is32vec4(_mm_madd_epi16(in[2], c[1]));
            t1 += Is32vec4(_mm_madd_epi16(in[2], c[6]));
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
            t2 += Is32vec4(_mm_madd_epi16(in[5], c[14]));
            t3 += Is32vec4(_mm_madd_epi16(in[5], c[19]));
            Is32vec4 e = _mm_add_epi32(_mm_unpacklo_epi32(t0, t2), _mm_unpackhi_epi32(t0, t2));
            Is32vec4 f = _mm_add_epi32(_mm_unpacklo_epi32(t1, t3), _mm_unpackhi_epi32(t1, t3));
            Is32vec4 sum2 = _mm_add_epi32(_mm_unpacklo_epi32(e, f), _mm_unpackhi_epi32(e, f));
            *out++ += sum2 >> 14;
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
            s0 += Is32vec4(_mm_madd_epi16(in[4], c[24]));
            s1 += Is32vec4(_mm_madd_epi16(in[4], c[29]));
            s2 += Is32vec4(_mm_madd_epi16(in[4], c[34]));
            s3 += Is32vec4(_mm_madd_epi16(in[4], c[39]));
            Is32vec4 a = _mm_add_epi32(_mm_unpacklo_epi32(s0, s2), _mm_unpackhi_epi32(s0, s2));
            Is32vec4 b = _mm_add_epi32(_mm_unpacklo_epi32(s1, s3), _mm_unpackhi_epi32(s1, s3));
            Is32vec4 sum = _mm_add_epi32(Is32vec4(_mm_unpacklo_epi32(a, b)), Is32vec4(_mm_unpackhi_epi32(a, b)));
            *out++ += sum >> 14;
            Is32vec4 t0 = _mm_madd_epi16(in[1], c[0]);
            Is32vec4 t1 = _mm_madd_epi16(in[1], c[5]);
            Is32vec4 t2 = _mm_madd_epi16(in[1], c[10]);
            t0 += Is32vec4(_mm_madd_epi16(in[2], c[1]));
            t1 += Is32vec4(_mm_madd_epi16(in[2], c[6]));
            t2 += Is32vec4(_mm_madd_epi16(in[2], c[11]));
            Is32vec4 t3 = _mm_madd_epi16(in[2], c[16]);
            t0 = Is32vec4(_mm_add_epi32(t0, Is32vec4(_mm_madd_epi16(in[3], c[2]))));
            t1 = Is32vec4(_mm_add_epi32(t1, Is32vec4(_mm_madd_epi16(in[3], c[7]))));
            t2 = Is32vec4(_mm_add_epi32(t2, Is32vec4(_mm_madd_epi16(in[3], c[12]))));
            t3 = Is32vec4(_mm_add_epi32(t3, Is32vec4(_mm_madd_epi16(in[3], c[17]))));
            t0 = Is32vec4(_mm_add_epi32(t0, Is32vec4(_mm_madd_epi16(in[4], c[3]))));
            t1 = Is32vec4(_mm_add_epi32(t1, Is32vec4(_mm_madd_epi16(in[4], c[8]))));
            t2 = Is32vec4(_mm_add_epi32(t2, Is32vec4(_mm_madd_epi16(in[4], c[13]))));
            t3 = Is32vec4(_mm_add_epi32(t3, Is32vec4(_mm_madd_epi16(in[4], c[18]))));
            t3 += Is32vec4(_mm_madd_epi16(in[5], c[19]));
            Is32vec4 e = _mm_add_epi32(_mm_unpacklo_epi32(t0, t2), _mm_unpackhi_epi32(t0, t2));
            Is32vec4 f = _mm_add_epi32(_mm_unpacklo_epi32(t1, t3), _mm_unpackhi_epi32(t1, t3));
            Is32vec4 sum2 = _mm_add_epi32(_mm_unpacklo_epi32(e, f), _mm_unpackhi_epi32(e, f));
            *out++ += sum2 >> 14;
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
            s0 += Is32vec4(_mm_madd_epi16(in[4], c[24]));
            s1 += Is32vec4(_mm_madd_epi16(in[4], c[29]));
            s2 += Is32vec4(_mm_madd_epi16(in[4], c[34]));
            s3 += Is32vec4(_mm_madd_epi16(in[4], c[39]));
            Is32vec4 a = _mm_add_epi32(_mm_unpacklo_epi32(s0, s2), _mm_unpackhi_epi32(s0, s2));
            Is32vec4 b = _mm_add_epi32(_mm_unpacklo_epi32(s1, s3), _mm_unpackhi_epi32(s1, s3));
            Is32vec4 sum = _mm_add_epi32(Is32vec4(_mm_unpacklo_epi32(a, b)), Is32vec4(_mm_unpackhi_epi32(a, b)));
            *out++ += sum >> 14;
            Is32vec4 t0 = _mm_madd_epi16(in[1], c[0]);
            Is32vec4 t1 = _mm_madd_epi16(in[1], c[5]);
            Is32vec4 t2 = _mm_madd_epi16(in[1], c[10]);
            Is32vec4 t3 = _mm_madd_epi16(in[1], c[15]);
            t0 += Is32vec4(_mm_madd_epi16(in[2], c[1]));
            t1 += Is32vec4(_mm_madd_epi16(in[2], c[6]));
            t2 += Is32vec4(_mm_madd_epi16(in[2], c[11]));
            t3 += Is32vec4(_mm_madd_epi16(in[2], c[16]));
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
            Is32vec4 sum2 = _mm_add_epi32(_mm_unpacklo_epi32(e, f), _mm_unpackhi_epi32(e, f));
            *out++ += sum2 >> 14;
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
            s0 += Is32vec4(_mm_madd_epi16(in[3], c[23]));
            s1 += Is32vec4(_mm_madd_epi16(in[3], c[28]));
            s2 += Is32vec4(_mm_madd_epi16(in[3], c[33]));
            s3 += Is32vec4(_mm_madd_epi16(in[3], c[38]));
            s1 += Is32vec4(_mm_madd_epi16(in[4], c[29]));
            s2 += Is32vec4(_mm_madd_epi16(in[4], c[34]));
            s3 += Is32vec4(_mm_madd_epi16(in[4], c[39]));
            Is32vec4 a = _mm_add_epi32(_mm_unpacklo_epi32(s0, s2), _mm_unpackhi_epi32(s0, s2));
            Is32vec4 b = _mm_add_epi32(_mm_unpacklo_epi32(s1, s3), _mm_unpackhi_epi32(s1, s3));
            Is32vec4 sum = _mm_add_epi32(Is32vec4(_mm_unpacklo_epi32(a, b)), Is32vec4(_mm_unpackhi_epi32(a, b)));
            *out++ += sum >> 14;
            Is32vec4 t0 = _mm_madd_epi16(in[1], c[0]);
            Is32vec4 t1 = _mm_madd_epi16(in[1], c[5]);
            Is32vec4 t2 = _mm_madd_epi16(in[1], c[10]);
            Is32vec4 t3 = _mm_madd_epi16(in[1], c[15]);
            t0 += Is32vec4(_mm_madd_epi16(in[2], c[1]));
            t1 += Is32vec4(_mm_madd_epi16(in[2], c[6]));
            t2 += Is32vec4(_mm_madd_epi16(in[2], c[11]));
            t3 += Is32vec4(_mm_madd_epi16(in[2], c[16]));
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
            Is32vec4 sum2 = _mm_add_epi32(_mm_unpacklo_epi32(e, f), _mm_unpackhi_epi32(e, f));
            *out++ += sum2 >> 14;
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
            s0 += Is32vec4(_mm_madd_epi16(in[3], c[23]));
            s1 += Is32vec4(_mm_madd_epi16(in[3], c[28]));
            s2 += Is32vec4(_mm_madd_epi16(in[3], c[33]));
            s3 += Is32vec4(_mm_madd_epi16(in[3], c[38]));
            s2 += Is32vec4(_mm_madd_epi16(in[4], c[34]));
            s3 += Is32vec4(_mm_madd_epi16(in[4], c[39]));
            Is32vec4 a = _mm_add_epi32(_mm_unpacklo_epi32(s0, s2), _mm_unpackhi_epi32(s0, s2));
            Is32vec4 b = _mm_add_epi32(_mm_unpacklo_epi32(s1, s3), _mm_unpackhi_epi32(s1, s3));
            Is32vec4 sum = _mm_add_epi32(Is32vec4(_mm_unpacklo_epi32(a, b)), Is32vec4(_mm_unpackhi_epi32(a, b)));
            *out++ += sum >> 14;
            Is32vec4 t0 = _mm_madd_epi16(in[1], c[0]);
            Is32vec4 t1 = _mm_madd_epi16(in[1], c[5]);
            Is32vec4 t2 = _mm_madd_epi16(in[1], c[10]);
            Is32vec4 t3 = _mm_madd_epi16(in[1], c[15]);
            t0 += Is32vec4(_mm_madd_epi16(in[2], c[1]));
            t1 += Is32vec4(_mm_madd_epi16(in[2], c[6]));
            t2 += Is32vec4(_mm_madd_epi16(in[2], c[11]));
            t3 += Is32vec4(_mm_madd_epi16(in[2], c[16]));
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
            Is32vec4 sum2 = _mm_add_epi32(_mm_unpacklo_epi32(e, f), _mm_unpackhi_epi32(e, f));
            *out++ += sum2 >> 14;
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
            s0 += Is32vec4(_mm_madd_epi16(in[3], c[23]));
            s1 += Is32vec4(_mm_madd_epi16(in[3], c[28]));
            s2 += Is32vec4(_mm_madd_epi16(in[3], c[33]));
            s3 += Is32vec4(_mm_madd_epi16(in[3], c[38]));
            s3 += Is32vec4(_mm_madd_epi16(in[4], c[39]));
            Is32vec4 a = _mm_add_epi32(_mm_unpacklo_epi32(s0, s2), _mm_unpackhi_epi32(s0, s2));
            Is32vec4 b = _mm_add_epi32(_mm_unpacklo_epi32(s1, s3), _mm_unpackhi_epi32(s1, s3));
            Is32vec4 sum = _mm_add_epi32(Is32vec4(_mm_unpacklo_epi32(a, b)), Is32vec4(_mm_unpackhi_epi32(a, b)));
            *out++ += sum >> 14;
            Is32vec4 t0 = _mm_madd_epi16(in[1], c[0]);
            Is32vec4 t1 = _mm_madd_epi16(in[1], c[5]);
            Is32vec4 t2 = _mm_madd_epi16(in[1], c[10]);
            Is32vec4 t3 = _mm_madd_epi16(in[1], c[15]);
            t0 += Is32vec4(_mm_madd_epi16(in[2], c[1]));
            t1 += Is32vec4(_mm_madd_epi16(in[2], c[6]));
            t2 += Is32vec4(_mm_madd_epi16(in[2], c[11]));
            t3 += Is32vec4(_mm_madd_epi16(in[2], c[16]));
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
            Is32vec4 sum2 = _mm_add_epi32(_mm_unpacklo_epi32(e, f), _mm_unpackhi_epi32(e, f));
            *out++ += sum2 >> 14;
            ++in;
        }
        break;
    }
}