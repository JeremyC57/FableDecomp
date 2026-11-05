// GFGetInterpolatedColour(CRGBColour* out, CRGBColour a, CRGBColour b, unsigned long weight) -- blend two byte colours with MMX: out = (a * (256 - weight) + b * weight) >> 8 per channel (the result returned through the hidden pointer)
// Leaf: ?GFGetInterpolatedColour@@YA?AVCRGBColour@@V1@0K@Z
// Retail-asm: 0x00A4426A..0x00A442A8 `movd mm0,[esp+18h] ; movd mm1,[esp+1Ch] ; movd mm2,[esp+4] ; movd mm3,[esp+20h] ; punpcklbw mm0,[esp+8] ; punpcklbw mm1,[esp+8] ; punpcklwd mm2,mm2 ; punpcklwd mm3,mm3 ; punpckldq mm2,mm2 ; punpckldq mm3,mm3 ; pmullw mm0,mm2 ; pmullw mm1,mm3 ; paddw mm0,mm1 ; psrlw mm0,8 ; packuswb mm0,mm0 ; movd [esp],mm0 ; emms` -- a hand MMX blend over named C locals and the by-value arguments (17 instructions, no branch: rasm1 family (f)): every operand a memory local, no register copies, `emms` inside the block; mmintrin intrinsics give register shuffles instead (rasm1 probe). The weight arithmetic, the zero local and the channel constructor built in the return slot is C++.
// Helper (2026-11-05 asm migration): the retail inline-asm block is the shared engine/GFGetInterpolatedColourMMX.h (byte-matching path + FABLE_PORTABLE twin); the row has
//   no asm of its own. C++/intrinsic attempts: the batch ATTEMPTS.tsv, family MMX_COLOUR.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A44250, 117 bytes (+11 int3), __cdecl, the CRGBColour result through the hidden
//   pointer ([esp+14h]); the two colours arrive by value as dwords ([esp+18h], [esp+1Ch]) and the weight at [esp+20h]. No callees.
//   Ego_r.pdb: ?GFGetInterpolatedColour@@YA?AVCRGBColour@@V1@0K@Z. CRGBColour byte order B, G, R, A; the class has a user
//   constructor (hence the hidden-pointer return and the four byte stores through eax).
#include "engine/GFGetInterpolatedColourMMX.h"
class CRGBColour {
public:
    unsigned char B, G, R, A;
    CRGBColour() { }
    // the channel constructor assigns R, G, B, A in that order (the four byte stores through the hidden pointer, +2 +1 +0 +3)
    CRGBColour(unsigned char r, unsigned char g, unsigned char b, unsigned char a) { R = r; G = g; B = b; A = a; }
};
CRGBColour GFGetInterpolatedColour(CRGBColour a, CRGBColour b, unsigned long weight)
{
    CRGBColour    blend;
    unsigned long inverse;
    unsigned long zero;
    zero = 0;
    inverse = 256 - weight;
    GF_MMX_INTERPOLATE_COLOUR(a, b, inverse, weight, zero, blend);
    return CRGBColour(blend.R, blend.G, blend.B, blend.A);
}