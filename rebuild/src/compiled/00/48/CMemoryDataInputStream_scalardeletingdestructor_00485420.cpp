// CMemoryDataInputStream::`scalar deleting destructor'(unsigned int flags) -- the implicit destructor (a direct call to ~CDataStream
// 0x00993B10: the derived destructors are compiler-generated, engine/CDataInputStream.h), then free the object when bit 0 is set
// Leaf: ??_GCMemoryDataInputStream@@UAEPAXI@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00485420, 30 bytes, thiscall, `ret 4`; vftable 0x0123675C slot 0 (rebuild/manifest/rtti-vtables.tsv).
//   (The 72-byte manifest capture over-captures: four unrelated accessor thunks follow after int3 padding; graded on the first 30.)
// Name (NF181, 2026-11-20, q4251): Ego_r.pdb 0x00491B30 ??_GCMemoryDataInputStream@@UAEPAXI@Z + the vftable slot; was the free __fastcall
//   stand-in on a row-local model calling ~CMemoryDataInputStream (the stand-in name of ~CDataStream 0x00993B10).
// Fidelity: scaffolding, not retail (nf86 / nf88 pattern): the deleting destructor is a compiler-emitted COMDAT that VC7.1 emits with the
//   vtable, i.e. wherever the header's inline default constructor (engine/CMemoryDataInputStream.h) is expanded. The anonymous-namespace
//   helper below expands it (placement new), so this object emits ??_7CMemoryDataInputStream@@6B@ and the ??_G pick-any. Internal to this
//   TU; never called.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CDATAINPUTSTREAM
#define FABLE_RETAIL_CMEMORYDATAINPUTSTREAM
#include <new>
#include "fable_string.h"
#include "engine/CMemoryDataInputStream.h"

namespace {
void ReferenceMemoryDataInputStreamVtable(void* where)
{
    new (where) CMemoryDataInputStream();
}
}
