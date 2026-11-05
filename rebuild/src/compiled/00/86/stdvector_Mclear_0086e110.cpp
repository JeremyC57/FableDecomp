// std::vector::_M_clear() -- (vector<CRandomAppearanceMorph::CMorphSet::CMeshTextureSet >) _M_clear (the old block release of _M_insert_overflow / reserve / operator=): destroy every 0x10-byte element (its implicit destructor frees the inner TextureMorphs array at +0x4 when it has a block), then free the vector's own block
// Leaf: ?_M_clear@?$vector@VCMeshTextureSet@CMorphSet@CRandomAppearanceMorph@@V?$allocator@
// Plan phase 4 (2026-11-05), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end().
// Plan phase 3 (2026-11-04), CArray wave: the local CArray (an empty public std::vector<T>) and its STLport stand-ins replaced by the shared engine/CArray.h (Lionhead's array: a public STLport 4.6 vector<T>, 12 bytes, no fields of its own). STLport algorithm / construct helpers and placement new come from the header's model (<new>).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0086E110, 58 bytes (+ int3 padding), thiscall, plain `ret`.
// Identity: vector<CRandomAppearanceMorph::CMorphSet::CMeshTextureSet >::_M_clear -- proven; evidence: its callers are the _M_insert_overflow 0x0086E2D0 and push_back 0x0086E7C0 of vector<CMeshTextureSet> (the landed push_back binds it as this _M_clear); 0x10-byte elements with TextureMorphs' block at +4 (Ego_r.pdb CMeshTextureSet).
// Label: the manifest's "Vector_DeleteAll_4ints" is descriptive / propagated across byte-identical bodies.
// Callees: free 0x00BFEA14 (twice: the element's inner block, then the vector's block).
// Body: STLport 4.6 `_M_clear() { _Destroy(_M_start, _M_finish); _M_end_of_storage.deallocate(_M_start, ...); }` (same bytes as the destructor); the element destructors are implicit (the inner vector's
//   trivially destructible elements need no loop: STLport's __type_traits<T>::has_trivial_destructor picks the empty
//   __destroy_aux).
//   _M_clear is protected in STLport 4.6; it is public here as every landed binding spells it (the bytes are the same).
extern "C" void __cdecl free(void* block);                   // 0x00BFEA14
#include "engine/CArray.h"
#include "engine/CVectorMap.h"
class CRandomAppearanceMorph {
public:
    class CTextureMorph {                                     // 8 bytes (Ego_r.pdb)
    public:
        unsigned long NewTexture;
        unsigned long GroupID;
    };
    class CBodyPartMesh {                                     // 24 bytes (Ego_r.pdb 28 with its 20-byte vector map)
    public:
        long MeshIndex;                                       // +0x00
        long Priority;                                        // +0x04
        CVectorMap<unsigned long, CTextureMorph, CKeyPairCompareLess<unsigned long, CTextureMorph> > TextureMorphs;   // +0x08
    };
    class CMorphSet {
    public:
        class CMorphSetTextureMorph {                         // 8 bytes (Ego_r.pdb)
        public:
            long OriginalTexture;
            long NewTexture;
        };
        class CMeshTextureSet {                               // 16 bytes (Ego_r.pdb 20)
        public:
            long MeshIndex;                                   // +0x00
            CArray<CMorphSetTextureMorph> TextureMorphs;      // +0x04
        };
    };
};
template class std::vector<CRandomAppearanceMorph::CMorphSet::CMeshTextureSet >;