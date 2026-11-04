// std::vector::resize(size_type new_size, const value_type& x) -- (vector<CArray<unsigned short> >) STLport two-argument resize: erase the surplus tail through the out-of-line range erase, or insert new_size - size() copies of x at the end through the out-of-line _M_fill_insert
// Leaf: ?resize@?$vector@V?$CArray@G@@V?$allocator@V?$CArray@G@@@std@@@std@@QAEXIABV?$CArray@G@@@Z
// Plan phase 3 (2026-11-04), CArray wave: the local CArray (an empty public std::vector<T>) and its STLport stand-ins replaced by the shared engine/CArray.h (Lionhead's array: a public STLport 4.6 vector<T>, 12 bytes, no fields of its own). STLport algorithm / construct helpers and placement new come from the header's model (<new>).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00F3FAF0, 98 bytes (+ int3 padding to 112), thiscall, `ret 8`.
// Identity (proven): vector<CArray<unsigned short> >::resize(n, x); evidence: its erase callee 0x00F3F7C0 is the landed vector<CArray<unsigned short> >::erase(first, last); its caller is the one-argument resize 0x00F3FB60; the PDB name sets hold ?resize@?$vector@V?$CArray@G@@....
// Label: the manifest's "resize" is propagated across byte-identical bodies (family H027: seven rows that differ only in
//   the element size (here 12 bytes: the reciprocal division of size() and `begin() + n`) and the call displacements;
//   retail has no identical-code folding, so each address is one instantiation).
// Callees: vector::erase(iterator first, iterator last) @ 0x00F3F7C0 (result dropped), vector::_M_fill_insert(iterator pos,
//   size_type n, const value_type& x) @ 0x00F3F960 (both thiscall, out of line).
// Shape: STLport 4.6 `if (__new_size < size()) erase(begin() + __new_size, end()); else insert(end(), __new_size - size(),
//   __x);` with `insert(pos, n, x)` forwarding to _M_fill_insert (the landed resize 0x00BBDD40 model); size() is recomputed
//   on the insert branch.
#include "engine/CArray.h"
typedef CArray<unsigned short> TElem;
template void std::vector<TElem>::resize(unsigned int, const TElem&);