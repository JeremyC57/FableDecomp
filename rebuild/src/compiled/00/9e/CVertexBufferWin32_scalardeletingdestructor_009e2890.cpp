// CVertexBufferWin32::`scalar deleting destructor'(unsigned int flags) -- the inline destructor (this class's vtable, its cleanup call, the base destructor), then free this when bit 0 of flags is set
// Leaf: ??_GCVertexBufferWin32@@UAEPAXI@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x009E2890, 43 bytes, thiscall, `ret 4`, returns this.
// Identity (proven): it stores vtable 0x0129BA20 (RTTI .?AVCVertexBufferWin32@@, bases CVertexBufferBase : CBaseClassNonCopyable : CBaseClass); the vtable's slot 0.
// Callees: CVertexBufferWin32::Uninitialise() 0x00A63200, CBaseClassNonCopyable::~CBaseClassNonCopyable() 0x0099A300, operator delete 0x00BFE9BC.
// Shape (family K048, representative 0x00987F30): the compiler's scalar deleting destructor around a one-call inline
//   destructor; a `new` of the class in this file makes VC7.1 emit the vtable and the ??_G COMDAT.
// nf149 landing (NF149 coordinator follow-up): on engine/CVertexBufferWin32.h's retail branch instead of a local model, so the
//   ??0CVertexBufferWin32@@QAE@XZ and ??_7CVertexBufferWin32@@6B@ COMDATs this file emits are the header's (the local model emitted an empty
//   constructor and a one-slot vtable beside the header copies of 0x00A8E160 / 0x00B8DCD0).
#define FABLE_RETAIL_CVERTEXBUFFERWIN32
#include "engine/CVertexBufferWin32.h"

namespace {
// Fidelity: scaffolding, not retail: a `new` of the class makes VC7.1 emit its vtable and so this ??_G COMDAT (nf86 / nf88 pattern).
#pragma inline_depth(0)
CVertexBufferWin32* ReferenceVertexBufferWin32ScalarDeletingDtor()
{
    return new CVertexBufferWin32;
}
#pragma inline_depth()
}
