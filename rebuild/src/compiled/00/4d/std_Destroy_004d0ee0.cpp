// std::_Destroy(CUnprovenCharStringEntry_004d0ee0* first, CUnprovenCharStringEntry_004d0ee0* last) -- STLport range destroy: run each element's destructor
// Leaf: ??$_Destroy@VCUnprovenCharStringEntry_004d0ee0@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x004D0EE0, 34 bytes, `__fastcall` (/Gr: first in ecx, last in edx), `ret`.
// Identity (descriptive): its only caller is the landed vector::operator= 0x00719B30 (stand-in CElement8bVector, "8-byte element type unconfirmed"); the element is a dword then a CCharString.
// Callees: the element destructor's one call -- ~CCharString 0x0099EAE0 on +4.
// Shape (family K118, representative 0x004D0EE0): STLport 4.6 `_Destroy(first, last)` -> `__destroy_aux(..., __false_type)` loop,
//   emitted out of line (the vector operator= calls it); the element's implicit destructor inlines to the member's call.
class CCharStringData;
class CCharString {                                            // 4 bytes: PStringData
public:
    ~CCharString();                                            // 0x0099EAE0
    CCharStringData* PStringData;
};
class CUnprovenCharStringEntry_004d0ee0 {               // 8 bytes: a dword, then a CCharString (unproven element)
public:
    unsigned long First;                                 // +0x00
    CCharString Second;                                  // +0x04
};
namespace std {
struct __true_type { };
struct __false_type { };
template <class _Tp> struct __type_traits { typedef __false_type has_trivial_destructor; };
template <class _Tp> inline void _Destroy(_Tp* __pointer) { __pointer->~_Tp(); }
template <class _ForwardIterator>
inline void __destroy_aux(_ForwardIterator __first, _ForwardIterator __last, const __false_type&)
{
    for ( ; __first != __last; ++__first)
        _Destroy(&*__first);
}
template <class _Tp> inline void __fastcall _Destroy(_Tp* __first, _Tp* __last)
{
    typedef typename __type_traits<_Tp>::has_trivial_destructor _Trivial_destructor;
    __destroy_aux(__first, __last, _Trivial_destructor());
}
}
template void __fastcall std::_Destroy<CUnprovenCharStringEntry_004d0ee0 >(CUnprovenCharStringEntry_004d0ee0*, CUnprovenCharStringEntry_004d0ee0*);