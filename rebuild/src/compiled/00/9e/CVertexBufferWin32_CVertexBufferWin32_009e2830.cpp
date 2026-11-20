// CVertexBufferWin32::CVertexBufferWin32(EVertexType, long, long, unsigned long, EVertexBufferPool) -- the empty buffer (CBaseClassNonCopyable's
// constructor, CVertexBufferBase's inline set-up, PVertexBuffer / Stride zero, the vtable 0x0129BA20), then Initialise(...) with the five arguments
// Leaf: ??0CVertexBufferWin32@@QAE@W4EVertexType@@JJKW4EVertexBufferPool@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009E2830, 95 bytes, thiscall, `ret 14h`: a compiler-emitted out-of-line copy of an inline constructor.
// Identity: FableWin.pdb ??0CVertexBufferWin32@@QAE@W4EVertexType@@JJKW4EVertexBufferPool@@@Z (0x030A3300; Ego_r.pdb has no out-of-line copy:
//   inline in release); the body stores .?AVCVertexBufferWin32@@'s vtable 0x0129BA20 and passes its five arguments on to
//   CVertexBufferWin32::Initialise 0x00A63150 (Ego_r.pdb + FableWin.pdb), whose parameter types the constructor's mangled name repeats.
// Port (NF149, q4552 binder / q6203 family): was a strong definition on local CBaseClassNonCopyable / CVertexCompressionParams /
//   CVertexBufferBase models and the stand-in class CVertexBufferWin32_Retail (leaf ??0CVertexBufferWin32_Retail@@QAE@JJJKJ@Z, binding
//   ?Initialise@CVertexBufferWin32_Retail@@QAE_NJJJKJ@Z and ??_7CVertexBufferWin32_Retail@@6B@); now the header's inline constructor (opt-in
//   FABLE_RETAIL_CVERTEXBUFFERWIN32_CTOR_INITIALISE), emitted here as its pick-any COMDAT (the nf86 pattern).
#define FABLE_RETAIL_CVERTEXBUFFERWIN32
#define FABLE_RETAIL_CVERTEXBUFFERWIN32_CTOR_INITIALISE
#include "engine/EVertexType.h"
#include "engine/EVertexBufferPool.h"
#include "engine/CVertexBufferWin32.h"

#pragma optimize("s", on)
namespace {
// Fidelity: scaffolding, not retail: the smallest out-of-line use of the header's inline constructor (a size-optimised `new`), so VC7.1
// emits ??0CVertexBufferWin32@@QAE@W4EVertexType@@JJKW4EVertexBufferPool@@@Z as a pick-any COMDAT.
CVertexBufferWin32* ReferenceVertexBufferWin32InitialiseCtor(EVertexType type, long count, long stride, unsigned long usage, EVertexBufferPool pool)
{
    return new CVertexBufferWin32(type, count, stride, usage, pool);
}
}
