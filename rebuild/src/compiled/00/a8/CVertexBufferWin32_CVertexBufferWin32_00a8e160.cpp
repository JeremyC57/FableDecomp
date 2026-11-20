// CVertexBufferWin32::CVertexBufferWin32() -- the empty buffer: CBaseClassNonCopyable's constructor, Length / Type zero and the identity
// compression params (CVertexBufferBase's inline constructor), PVertexBuffer / Stride zero, the CVertexBufferWin32 vtable 0x0129BA20
// Leaf: ??0CVertexBufferWin32@@QAE@XZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A8E160, 61 bytes, thiscall: a compiler-emitted out-of-line copy of the header's inline constructor.
// Identity: FableWin.pdb ??0CVertexBufferWin32@@QAE@XZ (0x02CBBB60, next to ??0CVertexBufferBase@@IAE@XZ); the stored vtable 0x0129BA20 is
//   .?AVCVertexBufferWin32@@; Ego_r.pdb has no out-of-line copy (inline in release, engine/CVertexBufferWin32.h).
// Port (NF149, q6203): was a strong definition on local CBaseClassNonCopyable / CVertexCompressionParams / CVertexBufferBase models and the
//   stand-in class CVertexBufferWin32_Retail (leaf ??0CVertexBufferWin32_Retail@@QAE@XZ, vtable ??_7CVertexBufferWin32_Retail@@6B@); now the
//   header's inline constructor (FABLE_RETAIL_CVERTEXBUFFERWIN32), emitted here as its pick-any COMDAT (the nf86 pattern), so one definition
//   serves every includer.
#define FABLE_RETAIL_CVERTEXBUFFERWIN32
#include "engine/CVertexBufferWin32.h"

#pragma optimize("s", on)
namespace {
// Fidelity: scaffolding, not retail: the smallest out-of-line use of the header's inline constructor (a size-optimised `new`), so VC7.1
// emits ??0CVertexBufferWin32@@QAE@XZ as a pick-any COMDAT.
CVertexBufferWin32* ReferenceVertexBufferWin32Ctor()
{
    return new CVertexBufferWin32;
}
}
