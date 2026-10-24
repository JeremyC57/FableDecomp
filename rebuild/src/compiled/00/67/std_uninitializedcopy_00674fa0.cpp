// std::__uninitialized_copy(const CBodyPartMesh* first, const CBodyPartMesh* last, CBodyPartMesh* result, const __false_type&) -- CRandomAppearanceMorph::CBodyPartMesh elements: two words, the texture-morph map, return the end
// Leaf: ?__uninitialized_copy@std@@YIPAUCBodyPartMesh@FableCopy_00674fa0@@PBU23@0PAU23@ABU__false_type@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00674FA0, length 66 bytes, `ret 8`.
// Element: CRandomAppearanceMorph::CBodyPartMesh (Ego_r.pdb: MeshIndex +0, Priority +4, TextureMorphs +8 =
// CVectorMap<unsigned long, CTextureMorph>); retail size 24: the CVectorMap is a 12-byte STLport vector, an EMPTY
// comparator at +0x14 (copied as nothing) and the Dirty flag at +0x15 (GOTCHAS 00a85360). The implicit copy
// constructor copies the two words, calls the vector copy constructor 0x00673E10 (its body is the
// pair<unsigned long, CTextureMorph> copy, cf. 0x00673C60) and moves the Dirty byte.
// Callers: 0x0067501C (the helper right after it), 0x00675230 (vector copy), 0x006755E0 (range insert), 0x006756FE;
// same TU as 0x00675850.
// Callees: vector<pair<unsigned long, CTextureMorph> >::vector(const vector&) @ 0x00673E10.
// Shape: STLport 4.x `__uninitialized_copy(first, last, result, const __false_type&)` from a /Gr TU (`__fastcall`: first
// in ecx, last in edx, result and the empty tag reference on the stack, `ret 8`): `for (; first != last; ++first,
// ++cur) _Construct(&*cur, *first);` -- a bottom-tested loop behind the `first == last` test, the placement new
// null-tests the destination, the advanced result is returned in eax.
// Label correction: the catalog's `_Uninit_copy<_Vector_const_iterator<_Vector_val<unsigned int,allocator<unsigned int> > >,
// unsigned int*,allocator<unsigned int> >` is a Dinkumware name propagated over all 40 rows of this chunk; retail's STL
// is STLport 4.x and the `ret 8`, the element stride and the placement null test name this body.
inline void* operator new(unsigned int, void* where) { return where; }
struct __false_type { __false_type() { } };
namespace FableCopy_00674fa0 {
struct CTextureMorphVector {                            // STLport vector<pair<unsigned long, CTextureMorph> >: 12 bytes
    void* _M_start;
    void* _M_finish;
    void* _M_end_of_storage;
    CTextureMorphVector(const CTextureMorphVector& other);   // 0x00673E10
};
struct CKeyPairCompareLess { };                        // empty comparator
struct CTextureMorphMap : CTextureMorphVector {         // CVectorMap<unsigned long, CTextureMorph>: 14 bytes
    CKeyPairCompareLess Compare;                        // +0x0C
    bool                Dirty;                          // +0x0D
};
struct CBodyPartMesh {                                  // Ego_r.pdb CRandomAppearanceMorph::CBodyPartMesh (retail 24 bytes)
    long             MeshIndex;                         // +0x00
    long             Priority;                          // +0x04
    CTextureMorphMap TextureMorphs;                     // +0x08
};
}
namespace std {
FableCopy_00674fa0::CBodyPartMesh* __fastcall __uninitialized_copy(const FableCopy_00674fa0::CBodyPartMesh* first, const FableCopy_00674fa0::CBodyPartMesh* last,
    FableCopy_00674fa0::CBodyPartMesh* result, const __false_type&)
{
    FableCopy_00674fa0::CBodyPartMesh* cur = result;
    for (; first != last; ++first, ++cur)
        new ((void*)cur) FableCopy_00674fa0::CBodyPartMesh(*first);
    return cur;
}
}