// std::vector<CPrizeScoreDef>::resize(unsigned int newSize) -- STLport's one-argument vector resize: grow or shrink to newSize, filling with a default-constructed CPrizeScoreDef
// Leaf: ?resize@?$vector@VCPrizeScoreDef@@V?$allocator@VCPrizeScoreDef@@@std@@@std@@QAEXI@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004EDA32, length 58 bytes.
// Identity (proven): (1) the landed caller CPersistContext::TransferVectorOfSubComponents<CPrizeScoreDef> binds this address under
//   ?resize@?$vector@VCPrizeScoreDef@@V?$allocator@VCPrizeScoreDef@@@std@@@std@@QAEXI@Z (reloc_bindings.tsv); (2) STLport 4.6 stl/_vector.h
//   `void resize(size_type __new_size) { resize(__new_size, _Tp()); }`, matching the body (a default CPrizeScoreDef temporary,
//   vtable 0x012437D4, then the two-argument resize).
// Callees: CSubDefClassBase::CSubDefClassBase @ 0x0044C0C0 (inline CPrizeScoreDef default ctor), std::vector<CPrizeScoreDef>::resize(size_type, const CPrizeScoreDef&) @ 0x004EDA6C (kept out of line in this TU:
//   FABLE_RETAIL_STLP_VECTOR_RESIZE_OUTOFLINE), CResource::~CResource @ 0x009FC550.
// Shape: a frame-pointer size TU (ebp frame, `leave`): `#pragma optimize("y", off)` + `("s", on)`; the temporary's inline
//   destructor chain leaves the CDefPointeeBase store and the root destructor call; `ret 4`.
#pragma optimize("y", off)
#pragma optimize("s", on)
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_STLP_VECTOR_RESIZE_OUTOFLINE
#include "fable_string.h"
#include "stlport/fable_containers.h"
#include "engine/CPrizeScoreDef.h"

template void std::vector<CPrizeScoreDef>::resize(std::vector<CPrizeScoreDef>::size_type newSize);
