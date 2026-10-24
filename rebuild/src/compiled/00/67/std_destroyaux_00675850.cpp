// std::__destroy_aux(CBodyPartMeshArray* first, CBodyPartMeshArray* last, const __false_type&) -- destroy a range of CArray<CRandomAppearanceMorph::CBodyPartMesh> (each mesh frees its texture-morph block)
// Leaf: ?__destroy_aux@std@@YIXPAU?$CRetailVector@UCBodyPartMesh@FableDestroy_00675850@@@FableDestroy_00675850@@0ABU__false_type@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00675850, length 84 bytes, `ret 4`.
// Element: CArray<CRandomAppearanceMorph::CBodyPartMesh> (a 12-byte STLport vector), the element of
// CRandomAppearanceMorph::BodyParts (CArray<CArray<CBodyPartMesh> >). Each 24-byte CBodyPartMesh (MeshIndex,
// Priority, TextureMorphs CVectorMap at +8) frees its map's vector block ([esi+8]), then the outer block is freed.
// Callers: 0x006758B0, 0x00675980, 0x00676740 (CRandomAppearanceMorph serialisation) and 0x0086EA80.. vector helpers;
// same TU as 0x00674FA0 (the CBodyPartMesh copy). Callees: free @ 0x00BFEA14.
// Shape: STLport 4.x `__destroy_aux(first, last, const __false_type&)` from a /Gr TU (`__fastcall`: first in ecx, last
// in edx, the empty tag reference on the stack, `ret 4`): `for (; first != last; ++first) _Destroy(&*first);` with the
// element's implicit destructor inlined.
// Label correction: the catalog's `_Uninit_copy<_Vector_const_iterator<_Vector_val<unsigned int,allocator<unsigned int> > >,
// unsigned int*,allocator<unsigned int> >` is a Dinkumware name propagated over all 40 rows of this chunk; retail's STL
// is STLport 4.x and the `ret 4` (no destination), the element stride and the inline destructor calls name this body.
extern "C" void __cdecl free(void* p);                  // 0x00BFEA14
struct __false_type { __false_type() { } };
namespace FableDestroy_00675850 {
template <class T>
inline void DestroyRange(T* first, T* last)             // STLport _Destroy(first, last) -> __destroy_aux, inlined
{
    for (; first != last; ++first)
        first->~T();
}
template <class T>
struct CRetailVector {                                  // STLport vector<T>: 12 bytes (start, finish, end of storage)
    T* _M_start;                                        // +0
    T* _M_finish;                                       // +4
    T* _M_end_of_storage;                               // +8
    ~CRetailVector()
    {
        DestroyRange(_M_start, _M_finish);
        if (_M_start)
            free(_M_start);                             // _Vector_base: deallocate through the malloc allocator
    }
};
struct CTextureMorph { unsigned long NewTexture; unsigned long GroupID; };   // Ego_r.pdb, POD
struct STextureMorphEntry { unsigned long first; CTextureMorph second; };
struct CKeyPairCompareLess { };                        // empty comparator
struct CTextureMorphMap {                               // CVectorMap<unsigned long, CTextureMorph>: 14 bytes
    CRetailVector<STextureMorphEntry> m_vector;         // +0x00
    CKeyPairCompareLess               Compare;          // +0x0C
    bool                              Dirty;            // +0x0D
};
struct CBodyPartMesh {                                  // Ego_r.pdb CRandomAppearanceMorph::CBodyPartMesh (retail 24 bytes)
    long             MeshIndex;                         // +0x00
    long             Priority;                          // +0x04
    CTextureMorphMap TextureMorphs;                     // +0x08
};
typedef CRetailVector<CBodyPartMesh> CBodyPartMeshArray;
}
namespace std {
template <class T> inline void _Destroy(T* p) { p->~T(); }
void __fastcall __destroy_aux(FableDestroy_00675850::CBodyPartMeshArray* first, FableDestroy_00675850::CBodyPartMeshArray* last, const __false_type&)
{
    for (; first != last; ++first)
        _Destroy(&*first);
}
}