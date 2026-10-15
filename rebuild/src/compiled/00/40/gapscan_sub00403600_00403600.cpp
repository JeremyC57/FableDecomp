// DeleteIfSet(Obj* p) -- delete a polymorphic object through its vtable if the pointer is non-null
// Retail: Fable.exe (TLC, VC7.1) @ 0x00403600, 11 bytes. Callees: vtable slot 0 of *p (deleting destructor, flags = 1).
//
// The helper every `delete owner->member` collapses to: when the pointer is set, call the object's
// deleting destructor (vtable slot 0) with flag 1 so it frees its own storage. A null pointer is a no-op.
// This body is shared by many owners, so no single class name is the right one for it.
//
// Matching: pointer in ecx; `push 1; call [eax]` through slot 0; the null test is a `test ecx,ecx; je`.
struct Obj { virtual void Destroy(int flags); };   // slot 0: deleting destructor
extern "C" void __fastcall DeleteIfSet(Obj* p) { if (p) p->Destroy(1); }
