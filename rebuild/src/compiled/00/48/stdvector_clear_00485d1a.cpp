// std::vector<long>::clear() -- STLport `erase(begin(), end())`, the range erase called out of line
// Leaf: ?clear@?$vector@JV?$allocator@J@std@@@std@@QAEXXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00485D1A, 11 bytes: push [ecx+4] ; push [ecx] ; call 0x00440590 ; ret.
// Name fix (agentNF94, q11238; descriptive-by-library: STLport instance, in neither PDB): the landed caller 0x00771DE0 binds this address as
//   ?clear@?$vector@JV?$allocator@J@std@@@std@@QAEXXZ (reloc_bindings), and the body is STLport's clear() = erase(_M_start, _M_finish) with
//   vector<long>::erase(first, last) 0x00440590 called out of line. The old row was the generated placeholder
//   CClass_00485d1a::ApplyPair_00485d1a calling a stand-in Apply; it is now the explicit instantiation over stlport/fable_containers.h
//   (FABLE_RETAIL_STLP_VECTOR_ERASE_RANGE_OUTOFLINE: erase(first, last) declared only, as this TU calls it).
#pragma optimize("s",on)
#define FABLE_RETAIL_STLP_VECTOR_ERASE_RANGE_OUTOFLINE
#include "stlport/fable_containers.h"
template void std::vector<long>::clear();
