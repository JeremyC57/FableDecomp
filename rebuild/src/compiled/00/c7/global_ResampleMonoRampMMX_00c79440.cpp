// ResampleMonoRampMMX(const short* source, unsigned long phase, unsigned long increment, unsigned long count, long* output, short startGain, short gainStep) -- MMX 8-tap polyphase resampler with a linear gain ramp: as ResampleMonoMMX, but each output quad is packed to 16 bits (>>15, saturated), scaled by the per-lane gain (pmulhw), widened back (punpck with zero, >>14) and added into `output`, the gain advancing by 4*gainStep per quad (paddsw)
// Leaf: ?ResampleMonoRampMMX@@YAXPBFKKKPAJFF@Z
// Flags: /GS
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C79440, 2948 bytes incl. the 4-entry switch table at 0x00C79FB4 (+ int3 padding to 0x00C79FD0), `ret` (__cdecl).
// Callers: MixVoiceFadeIn 0x00C70240 (0, 0x100), MixVoiceFadeOut 0x00C702E0.
// Library: libpk software mixer (no PDB/RTTI; names are the landed callers' spellings, 0x00C70240/0x00C702E0/0x00C70390).
//   Not inline asm: the body is the Intel ivec.h class library (M64/Is32vec2/Is16vec4 and the free mul_add/unpack/
//   pack_sat/mul_high/sat_add helpers whose out-of-line copies landed at 0x00C78590..0x00C78970 in this TU), so every
//   pmaddwd/paddd/punpck is a compiler-scheduled MMX intrinsic.
// Shape: `switch (increment)` selects which of the four 8-tap phase rows start one quad later (the window slides one
//   __m64 per output pair); each case is a counted loop of four 7-tap pmaddwd sums s0..s3 folded pairwise
//   (unpack_low+unpack_high) and shifted.  The window quads are deliberately NOT named: writing `mul_add(in[k], c[n])`
//   lets VC7.1's CSE temporaries take retail's spill slots; named locals pack the 8-byte frame slots differently.
// Built under /GS: the __m64 locals give an aligned frame (`and esp,-16` here, for the align(16) gain/step) with the cookie (0x0139C8A8) checked by
//   __security_check_cookie 0x00BFE9F9 before the return.
// Gains: startGain/gainStep are `short` (retail forms the lane gains with `lea edx,[ecx+eax]`/`lea ecx,[ecx+eax*2]` in
//   32-bit registers and stores the low words; `long` parameters give a different lea operand order).  gain/step are 16-byte aligned stack vectors built lane by lane
//   by the ivec short constructor (retail stores four words to [esp+..] and reloads the quad with movq).
#include <mmintrin.h>

// Intel C++ class library (ivec.h) model: VC7.1's RTM include directory has no ivec.h, so the classes the libpk mixer
// used are restated here with the header's own member spellings (the same model as the landed 0x00C78590..0x00C78970).

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

void ResampleMonoRampMMX(const short* source, unsigned long phase, unsigned long increment, unsigned long count, long* output, short startGain, short gainStep)
{
    __declspec(align(16)) Is16vec4 gain((short)(startGain + 3 * gainStep), (short)(startGain + 2 * gainStep), (short)(startGain + gainStep), (short)startGain);
    __declspec(align(16)) Is16vec4 step((short)(4 * gainStep), (short)(4 * gainStep), (short)(4 * gainStep), (short)(4 * gainStep));
    const Is16vec4* in = (const Is16vec4*)(source - 32);
    const Is16vec4* c = (const Is16vec4*)phase;
    Is32vec2* out = (Is32vec2*)output;
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
            *out++ += Is32vec2(unpack_low(Is16vec4(_mm_setzero_si64()), r)) >> 14;
            *out++ += Is32vec2(unpack_high(Is16vec4(_mm_setzero_si64()), r)) >> 14;
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
            *out++ += Is32vec2(unpack_low(Is16vec4(_mm_setzero_si64()), r)) >> 14;
            *out++ += Is32vec2(unpack_high(Is16vec4(_mm_setzero_si64()), r)) >> 14;
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
            *out++ += Is32vec2(unpack_low(Is16vec4(_mm_setzero_si64()), r)) >> 14;
            *out++ += Is32vec2(unpack_high(Is16vec4(_mm_setzero_si64()), r)) >> 14;
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
            *out++ += Is32vec2(unpack_low(Is16vec4(_mm_setzero_si64()), r)) >> 14;
            *out++ += Is32vec2(unpack_high(Is16vec4(_mm_setzero_si64()), r)) >> 14;
            ++in;
        }
        break;
    }
    _mm_empty();
}