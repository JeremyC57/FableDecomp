// std::__copy_backward_ptrs(_Tp* first, _Tp* last, _Tp* result, const __false_type&) -- STLport __copy_backward_ptrs(first, last, result, __false_type): forward to the out-of-line __copy_backward with a random_access_iterator_tag and a null Distance* (CVectorMap<unsigned long, CRandomAppearanceMorph::CTextureMorph, ...>)
// Leaf: ??$__copy_backward_ptrs@V?$CVectorMap@KVCTextureMorph@CRandomAppearanceMorph@@V?$CKeyPairCompareLess@KVCTextureMorph@CRandomAppearanceMorph@@@@@@@std@@YIPAV?$CVectorMap@KVCTextureMorph@CRandomAppearanceMorph@@V?$CKeyPairCompareLess@KVCTextureMorph@CRandomAppearanceMorph@@@@@@PAV1@00ABU__false_type@0@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x0086d960, 20 bytes, __fastcall (first in ecx, last in edx, result on the stack), `ret 8`.
// Identity (probable): its callee 0x0086D5A0 copies 16-byte CVectorMap records through 0x006741E0, the vector operator= that the landed CRandomAppearanceMorph::operator= 0x00675B60 calls for GeneralTextureMorphs (CVectorMap<unsigned long, CTextureMorph, CKeyPairCompareLess<..> >); Ego_r.pdb CRandomAppearanceMorph::BodyPartTextureMorphs is CArray<CVectorMap<unsigned long, CTextureMorph, ...> > -- that array's element; no caller (unreferenced COMDAT).
// Callee: std::__copy_backward<CVectorMap<unsigned long, CRandomAppearanceMorph::CTextureMorph, ...>*, ..., int> 0x0086D5A0 (landed under a global-namespace stand-in).
// Shape (family L002): `push 0 ; lea eax,[esp+8] ; push eax ; push result ; call` -- the empty tag temporary is homed on the
//   result argument's own slot (its address is pushed, nothing is stored), ecx/edx pass through untouched.
#include <stddef.h>
namespace std {
template <class _Tp> class allocator { public: allocator() { } };
struct __false_type { __false_type() { } };
struct random_access_iterator_tag { random_access_iterator_tag() { } };
}
template <class _Tp> class CArray : public std::allocator<_Tp> { };   // (layout not used here)
class CRandomAppearanceMorph {
public:
    class CTextureMorph;
    class CBodyPartMesh;
};
template <class K, class V> class CKeyPairCompareLess { };
template <class K, class V, class C> class CVectorMap {     // retail 16 bytes: vector, empty comparator, sorted flag
public:
    void* Vector[3];                                       // +0x00 (vector<pair<K, V> >, operator= 0x006741E0)
    C Compare;                                             // +0x0C
    bool Sorted;                                           // +0x0D
};
typedef CVectorMap<unsigned long, CRandomAppearanceMorph::CTextureMorph, CKeyPairCompareLess<unsigned long, CRandomAppearanceMorph::CTextureMorph> > _Elem;
namespace std {
template <class _BidirectionalIter1, class _BidirectionalIter2, class _Distance>       // 0x0086D5A0
_BidirectionalIter2 __fastcall __copy_backward(_BidirectionalIter1 __first, _BidirectionalIter1 __last,
                                              _BidirectionalIter2 __result, const random_access_iterator_tag&, _Distance*);
template <class _Tp>
inline _Tp* __fastcall __copy_backward_ptrs(_Tp* __first, _Tp* __last, _Tp* __result, const __false_type&)
{
    return __copy_backward(__first, __last, __result, random_access_iterator_tag(), (ptrdiff_t*)0);
}
}
template _Elem* __fastcall std::__copy_backward_ptrs(_Elem*, _Elem*, _Elem*, const std::__false_type&);