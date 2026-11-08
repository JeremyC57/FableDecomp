// ResampleMonoRampFIR(const short* source, unsigned long phase, unsigned long increment, unsigned long count, long* output, short startGain, short gainStep) -- libpk software mixer: the gain-ramped twin of ResampleMonoFIR: the same SSE2 polyphase FIR over `count` steps (two 16-tap blocks giving output phases 4..7 and 0..3), but each block's sums are shifted down 15 bits, scaled by a per-phase 16-bit gain (startGain + k*gainStep for the four lanes, all four advanced by 4*gainStep with signed saturation after every block) and shifted down 14 more before being ADDED into the mix buffer; used for fade-in / fade-out
// Leaf: ?ResampleMonoRampFIR@@YAXPBFKKKPAJFF@Z
// Flags: /GS
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C7E420, 11980 bytes = code to 0x00C812CB + the 8-entry case table at 0x00C812CC
// (the catalog's 11984 includes 4 bytes of int3 padding; one function); `and esp,-16 ; sub esp,254h` frame with the /GS
// cookie (0x0139C8A8, __security_check_cookie 0x00BFE9F9).
// Identity: the landed MixFilteredVoiceFadeIn 0x00C70C10 / MixFilteredVoiceFadeOut 0x00C70CB0 call it with (0, 0x100) and
// (0x7FFF, -0x100) (`push` immediates, the same bytes for a short or a long parameter); it is ResampleMonoFIR 0x00C7BAF0
// plus the gain stage, and the SSE2 twin of ResampleMonoRampMMX 0x00C79440. Descriptive name (libpk, no PDB/RTTI); the
// catalog's "Audio_ConvolveFilter_LongVariant" is propagated. The gains are `short` as in the MMX twin (chunk 874): with
// `long` parameters the first lane-gain lea comes out `[eax+ecx]` (retail `[ecx+eax]`) and the extra conversions move
// the inline split, so the landed callers' `long startGain, long gainStep` spelling is wrong (NAME_FIXES.tsv).
// Callees: Is32vec4::operator+= 0x00C7B870, Is32vec4::operator>>(int) 0x00C7B8B0 and Is32vec4::operator>>=(int) 0x00C7B8F0
// once the inline budget is spent; the gain stage is intrinsics (pmaddwd / psrad / paddd / paddsw).
// Shape: the gain vectors are dvec.h Is16vec8 eight-short constructors (the 16 word stores ahead of the case jump). As
// in ResampleMonoFIR the inline budget is spent by the inlined ctor chains: with the explicit `Is32vec4(...)` around the
// intrinsic sums of the second block, both blocks' final unpacks and the mix-buffer add, retail's split falls out -- case
// 0 calls `+=` at the last two taps of its second block (0x00C7E81A, 0x00C7E846) and its `>>=` there, cases 1..7 call
// every `+=`, `>>=` and `>>` except the intrinsic third / fourth quad of the second block. The mix-buffer add and the
// gain step (`paddsw`) are intrinsics in every case.
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