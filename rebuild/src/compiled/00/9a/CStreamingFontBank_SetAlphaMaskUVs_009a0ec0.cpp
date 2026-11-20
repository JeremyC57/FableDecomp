// CStreamingFontBank::SetAlphaMaskUVs(const C2DVector& uv0, const C2DVector& uv1) -- stores the two alpha-mask texture coordinates the streaming font renderer uses
// Leaf: ?SetAlphaMaskUVs@CStreamingFontBank@@SIXABVC2DVector@@0@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x009A0EC0, 33 bytes (+ int3 padding), plain `ret`. Catalog row "_global
// SetAlphaMaskUVs"; the Ego_r PDB name is `?SetAlphaMaskUVs@CStreamingFontBank@@SIXABVC2DVector@@0@Z`, a static
// `__fastcall` member (uv0 in ecx, uv1 in edx).
// Globals: the two UV pairs @ 0x013CA2F8 and 0x013CA300: the static members CStreamingFontBank::AlphaMaskUVMul / AlphaMaskUVAdd
//   (Ego_r.pdb `dynamic initializer for 'CStreamingFontBank::AlphaMaskUVMul'` / `...AlphaMaskUVAdd'`, Mul first; the landed Render
//   0x009A25D0 multiplies by 0x013CA2F8 and adds 0x013CA300 under these spellings).
// Shape: two whole-object assignments of the 8-byte C2DVector POD (two floats, implicit copy) are dword moves
// through eax/ecx; a user operator= on the float members would go through the x87.
// NF192 (q6313): ported onto engine/CStreamingFontBank.h (_GLYPHLOOKUP declares the two statics, _TEXT_RENDER the setter) and
//   engine/C2DVector.h; was row-local C2DVector / CStreamingFontBank models binding ?g_StreamingFontAlphaMaskUV0@@3VC2DVector@@A /
//   ?g_StreamingFontAlphaMaskUV1@@3VC2DVector@@A.
#define FABLE_RETAIL_CFONTBANK
#define FABLE_RETAIL_CSTREAMINGFONTBANK
#define FABLE_RETAIL_CSTREAMINGFONTBANK_GLYPHLOOKUP
#define FABLE_RETAIL_CENGINEPRIMITIVEMANAGERTEXT_RENDER
#include "engine/C2DVector.h"
#include "engine/CStreamingFontBank.h"

void FABLE_FASTCALL CStreamingFontBank::SetAlphaMaskUVs(const C2DVector& uv0, const C2DVector& uv1)
{
    AlphaMaskUVMul = uv0;
    AlphaMaskUVAdd = uv1;
}
