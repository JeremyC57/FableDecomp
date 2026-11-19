// CMatrix4x4::AlignedPreScale(const C3DVector& scale) -- multiply row r of the 3x3 part by scale[r], the fourth column kept; SSE kernel when
//   the processor has SSE, x87 otherwise. Retail 0x00A9D480, 243 bytes, `ret 4`. Name: FableWin.pdb ?AlignedPreScale@CMatrix4x4@@QAEXABVC3DVector@@@Z
//   (484 debug bytes, the larger Aligned PreScale member; FableWin order ApplyBoneMorphScaling, AlignedPreScale, AlignedPreScaleTransposed).
// kept); SSE when the processor has it, x87 otherwise
// Retail-asm: 0x00A9D4CD `mov edx,[esp]` + 0x00A9D4D0 `movaps xmm3,[esp+10h]` + 0x00A9D4EA `shufps xmm4,xmm4,55h` (GF_SSE_MATRIX_PRESCALE; NOTES site 2)
// Leaf: ?AlignedPreScale@CMatrix4x4@@QAEXABVC3DVector@@@Z
// Shape: FableWin.pdb /Od 0x03382F80 + Ego_d.pdb locals: SSE block `C4DVector vect(scale, 0.0f)`, `float* dest` (assigned before
//   `C3DVector right(E14, E24, E34)`: retail stores dest at 0x00A9D4C2 before the column copies), the shared GF_SSE_MATRIX_PRESCALE,
//   then E14/E24/E34 restored from right; the x87 fallback writes each product through `CMatrix4x4& m = *this` and the PDB element
//   accessor `m(r, c) *= scale.X` (FableWin calls ??RCMatrix4x4@@QAEAAMHH@Z nine times; header opt-in
//   FABLE_RETAIL_CMATRIX4X4_ELEMENT_ACCESS). That accessor form gives retail's fld/fmul operand order for all nine products (the
//   direct `E11 *= scale.X` spelling swaps four of them).
// Text: the shared body in engine/CMatrix4x4AlignedSSE.h (per-member out-of-line opt-in FABLE_RETAIL_CMATRIX4X4_ALIGNEDPRESCALE_OUTOFLINE,
//   hand3c batch 5). It is the same text that ApplyBoneMorphScaling 0x00A9D750 expands under FABLE_RETAIL_CMATRIX4X4_ALIGNEDPRESCALE_BODIES.
// Flags: none
#define FABLE_RETAIL_CMATRIX4X4
#define FABLE_RETAIL_CMATRIX4X4_ELEMENT_ACCESS
#define FABLE_RETAIL_CMATRIX4X4_ALIGNEDPRESCALE_OUTOFLINE
#include "engine/CMatrix4x4.h"
#include "engine/CMatrix4x4AlignedSSE.h"
