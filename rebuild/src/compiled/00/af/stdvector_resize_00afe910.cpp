// std::vector::resize(size_type new_size, const value_type& x) -- (vector<CArray<long> >) STLport two-argument resize: erase the surplus tail through the out-of-line range erase, or insert new_size - size() copies of x at the end through the out-of-line _M_fill_insert
// Leaf: ?resize@?$vector@V?$CArray@J@@V?$allocator@V?$CArray@J@@@std@@@std@@QAEXIABV?$CArray@J@@@Z
// Plan phase 3 (2026-11-04), CArray wave: the local CArray (an empty public std::vector<T>) and its STLport stand-ins replaced by the shared engine/CArray.h (Lionhead's array: a public STLport 4.6 vector<T>, 12 bytes, no fields of its own). STLport algorithm / construct helpers and placement new come from the header's model (<new>).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00AFE910, 98 bytes (+ int3 padding to 112), thiscall, `ret 8`.
// Identity (proven): vector<CArray<long> >::resize(n, x); evidence: its erase callee 0x00AF7910 is the landed vector<CArray<long> >::erase(first, last); its callers are three sites in NParticleEngine::CPSCRenderSprite 0x00AFCC30; the PDB name sets hold ?resize@?$vector@V?$CArray@J@@....
// Label: the manifest's "resize" is propagated across byte-identical bodies (family H027: seven rows that differ only in
//   the element size (here 12 bytes: the reciprocal division of size() and `begin() + n`) and the call displacements;
//   retail has no identical-code folding, so each address is one instantiation).
// Callees: vector::erase(iterator first, iterator last) @ 0x00AF7910 (result dropped), vector::_M_fill_insert(iterator pos,
//   size_type n, const value_type& x) @ 0x00AFE7C0 (both thiscall, out of line).
// Shape: STLport 4.6 `if (__new_size < size()) erase(begin() + __new_size, end()); else insert(end(), __new_size - size(),
//   __x);` with `insert(pos, n, x)` forwarding to _M_fill_insert (the landed resize 0x00BBDD40 model); size() is recomputed
//   on the insert branch.
#include "engine/CArray.h"
typedef CArray<long> TElem;
template void std::vector<TElem>::resize(unsigned int, const TElem&);