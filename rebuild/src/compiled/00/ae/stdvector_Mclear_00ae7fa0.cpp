// std::vector::_M_clear() -- (vector<C3DGroup2 >) _M_clear (the old block release of _M_insert_overflow / reserve / operator=): destroy every 0x10-byte element (its implicit destructor frees the inner VertexBlends array at +0x4 when it has a block), then free the vector's own block
// Leaf: ?_M_clear@?$vector@VC3DGroup2@@V?$allocator@
// Plan phase 3 (2026-11-04), CArray wave: the local CArray (an empty public std::vector<T>) and its STLport stand-ins replaced by the shared engine/CArray.h (Lionhead's array: a public STLport 4.6 vector<T>, 12 bytes, no fields of its own). STLport algorithm / construct helpers and placement new come from the header's model (<new>).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00AE7FA0, 58 bytes (+ int3 padding), thiscall, plain `ret`.
// Identity: vector<C3DGroup2 >::_M_clear -- proven; evidence: its callers are the vector<C3DGroup2> range insert 0x00AE7FE0, reserve 0x00AE80F0, _M_insert_overflow 0x00AE81F0 and push_back 0x00AE83D0 (the landed reserve binds it as ?_M_clear@?$vector@UC3DGroup2@@...; FableWin.pdb spells the class V).
// Label: the manifest's "Container_FreeElements" is descriptive / propagated across byte-identical bodies.
// Callees: free 0x00BFEA14 (twice: the element's inner block, then the vector's block).
// Body: STLport 4.6 `_M_clear() { _Destroy(_M_start, _M_finish); _M_end_of_storage.deallocate(_M_start, ...); }` (same bytes as the destructor); the element destructors are implicit (the inner vector's
//   trivially destructible elements need no loop: STLport's __type_traits<T>::has_trivial_destructor picks the empty
//   __destroy_aux).
//   _M_clear is protected in STLport 4.6; it is public here as every landed binding spells it (the bytes are the same).
extern "C" void __cdecl free(void* block);                   // 0x00BFEA14
#include "engine/CArray.h"
class C3DVertexBlend2 {                                       // 8 bytes (Ego_r.pdb)
public:
    unsigned long VertexIndex;
    float BlendValue;
};
class C3DGroup2 {                                             // 16 bytes (Ego_r.pdb 20)
public:
    long BoneIndex;                                           // +0x00
    CArray<C3DVertexBlend2> VertexBlends;                     // +0x04
};
template class std::vector<C3DGroup2 >;