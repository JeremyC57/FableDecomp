// std::vector::~vector() -- (vector<CRandomAppearanceMorph::CBodyPartMesh >) out-of-line destructor: destroy every 0x18-byte element (its implicit destructor frees the inner CVectorMap's vector at +0x8 when it has a block), then free the vector's own block
// Leaf: ??1?$vector@VCBodyPartMesh@CRandomAppearanceMorph@@V?$allocator@
// Plan phase 3 (2026-11-04), CArray wave: the local CArray (an empty public std::vector<T>) and its STLport stand-ins replaced by the shared engine/CArray.h (Lionhead's array: a public STLport 4.6 vector<T>, 12 bytes, no fields of its own). STLport algorithm / construct helpers and placement new come from the header's model (<new>).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006755A0, 58 bytes (+ int3 padding), thiscall, plain `ret`.
// Identity: vector<CRandomAppearanceMorph::CBodyPartMesh >::~vector -- proven; evidence: the landed CRandomAppearanceMorph constructor 0x0086C4D0 (and 0x0086EE50, 0x0086ECA0) destroys a local CArray<CRandomAppearanceMorph::CBodyPartMesh> through it (the CArray destructor inlined), and the unreferenced CArray<CBodyPartMesh> deleting destructor 0x00675770 calls it; 0x18-byte elements with the TextureMorphs vector map's block at +8 (Ego_r.pdb CBodyPartMesh: MeshIndex, Priority, CVectorMap<unsigned long, CTextureMorph>); the jmp thunks 0x00675720 / 0x006757F0 are the CArray-level destructors that tail-call it.
// Label: the manifest's "CScriptInfo_DestroyArray" is descriptive / propagated across byte-identical bodies.
// Callees: free 0x00BFEA14 (twice: the element's inner block, then the vector's block).
// Body: STLport 4.6 `~vector() { _Destroy(_M_start, _M_finish); }` with the inline `~_Vector_base() { if (_M_start) deallocate(_M_start, ...); }`; the element destructors are implicit (the inner vector's
//   trivially destructible elements need no loop: STLport's __type_traits<T>::has_trivial_destructor picks the empty
//   __destroy_aux).
extern "C" void __cdecl free(void* block);                   // 0x00BFEA14
#include "engine/CArray.h"
template <class K, class V> class CKeyPairCompareLess { };
template <class K, class V, class C> class CVectorMap : public CArray<std::pair<K, V> > {   // 16 bytes in retail
public:
    C    Compare;                                             // +0x0c
    bool Dirty;                                               // +0x0d
};
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
template class std::vector<CRandomAppearanceMorph::CBodyPartMesh >;