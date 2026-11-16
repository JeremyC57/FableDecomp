// std::vector<unsigned long>::resize(size_type, const unsigned long&) -- STLport resize: erase the surplus when shrinking, else
//   fill-insert copies of the value at the end
// Leaf: ?resize@?$vector@KV?$allocator@K@std@@@std@@QAEXIABK@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00464931 (size-optimised TU); the range erase 0x0046496F and _M_fill_insert 0x004649A3 out of line.
// Name: descriptive-by-library (STLport 4.6 const& form; FableWin.pdb's Dinkumware ?resize@..@QAEXIK@Z takes the value): every
//   landed binder passes unsigned long (CArray<unsigned long>); 0x004648A1 / 0x00B22B40 / 0x009CE5E0 bind this symbol (followup2 of
//   landing gn). Emitted from the shared stlport/fable_containers.h (NF33 2026-11-16; was the stand-in CArrayElem::resize).
#pragma optimize("s", on)
#define FABLE_RETAIL_STLP_VECTOR_ERASE_RANGE_OUTOFLINE
#include "stlport/fable_containers.h"
template void std::vector<unsigned long, std::allocator<unsigned long> >::resize(std::vector<unsigned long, std::allocator<unsigned long> >::size_type, const unsigned long&);
