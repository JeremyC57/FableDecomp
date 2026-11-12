// CArray<std::pair<unsigned long, C3DVector> >::push_back(const std::pair<unsigned long, C3DVector>& value) -- Lionhead's array
// push_back: the STLport vector push_back expanded inline (construct in spare capacity, else _M_insert_overflow 0x0076ADA0).
// Leaf: ?push_back@?$CArray@U?$pair@KVC3DVector@@@std@@@@QAEXABU?$pair@KVC3DVector@@@std@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x0076B120.
// Name (agentNF32, 2026-11-12; was ?push_back@?$vector@U?$pair@KVC3DVector@@@std@@V?$allocator@..@std@@QAEXABU?$pair@KVC3DVector@@@2@@Z):
//   Ego_r.pdb 0x0077A190 and FableWin.pdb 0x025F4840 list only the CArray<pair<unsigned long, C3DVector> >::push_back instance (no
//   out-of-line vector<pair<..> >::push_back in either PDB); the landed 0x008548B0 binds this address under this symbol.
//   Now on the shared engine/CArray.h (FABLE_RETAIL_CARRAY_PUSHBACK) over stlport/fable_containers.h.
#include "engine/C3DVector.h"
#define FABLE_RETAIL_CARRAY_PUSHBACK
#include "engine/CArray.h"

template void CArray<std::pair<unsigned long, C3DVector> >::push_back(const std::pair<unsigned long, C3DVector>&);
