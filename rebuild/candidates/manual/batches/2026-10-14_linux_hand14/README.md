# Batch `2026-10-14_linux_hand14` — hand-authored CBankFile .. CBaseIntelligentPointer functions: 40 bodies (37 landed)

Delegated hand-authoring pass (compiler in the loop, Wine lane) over the fifteenth chunk of
class-labelled open functions of 17–96 bytes: `CBankFile` entry getters and counters (packed
array 0-based, 1-based 12-byte runtime records, the tail `jmp` with the argument slot rewritten
to `i-1`), `CBankFileAsync` entry construction and update bodies, the `CBase` sub-component /
mesh-height / allocator bodies, the `CBaseObject` copy constructor (vtable 0x0129A7D8, propagated
label corrected), a `__fastcall` entry-allocator size lookup, a tree `Contains`, a size-optimised
`FreeBuffer` (`#pragma optimize("s")` TU) and a `__stdcall` COM record release through
`CoTaskMemFree`. The PDB `CBankFile.h` disagrees with the retail layout (retail `CArray` is 12
bytes, so PDB offsets sit 0x10 high from +0x38) and the local models are named `*_Retail` /
`*_Calls` to avoid the shared-header policy.

Lessons recorded by the pass: `return ulong != 0` as `bool` is `neg;sbb;neg` out of line and
`setne` inlined into a loop; a dead `GetEntryDataSize(index)` call survives as an opaque call
(write the unused local); a by-value string return that re-reads `m_pFirst` after the emptiness
test reaches the element through the iterator object; `lea ecx,[eax+100h]` before `GetPItem` is an
inline accessor on the holder; the `CCriticalSectionLock{cs*, bool}` try-lock pattern on an 8-byte
frame; a vector loop whose end is re-read after each virtual call caches nothing; Dinkumware
`erase(it)` inlined is a `memmove` plus `--last`. Three bodies differ: `CBase::Die` (vector load
order), `PopLoadErrorQueue` (the `_Find` tag's address pushed from a live argument slot) and
`SetBankUpdateStatus` (the DDK re-stores the forwarded argument before the sibling jump).

All landed bodies graded `MATCH` / `RELOCATION_MATCH` + `PASS` on 2026-10-14.
