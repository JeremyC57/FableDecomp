// CMatrix4x4::AlignedPreScaleTransposed(const C3DVector& scale) -- multiply column c of the 3x3 part by scale[c] (rows 0..2 times (X, Y, Z, 0) with the fourth column E14/E24/E34 saved around the product); the SSE kernel when the processor has SSE, x87 otherwise
// Leaf: ?AlignedPreScaleTransposed@CMatrix4x4@@QAEXABVC3DVector@@@Z
// Flags: none
// Retail-asm: 0x00A9D5C9 `mov edx,[esp]` + 0x00A9D5CC `movaps xmm3,[esp+10h]` + 0x00A9D5DC `mulps xmm0,xmm3` (the GF_SSE_MATRIX_PRESCALE_TRANSPOSED block of this function; C++/intrinsic table in chunk 748 NOTES, site 1)
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A9D580, 221 bytes, `ret 4`, `and esp,-16` frame (the aligned scale local at [esp+10h]).
// Name: FableWin.pdb ?AlignedPreScaleTransposed@CMatrix4x4@@QAEXABVC3DVector@@@Z (466 debug bytes, the smaller of the two
//   Aligned*PreScale members; FableWin order ApplyBoneMorphScaling, AlignedPreScale, AlignedPreScaleTransposed = retail's
//   0x00A9D750 neighbours 0x00A9D480 / 0x00A9D580; this body scales columns, the transposed form). The manifest's
//   "CMatrix3x4::PostScale" is a propagated label (NAME_FIXES.tsv).
// SSE: the isolated kernel GF_SSE_MATRIX_PRESCALE_TRANSPOSED (engine/CMatrix4x4AlignedSSE.h, grade retail_asm): retail homes
//   `this` in a frame word and re-reads it into edx inside the block (`mov [esp],ecx ; ... ; mov edx,[esp]`), the fourth
//   column goes through dword frame slots. Intrinsic spellings (chunk 748 NOTES table, 12 graded) never re-read the pointer.
// Text (hand3c batch 5): the shared body in engine/CMatrix4x4AlignedSSE.h under the per-member out-of-line opt-in
//   FABLE_RETAIL_CMATRIX4X4_ALIGNEDPRESCALETRANSPOSED_OUTOFLINE. That is the Ego_d / FableWin 0x033831E0 form: SSE block vect / dest / right,
//   and the x87 block through `CMatrix4x4& m = *this` and the element accessors, which give retail's operand order with no row rewritten as a
//   loop. ApplyBoneMorphScaling 0x00A9D750 expands the same text. The global keeps this row's landed spelling GEnableSIMD through
//   FABLE_RETAIL_CMATRIX4X4_SIMD_ORIGINAL_FLAG.
// Global: GEnableSIMD 0x013D2880.
#define FABLE_RETAIL_CMATRIX4X4
#define FABLE_RETAIL_CMATRIX4X4_ELEMENT_ACCESS
#define FABLE_RETAIL_CMATRIX4X4_SIMD_ORIGINAL_FLAG
#define FABLE_RETAIL_CMATRIX4X4_ALIGNEDPRESCALETRANSPOSED_OUTOFLINE
#include "engine/CMatrix4x4.h"
#include "engine/CMatrix4x4AlignedSSE.h"
