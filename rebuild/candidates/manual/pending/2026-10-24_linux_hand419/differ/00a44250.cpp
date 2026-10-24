// GFGetInterpolatedColour(CRGBColour from, CRGBColour to, unsigned long t) -- blend two colours per channel as (from * (256 - t) + to * t) >> 8 with MMX
// Grade: DIFFER(124v117) -- retail_asm candidate: retail is an inline-asm MMX block over C locals (every operand a `movd` from a frame slot, the zero a memory operand, no pxor, an esp frame); the VC7.1 intrinsics give an aligned ebp frame, register movd and movq shuffles
// Leaf: ?GFGetInterpolatedColour@@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A44250, length 117 bytes, `__cdecl`, the CRGBColour
//   result through the hidden pointer (first stack argument), plain `ret`.
// Name: Ego_r.pdb / FableWin.pdb `?GFGetInterpolatedColour@@YA?AVCRGBColour@@V1@0K@Z` (name set).
// CRGBColour (PDB): a 4-byte union whose struct view is B +0, G +1, R +2, A +3; the result is built through the
//   `CRGBColour(r, g, b, a) { R = r; G = g; B = b; A = a; }` constructor (stores at +2, +1, +0, +3, as retail).
// Retail shape: `256 - t` and a zero dword are stored to frame slots and every MMX operand is a `movd` from memory, the
//   zero is a memory operand of both `punpcklbw` (no `pxor`): the signature of a retail inline-asm block over C locals
//   (inline-asm operands must be memory), not of the VC7.1 MMX intrinsics. This C++ body uses the intrinsics (the
//   declarations of the QFE toolset's <mmintrin.h>, which the RTM lane lacks, are restated below).
typedef union __declspec(intrin_type) __declspec(align(8)) __m64 {
    unsigned __int64 m64_u64;
    float            m64_f32[2];
    __int8           m64_i8[8];
    __int16          m64_i16[4];
    __int32          m64_i32[2];
    __int64          m64_i64;
    unsigned __int8  m64_u8[8];
    unsigned __int16 m64_u16[4];
    unsigned __int32 m64_u32[2];
} __m64;

extern "C" {
void  _m_empty(void);
__m64 _m_from_int(int i);
int   _m_to_int(__m64 m);
__m64 _m_packuswb(__m64 m1, __m64 m2);
__m64 _m_punpcklbw(__m64 m1, __m64 m2);
__m64 _m_punpcklwd(__m64 m1, __m64 m2);
__m64 _m_punpckldq(__m64 m1, __m64 m2);
__m64 _m_paddw(__m64 m1, __m64 m2);
__m64 _m_pmullw(__m64 m1, __m64 m2);
__m64 _m_psrlwi(__m64 m, int count);
}

class CRGBColour {
public:
    unsigned char B;                                 // +0x00
    unsigned char G;                                 // +0x01
    unsigned char R;                                 // +0x02
    unsigned char A;                                 // +0x03
    CRGBColour(unsigned char r, unsigned char g, unsigned char b, unsigned char a) { R = r; G = g; B = b; A = a; }
};

CRGBColour GFGetInterpolatedColour(CRGBColour from, CRGBColour to, unsigned long t)
{
    unsigned long result;
    unsigned long inverse = 256 - t;
    __m64 zero = _m_from_int(0);
    __m64 c0 = _m_punpcklbw(_m_from_int(*(int*)&from), zero);
    __m64 c1 = _m_punpcklbw(_m_from_int(*(int*)&to), zero);
    __m64 w0 = _m_from_int((int)inverse);
    __m64 w1 = _m_from_int((int)t);
    w0 = _m_punpcklwd(w0, w0);
    w1 = _m_punpcklwd(w1, w1);
    w0 = _m_punpckldq(w0, w0);
    w1 = _m_punpckldq(w1, w1);
    c0 = _m_pmullw(c0, w0);
    c1 = _m_pmullw(c1, w1);
    c0 = _m_paddw(c0, c1);
    c0 = _m_psrlwi(c0, 8);
    c0 = _m_packuswb(c0, c0);
    result = (unsigned long)_m_to_int(c0);
    _m_empty();
    const unsigned char* bytes = (const unsigned char*)&result;
    return CRGBColour(bytes[2], bytes[1], bytes[0], bytes[3]);
}
