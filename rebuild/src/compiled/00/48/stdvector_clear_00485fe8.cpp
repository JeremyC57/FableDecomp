#pragma optimize("s",on)
// std::vector<CCharString>::clear() -- STLport 4.6.2 `clear() { erase(begin(), end()); }`: erase the whole range through the out-of-line erase
// Leaf: ?clear@?$vector@VCCharString@@V?$allocator@VCCharString@@@std@@@std@@QAEXXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00485FE8, 11 bytes, thiscall, plain `ret`.
// Retail bytes:  push dword ptr [ecx + 4] ; push dword ptr [ecx] ; call 0x43336a ; ret
// Identity (NF48, round 7): the callee 0x0043336A is the landed vector<CCharString>::erase(iterator, iterator) (4-byte elements,
//   CCharString::operator= 0x0099EFB0, the CCharString _Destroy 0x00414E00), called on the same object with (_M_start, _M_finish):
//   STLport's clear(). Ego_r.pdb / FableWin.pdb both name ?clear@?$vector@VCCharString@@V?$allocator@VCCharString@@@std@@@std@@QAEXXZ
//   (0x0043D810 / 0x018B6EA0 in those builds). Was the generated stand-in CClass_00485fe8::ApplyPair_00485fe8. Name
//   descriptive-by-library: the PDBs are Dinkumware builds, retail is STLport 4.6.2. The size-optimised (`push [mem]`) form is the
//   row's `#pragma optimize("s",on)`.
// Shape: the shared stlport/fable_containers.h clear() over the declaration-only erase(first, last)
//   (FABLE_RETAIL_STLP_VECTOR_ERASE_RANGE_OUTOFLINE), emitted by an explicit instantiation (CCharString only declared: clear() never
//   touches an element).
#include "fable_compiler.h"
#define FABLE_RETAIL_STLP_VECTOR_ERASE_RANGE_OUTOFLINE
#include "stlport/fable_containers.h"
class CCharString;

template void std::vector<CCharString>::clear();
