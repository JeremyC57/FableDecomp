// std::vector<CScriptThing>::clear() -- STLport 4.6.2 `clear() { erase(begin(), end()); }`: erase the whole range through the out-of-line erase
// Leaf: ?clear@?$vector@VCScriptThing@@V?$allocator@VCScriptThing@@@std@@@std@@QAEXXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008AD5C0, 13 bytes, thiscall, plain `ret`.
// Retail bytes:  mov eax,[ecx+4] ; mov edx,[ecx] ; push eax ; push edx ; call 0x008AC9D0 ; ret
// Identity (NF48, round 7): the callee 0x008AC9D0 is the landed vector<CScriptThing>::erase(iterator, iterator) (12-byte elements, the
//   CScriptThing __copy 0x008AC2A0 and virtual destructor), called on the same object with (_M_start, _M_finish): STLport's clear().
//   Ego_r.pdb / FableWin.pdb both name ?clear@?$vector@VCScriptThing@@V?$allocator@VCScriptThing@@@std@@@std@@QAEXXZ (0x008EB410 /
//   0x02AC8660 in those builds). Was the descriptive stand-in CPair_Retail::CallWithPair_008ac9d0 (manifest label
//   `SetStaticCast<NUISystem::CComponent>` propagated). Name descriptive-by-library: the PDBs are Dinkumware builds, retail is STLport 4.6.2.
// Shape: the shared stlport/fable_containers.h clear() over the declaration-only erase(first, last)
//   (FABLE_RETAIL_STLP_VECTOR_ERASE_RANGE_OUTOFLINE), emitted by an explicit instantiation.
#include "fable_compiler.h"
#define FABLE_RETAIL_STLP_VECTOR_ERASE_RANGE_OUTOFLINE
#include "stlport/fable_containers.h"
class CCPPointerInfo;
#include "engine/CCountedPointer.h"
#define FABLE_RETAIL_NSCRIPT
#include "engine/CScriptThing.h"

template void std::vector<CScriptThing>::clear();
