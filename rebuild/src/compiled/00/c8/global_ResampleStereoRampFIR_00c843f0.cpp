// ResampleStereoRampFIR(const short* source, unsigned long phase, unsigned long increment, unsigned long count, long* first, long* second, unsigned short pan, short startGain, short gainStep) -- libpk software mixer: the gain-ramped twin of ResampleStereoFIR, used for fade-in / fade-out: the same SSE2 polyphase FIR over `count` steps (two 16-tap blocks giving output phases 4..7 and 0..3), but each block's four sums are shifted down 15 bits, scaled by a per-phase 16-bit gain (startGain + k*gainStep for the four lanes, all four advanced by 4*gainStep with signed saturation after every block) and shifted down 14 more; that value is ADDED into `first`, then halved, weighted by the 16-bit `pan` (pmaddwd against {pan, 0} lanes), shifted down 14 and ADDED into `second`
// Leaf: ?ResampleStereoRampFIR@@YAXPBFKKKPAJ1GFF@Z
// Flags: /GS
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C843F0, 15328 bytes = code to 0x00C87FAC, `lea ecx,[ecx]` pad, the 8-entry case table
// at 0x00C87FB0 (= the catalog length; one function; winmm import thunks follow); `and esp,-16 ; sub esp,4A4h` frame with
// the /GS cookie (0x0139C8A8, __security_check_cookie 0x00BFE9F9).
// Identity: the landed MixFilteredVoiceFadeIn 0x00C70C10 / MixFilteredVoiceFadeOut 0x00C70CB0 call it (0x00C70C66 /
// 0x00C70D09, front/rear swapped by the voice flag) with (0, 0x100) / (0x7FFF, -0x100); it is ResampleStereoFIR 0x00C812F0
// plus the gain stage of ResampleMonoRampFIR 0x00C7E420 and the SSE2 twin of the landed ResampleStereoRampMMX 0x00C7AB20.
// The gains are `short` as in both twins: with `long` parameters the first lane-gain lea comes out `[eax+edx]` (retail
// `[edx+eax]`) and the inline split moves (probe/long1.cpp DIFFER 15308v15328), so the landed callers' `long startGain,
// long gainStep` spelling is wrong (NAME_FIXES.tsv). Descriptive name (libpk, no PDB/RTTI); the catalog's
// "Audio_ConvolveFilter_LongFull" is propagated.
// Callees: Is32vec4::operator+= 0x00C7B870, Is32vec4::operator>>(int) 0x00C7B8B0, Is32vec4::operator>>=(int) 0x00C7B8F0 and,
// once even the constructors run out of inline budget (case 7), the out-of-line dvec.h constructors
// Is32vec4::Is32vec4(__m128i) 0x00C7B840, I32vec4::I32vec4(__m128i) 0x00C7B810 (the implicit `+=` argument) and
// Is16vec8::Is16vec8(__m128i) 0x00C7B970; the rest are SSE2 intrinsics (pmaddwd / paddd / punpck[lh]dq / psrad / paddsw).
// Shape: gain, step and pan are dvec.h Is16vec8 eight-short constructors (the 24 word stores before the case jump). Case 7
// calls every operator and constructor after its first block's `a` reduction, which shows the spelling: `Is16vec8(sum)`
// around both pmaddwd inputs, `gain = Is16vec8(...)`, `*out = Is32vec4(...)` (dword copies), no wrap on the second block's
// third / fourth quad products or on the `b` reduction, both unpacks wrapped in the second block's e / f / sum2 and the
// first block's sum. The rest of the split is VC7.1's whole-function inline budget: `+=` / `>>=` stop expanding at the
// last `+=` of case 0's first block (0x00C84699), `>>` a little later, the constructors at case 7's `b` reduction
// (0x00C877B9). Every statement anywhere moves both points (VC7.1 meters the size of the whole function), so they are
// placed by the explicit `Is32vec4(...)` wraps whose constructors expand everywhere before that: `a`'s unpacks and the
// first two `+=` rounds of the first block (only the first round in case 0). Found by enumerating the wrap placements
// (probe/genr.py; no uniform placement, /d2inlS or /d2inlT value reproduces both points).
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