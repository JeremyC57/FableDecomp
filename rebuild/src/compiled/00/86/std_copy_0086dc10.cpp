// std::__copy(_RandomAccessIter first, _RandomAccessIter last, _OutputIter result, const random_access_iterator_tag&, _Distance*) -- STLport random-access copy: assign last - first elements forward, return the end of the output (CTextureMorphMap)
// Leaf: ??$__copy@PAV?$CVectorMap@KVCTextureMorph@CRandomAppearanceMorph@@V?$CKeyPairCompareLess@KVCTextureMorph@CRandomAppearanceMorph@@@@@@PAV1@H@std@@YIPAV?$CVectorMap@KVCTextureMorph@CRandomAppearanceMorph@@V?$CKeyPairCompareLess@KVCTextureMorph@CRandomAppearanceMorph@@@@@@PAV1@00ABUrandom_access_iterator_tag@0@PAH@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x0086dc10, 59 bytes, __fastcall (first in ecx, last in edx), `ret 0Ch`.
// Identity (proven): bound (reloc_bindings) as ??$__copy@PAV?$CVectorMap@KVCTextureMorph@CRandomAppearanceMorph@@...@@PAV1@H@std@@YI...; its element is 16 bytes and its assignment is CVectorMap's implicit operator= -- the base vector<pair<unsigned long, CTextureMorph> >::operator= 0x006741E0 (bound) then Dirty (+0x0D, Ego_r.pdb +17); caller the landed vector erase 0x0086E0B0 and 0x0086DD80.
// Callee: the element's member assignment 0x006741E0 (thiscall).
// Shape (family L031): n = (last - first) >> 4; per element one thiscall assignment of the leading member, then the
//   trailing scalar copied through al/eax; returns the advanced result.
namespace std {
template <class _Tp> class allocator { };
template <class _T1, class _T2> struct pair {
    _T1 first;                                            // +0x00
    _T2 second;                                           // +0x04
};
template <class _Tp, class _Alloc = allocator<_Tp> >
class vector {                                             // STLport 4.6, 12 bytes
public:
    _Tp* _M_start;                                        // +0x00
    _Tp* _M_finish;                                       // +0x04
    _Tp* _M_end_of_storage;                               // +0x08
    vector<_Tp, _Alloc>& operator=(const vector<_Tp, _Alloc>& __x);   // 0x006741E0
};
}
class CRandomAppearanceMorph {
public:
    class CTextureMorph {                                  // Ego_r.pdb, 8 bytes
    public:
        unsigned long NewTexture;                         // +0x00
        unsigned long GroupID;                            // +0x04
    };
};
template <class K, class V> class CKeyPairCompareLess { };  // empty comparator
template <class K, class V, class C>
class CVectorMap : public std::vector<std::pair<K, V> > {  // Ego_r.pdb: the vector base, then Compare, Dirty
public:
    C Compare;                                            // +0x0C
    bool Dirty;                                           // +0x0D
};
typedef CVectorMap<unsigned long, CRandomAppearanceMorph::CTextureMorph,
                   CKeyPairCompareLess<unsigned long, CRandomAppearanceMorph::CTextureMorph> > CTextureMorphMap;
namespace std {
struct random_access_iterator_tag { };
template <class _RandomAccessIter, class _OutputIter, class _Distance>
_OutputIter __fastcall __copy(_RandomAccessIter __first, _RandomAccessIter __last, _OutputIter __result,
                              const random_access_iterator_tag&, _Distance*)
{
    for (_Distance __n = __last - __first; __n > 0; --__n) {
        *__result = *__first;
        ++__first;
        ++__result;
    }
    return __result;
}
}
template CTextureMorphMap* __fastcall std::__copy(CTextureMorphMap*, CTextureMorphMap*, CTextureMorphMap*, const std::random_access_iterator_tag&, int*);