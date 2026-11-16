// ResampleStereoMMX(const short* source, unsigned long phase, unsigned long increment, unsigned long count, long* output1, long* output2, unsigned short panGain) -- MMX 8-tap polyphase resampler into a stereo pair at constant gain: each 32-bit sample (four 7-tap pmaddwd sums folded, >>14) is added into output1, and the sample >>1 weighted by the pan gain (pmaddwd against {panGain, 0}) >>14 into output2
// Leaf: ?ResampleStereoMMX@@YAXPBFKKKPAJ1G@Z
// Flags: /GS
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C79FD0, 2884 bytes incl. the 4-entry switch table at 0x00C7AB04 (+ int3 padding to 0x00C7AB20), `ret` (__cdecl).
// Identity: the catalog label Audio_ConvolveFilter_WithGain is wrong; the row is ?ResampleStereoMMX@@YAXPBFKKKPAJ1G@Z as bound in
//   rebuild/oracles/reloc_bindings.tsv and declared by the landed callers MixVoiceFadeIn 0x00C70240 / MixVoiceFadeOut 0x00C702E0 /
//   MixVoice 0x00C70390 (libpk software mixer: no PDB / RTTI, so the name is DESCRIPTIVE, the callers' spelling).
// Library: the Intel ivec.h class library (M64 / Is32vec2 / Is16vec4 + the free mul_add / unpack helpers, out-of-line copies at
//   0x00C78590..0x00C78970 in this TU), restated here with the header's own member spellings exactly as the landed siblings
//   ResampleMonoMMX 0x00C78A10, ResampleMonoRampMMX 0x00C79440 and ResampleStereoRampMMX 0x00C7AB20 do; every MMX instruction is
//   a compiler-scheduled intrinsic (no inline asm). Built under /GS like the siblings (aligned frame, cookie 0x0139C8A8 checked by
//   __security_check_cookie 0x00BFE9F9 at each of the duplicated returns).
// Shape: the stereo-ramp sibling's sums s0..s3 per case; each output pair is the unshifted fold a (b), added >>14 into output1 and,
//   halved and weighted by the pan gain (pmaddwd against {panGain, 0}), >>14 into output2.
// FITTED (chosen by grading; behaviour identical): (1) the fold is kept unshifted in a / b and `a >> 14` is written at both uses
//   (one more MMX candidate per output pair: without it c2's priority tie colours the s1 reload before s3, retail `paddd mm0,mm1 ;
//   movq mm1,mm0` at 0x00C7A230 v ours `movq mm2,mm0 ; paddd mm2,mm1`); (2) a, b and the pan products u, v are declared at the top
//   of the loop body and assigned (an assignment to a declared object is not forwarded into the += tree, so the pmaddwd precedes the
//   output2 post-increment in c2's IL order and wins the scheduler tie at 0x00C7A2A3: `pmaddwd mm0,mm2` before `mov edi,esi`).
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
            Is32vec2 a, b, u, v;
            Is32vec2 s0 = mul_add(in[1], c[1]) + mul_add(in[2], c[2]) + mul_add(in[3], c[3]) + mul_add(in[4], c[4]) + mul_add(in[5], c[5]) + mul_add(in[6], c[6]) + mul_add(in[7], c[7]);
            Is32vec2 s1 = mul_add(in[1], c[9]) + mul_add(in[2], c[10]) + mul_add(in[3], c[11]) + mul_add(in[4], c[12]) + mul_add(in[5], c[13]) + mul_add(in[6], c[14]) + mul_add(in[7], c[15]);
            Is32vec2 s2 = mul_add(in[1], c[17]) + mul_add(in[2], c[18]) + mul_add(in[3], c[19]) + mul_add(in[4], c[20]) + mul_add(in[5], c[21]) + mul_add(in[6], c[22]) + mul_add(in[7], c[23]);
            Is32vec2 s3 = mul_add(in[1], c[25]) + mul_add(in[2], c[26]) + mul_add(in[3], c[27]) + mul_add(in[4], c[28]) + mul_add(in[5], c[29]) + mul_add(in[6], c[30]) + mul_add(in[7], c[31]);
            a = unpack_low(s0, s1) + unpack_high(s0, s1);
            *first++ += a >> 14;
            u = mul_add(Is16vec4((a >> 14) >> 1), pan);
            *second++ += u >> 14;
            b = unpack_low(s2, s3) + unpack_high(s2, s3);
            *first++ += b >> 14;
            v = mul_add(Is16vec4((b >> 14) >> 1), pan);
            *second++ += v >> 14;
            ++in;
        }
        break;
    case 1:
        for (unsigned long i = 0; i < count; ++i)
        {
            Is32vec2 a, b, u, v;
            Is32vec2 s0 = mul_add(in[0], c[0]) + mul_add(in[1], c[1]) + mul_add(in[2], c[2]) + mul_add(in[3], c[3]) + mul_add(in[4], c[4]) + mul_add(in[5], c[5]) + mul_add(in[6], c[6]);
            Is32vec2 s1 = mul_add(in[1], c[9]) + mul_add(in[2], c[10]) + mul_add(in[3], c[11]) + mul_add(in[4], c[12]) + mul_add(in[5], c[13]) + mul_add(in[6], c[14]) + mul_add(in[7], c[15]);
            Is32vec2 s2 = mul_add(in[1], c[17]) + mul_add(in[2], c[18]) + mul_add(in[3], c[19]) + mul_add(in[4], c[20]) + mul_add(in[5], c[21]) + mul_add(in[6], c[22]) + mul_add(in[7], c[23]);
            Is32vec2 s3 = mul_add(in[1], c[25]) + mul_add(in[2], c[26]) + mul_add(in[3], c[27]) + mul_add(in[4], c[28]) + mul_add(in[5], c[29]) + mul_add(in[6], c[30]) + mul_add(in[7], c[31]);
            a = unpack_low(s0, s1) + unpack_high(s0, s1);
            *first++ += a >> 14;
            u = mul_add(Is16vec4((a >> 14) >> 1), pan);
            *second++ += u >> 14;
            b = unpack_low(s2, s3) + unpack_high(s2, s3);
            *first++ += b >> 14;
            v = mul_add(Is16vec4((b >> 14) >> 1), pan);
            *second++ += v >> 14;
            ++in;
        }
        break;
    case 2:
        for (unsigned long i = 0; i < count; ++i)
        {
            Is32vec2 a, b, u, v;
            Is32vec2 s0 = mul_add(in[0], c[0]) + mul_add(in[1], c[1]) + mul_add(in[2], c[2]) + mul_add(in[3], c[3]) + mul_add(in[4], c[4]) + mul_add(in[5], c[5]) + mul_add(in[6], c[6]);
            Is32vec2 s1 = mul_add(in[0], c[8]) + mul_add(in[1], c[9]) + mul_add(in[2], c[10]) + mul_add(in[3], c[11]) + mul_add(in[4], c[12]) + mul_add(in[5], c[13]) + mul_add(in[6], c[14]);
            Is32vec2 s2 = mul_add(in[1], c[17]) + mul_add(in[2], c[18]) + mul_add(in[3], c[19]) + mul_add(in[4], c[20]) + mul_add(in[5], c[21]) + mul_add(in[6], c[22]) + mul_add(in[7], c[23]);
            Is32vec2 s3 = mul_add(in[1], c[25]) + mul_add(in[2], c[26]) + mul_add(in[3], c[27]) + mul_add(in[4], c[28]) + mul_add(in[5], c[29]) + mul_add(in[6], c[30]) + mul_add(in[7], c[31]);
            a = unpack_low(s0, s1) + unpack_high(s0, s1);
            *first++ += a >> 14;
            u = mul_add(Is16vec4((a >> 14) >> 1), pan);
            *second++ += u >> 14;
            b = unpack_low(s2, s3) + unpack_high(s2, s3);
            *first++ += b >> 14;
            v = mul_add(Is16vec4((b >> 14) >> 1), pan);
            *second++ += v >> 14;
            ++in;
        }
        break;
    case 3:
        for (unsigned long i = 0; i < count; ++i)
        {
            Is32vec2 a, b, u, v;
            Is32vec2 s0 = mul_add(in[0], c[0]) + mul_add(in[1], c[1]) + mul_add(in[2], c[2]) + mul_add(in[3], c[3]) + mul_add(in[4], c[4]) + mul_add(in[5], c[5]) + mul_add(in[6], c[6]);
            Is32vec2 s1 = mul_add(in[0], c[8]) + mul_add(in[1], c[9]) + mul_add(in[2], c[10]) + mul_add(in[3], c[11]) + mul_add(in[4], c[12]) + mul_add(in[5], c[13]) + mul_add(in[6], c[14]);
            Is32vec2 s2 = mul_add(in[0], c[16]) + mul_add(in[1], c[17]) + mul_add(in[2], c[18]) + mul_add(in[3], c[19]) + mul_add(in[4], c[20]) + mul_add(in[5], c[21]) + mul_add(in[6], c[22]);
            Is32vec2 s3 = mul_add(in[1], c[25]) + mul_add(in[2], c[26]) + mul_add(in[3], c[27]) + mul_add(in[4], c[28]) + mul_add(in[5], c[29]) + mul_add(in[6], c[30]) + mul_add(in[7], c[31]);
            a = unpack_low(s0, s1) + unpack_high(s0, s1);
            *first++ += a >> 14;
            u = mul_add(Is16vec4((a >> 14) >> 1), pan);
            *second++ += u >> 14;
            b = unpack_low(s2, s3) + unpack_high(s2, s3);
            *first++ += b >> 14;
            v = mul_add(Is16vec4((b >> 14) >> 1), pan);
            *second++ += v >> 14;
            ++in;
        }
        break;
    }
    _mm_empty();
}