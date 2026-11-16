// std::vector<CDefIndex>::at(size_type) const -- range-checked element reference (STLport vector::at with exceptions off)
// Leaf: ?at@?$vector@VCDefIndex@@V?$allocator@VCDefIndex@@@std@@@std@@QBEABVCDefIndex@@I@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0074CBC0, length 42 bytes.
// Callees: puts @ 0x00BFEAF2 with the literal "vector" @ 0x01268AE4, abort @ 0x00BFEAEC.
// Caller: CTCContainerRewardHero::GetRewardItems @ 0x0074E860 (call at 0x0074EA59 on the const CArray<CDefIndex>
//   CContainerRewardHeroDef::ObjectFamilies, Ego_r.pdb +0x28), which already binds this spelling through
//   stlport/fable_containers.h (FABLE_RETAIL_STLP_VECTOR_AT_OUTOFLINE). 0x0074D120 is the byte-identical copy in the
//   CTCCreatureGenerator TU (its own row, a separate address).
// Body: STLport 4.6 `_M_range_check(n)` (`if (n >= size()) __stl_throw_out_of_range("vector")`, without exceptions
//   `puts(msg); abort()`) followed by `*(begin() + n)` on 4-byte elements; `abort` is noreturn.
// Name (round 4, NF40): was the descriptive CRetailVector_dword_0074cbc0::at on a local three-pointer model; the name is the
//   STLport member of vector<CDefIndex> (descriptive-by-library: the PDBs are Dinkumware builds and hold no STLport at();
//   the body shape, the "vector" range-check literal and the typed caller prove it). Now the explicit instantiation of
//   stlport/fable_containers.h's inline at() (FABLE_RETAIL_STLP_VECTOR_AT_INLINE) over engine/CDefIndex.h.
#include "rebuild_abi.h"
#define FABLE_RETAIL_STLP_VECTOR_AT_INLINE
#include "stlport/fable_containers.h"
#include "engine/CDefIndex.h"

template const CDefIndex& std::vector<CDefIndex, std::allocator<CDefIndex> >::at(size_type) const;
