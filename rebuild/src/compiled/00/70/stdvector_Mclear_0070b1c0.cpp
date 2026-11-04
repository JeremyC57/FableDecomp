// std::vector::_M_clear() -- (vector<CEngineTextureDecal >) _M_clear (the old block release of _M_insert_overflow / reserve / operator=): destroy every 0x14-byte element (its implicit destructor frees the inner OverlaidBlends vector at +0x8 when it has a block), then free the vector's own block
// Leaf: ?_M_clear@?$vector@VCEngineTextureDecal@@V?$allocator@
// Plan phase 3 (2026-11-04), CArray wave: the local CArray (an empty public std::vector<T>) and its STLport stand-ins replaced by the shared engine/CArray.h (Lionhead's array: a public STLport 4.6 vector<T>, 12 bytes, no fields of its own). STLport algorithm / construct helpers and placement new come from the header's model (<new>).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0070B1C0, 58 bytes (+ int3 padding), thiscall, plain `ret`.
// Identity: vector<CEngineTextureDecal >::_M_clear -- proven; evidence: its callers are the landed vector<CEngineTextureDecal>::_M_insert_overflow 0x0070B240 and operator= 0x0077B860, which bind it as ?_M_clear@?$vector@UCEngineTextureDecal@@...; 0x14-byte elements with OverlaidBlends' block at +8 (Ego_r.pdb CEngineTextureDecal; FableWin.pdb spells the class V).
// Label: the manifest's "Vector_Destructor_RandomAppearance" is descriptive / propagated across byte-identical bodies.
// Callees: free 0x00BFEA14 (twice: the element's inner block, then the vector's block).
// Body: STLport 4.6 `_M_clear() { _Destroy(_M_start, _M_finish); _M_end_of_storage.deallocate(_M_start, ...); }` (same bytes as the destructor); the element destructors are implicit (the inner vector's
//   trivially destructible elements need no loop: STLport's __type_traits<T>::has_trivial_destructor picks the empty
//   __destroy_aux).
//   _M_clear is protected in STLport 4.6; it is public here as every landed binding spells it (the bytes are the same).
extern "C" void __cdecl free(void* block);                   // 0x00BFEA14
#include "engine/CArray.h"
enum ECompositeBlendType { COMPOSITE_BLEND_TYPE_0 = 0 };
class CRGBColour { public: unsigned long Colour; };
class CCompositeBlendDesc {                                   // 16 bytes (Ego_r.pdb)
public:
    unsigned long TextureIndex;
    ECompositeBlendType BlendType;
    CRGBColour DiffuseColour;
    CRGBColour SpecularColour;
};
class CEngineTextureDecal {                                   // 20 bytes (Ego_r.pdb 24 with its 16-byte vector)
public:
    long OriginalTextureIndex;                                // +0x00
    long DecalID;                                             // +0x04
    std::vector<CCompositeBlendDesc> OverlaidBlends;          // +0x08
};
template class std::vector<CEngineTextureDecal >;