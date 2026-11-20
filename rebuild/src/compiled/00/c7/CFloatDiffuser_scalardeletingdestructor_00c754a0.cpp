// CFloatDiffuser::`scalar deleting destructor'(unsigned int flags) -- compiler-generated: the destructor (0x00C75440), then the CRT's operator delete (0x00BFE9BC) when bit 0 of the flags is set; returns `this`
// Leaf: ??_GCFloatDiffuser@@UAEPAXI@Z
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C754A0, 28 bytes (+ int3 padding), thiscall `ret 4`.
// Name (NF109, q11607 family; was the descriptive ??_GCStereoDelayLine over a local model): GameCODA 2.10 CodeView class CFloatDiffuser.
//   Two sources: (1) it is the only slot of the vtable 0x012BF078 that ??0CFloatDiffuser 0x00C75410 / ??1CFloatDiffuser 0x00C75440 store
//   (both named in their rows: the CodeView m_diffuser at engine +0x25c); (2) it calls ??1CFloatDiffuser 0x00C75440 and the CRT operator
//   delete, the compiler-generated `??_G` shape (the CODAApi.lib release ??_G bodies of this size are folded across 24 classes, so byte
//   identity alone does not name it).
// Shape: the compiler-generated `??_G`; the vtable (and so `??_G`) is emitted by the out-of-line helper below through the implicit
//   inline ctor, the destructor stays an out-of-line call. NewFloatDiffuser is a file-local helper, not retail code (descriptive name).
#define FABLE_RETAIL_PASS22_FLOATREVERB_CREATE
#include "engine/CFloatDiffuser.h"

CFloatDiffuser* NewFloatDiffuser()
{
    return new CFloatDiffuser;
}
