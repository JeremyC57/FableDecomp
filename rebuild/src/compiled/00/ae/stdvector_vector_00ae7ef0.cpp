// std::vector::~vector() -- (vector<C3DGroup2 >) out-of-line destructor: destroy every 0x10-byte element (its implicit destructor frees the inner VertexBlends array at +0x4 when it has a block), then free the vector's own block
// Leaf: ??1?$vector@VC3DGroup2@@V?$allocator@
// Plan phase 3 (2026-11-04), CArray wave: the local CArray (an empty public std::vector<T>) and its STLport stand-ins replaced by the shared engine/CArray.h (Lionhead's array: a public STLport 4.6 vector<T>, 12 bytes, no fields of its own). STLport algorithm / construct helpers and placement new come from the header's model (<new>).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00AE7EF0, 58 bytes (+ int3 padding), thiscall, plain `ret`.
// Identity: vector<C3DGroup2 >::~vector -- proven; evidence: the CParticleProgram destructor 0x00AE3670 (it stores the CParticleProgram vtable 0x0129F0D8) destroys NonSimGroups (+0x108) and SimGroups (+0xFC) with it: Ego_r.pdb CArray<C3DGroup2> members at Ego_r 0x12C / 0x11C less the 4-byte-smaller retail containers before them; the jmp thunk 0x00AE81D0 is the CArray-level destructor.
// Label: the manifest's "FreeStringVector" is descriptive / propagated across byte-identical bodies.
// Callees: free 0x00BFEA14 (twice: the element's inner block, then the vector's block).
// Body: STLport 4.6 `~vector() { _Destroy(_M_start, _M_finish); }` with the inline `~_Vector_base() { if (_M_start) deallocate(_M_start, ...); }`; the element destructors are implicit (the inner vector's
//   trivially destructible elements need no loop: STLport's __type_traits<T>::has_trivial_destructor picks the empty
//   __destroy_aux).
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