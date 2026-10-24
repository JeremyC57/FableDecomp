// std::_Destroy(CTextureMorphMap* first, CTextureMorphMap* last) -- destroy a range of CVectorMap<unsigned long, CTextureMorph> maps (free each map's vector block)
// Leaf: ?_Destroy@std@@YIXPAUCTextureMorphMap@FableDestroy_00675310@@0@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00675310, length 41 bytes, plain `ret`.
// Element: 16 bytes = CVectorMap<unsigned long, CRandomAppearanceMorph::CTextureMorph> (a 12-byte STLport vector of
// POD pairs, the empty comparator +0xC and the Dirty flag +0xD, padded to 16), the element of
// CRandomAppearanceMorph::BodyPartTextureMorphs (CArray<CVectorMap<...> >): destroying it frees the vector block.
// Caller: 0x006753F0 (that vector's operator=, manifest "Vector_Assign_16byte_CFeatDef"); same TU as 0x00675850.
// Callees: free @ 0x00BFEA14.
// Shape: STLport 4.x `_Destroy(first, last)` emitted out of line from a /Gr TU (`__fastcall`: first in ecx, last in
// edx, plain `ret`: no tag argument): `__destroy(first, last, value_type)` -> `__destroy_aux(first, last,
// __false_type())` inlined, with the element's implicit destructor inlined in the loop.
// Label correction: the catalog's `_Uninit_copy<_Vector_const_iterator<_Vector_val<unsigned int,allocator<unsigned int> > >,
// unsigned int*,allocator<unsigned int> >` is a Dinkumware name propagated over all 40 rows of this chunk; retail's STL
// is STLport 4.x and the plain `ret` (two register arguments, no destination), the element stride and the inline frees name this body.
extern "C" void __cdecl free(void* p);                  // 0x00BFEA14
struct __false_type { __false_type() { } };
namespace FableDestroy_00675310 {
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
struct CTextureMorphMap {                               // CVectorMap<unsigned long, CTextureMorph>: 16 bytes in an array
    CRetailVector<STextureMorphEntry> m_vector;         // +0x00
    CKeyPairCompareLess               Compare;          // +0x0C
    bool                              Dirty;            // +0x0D
};
}
namespace std {
template <class T> inline void _Destroy(T* p) { p->~T(); }
template <class T> inline void __destroy_aux(T* first, T* last, const __false_type&)
{
    for (; first != last; ++first)
        _Destroy(&*first);
}
void __fastcall _Destroy(FableDestroy_00675310::CTextureMorphMap* first, FableDestroy_00675310::CTextureMorphMap* last)
{
    __destroy_aux(first, last, __false_type());
}
}