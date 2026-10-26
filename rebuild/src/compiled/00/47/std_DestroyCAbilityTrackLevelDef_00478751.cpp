// std::_Destroy<CAbilityTrackLevelDef*>(CAbilityTrackLevelDef* first, CAbilityTrackLevelDef* last) -- STLport: run each
//   element's (virtual) destructor over [first, last)
// Leaf: ??$_Destroy@PAVCAbilityTrackLevelDef@@@std@@YIXPAVCAbilityTrackLevelDef@@0@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00478751, 26 bytes, first/last in ecx/edx, plain `ret` (two arguments: the
//   2-argument _Destroy, with __destroy / __destroy_aux inlined; the out-of-line __destroy_aux would `ret 4`).
// Identity: the element is 0x48 bytes with a virtual destructor called through vtable slot 0 with flag 0 (`p->~T()`).
//   Callers: the landed CAbilityTrackDef::~CAbilityTrackDef 0x00478722 on this+0x40 (Ego_r.pdb CAbilityTrackDef::Abilities,
//   vector<CAbilityTrackLevelDef>, +0x44 with the 0x40 debug parent), the landed std::vector<CAbilityTrackLevelDef>::erase
//   0x00478C2D (whose assignment 0x00478876 is CAbilityTrackLevelDef::operator=, the target of CAbilityTrackLevelDef::Copy
//   0x00478871), and 0x00478974 / 0x00478A0A in the same vector's code. The manifest's
//   "vector<CIntelligentPointer<CThing const> >::_Destroy" is propagated (that element is 8 bytes) and the catalog's
//   "DestroyRange_Elem0x48" is descriptive.
// Shape: STLport 4.6 `_Destroy(first, last)` -> `__destroy(first, last, value_type)` -> `__destroy_aux(first, last,
//   __false_type)`: `for ( ; first != last; ++first) _Destroy(&*first);` with `_Destroy(p)` = `p->~_Tp()`. Size TU
//   (unaligned entry); the size TU does not expand plain `inline` helpers, so the three are __forceinline (as the
//   __copy helpers of the sibling 0x00478C2D), and _Destroy carries the /Gr TU's __fastcall explicitly.
#pragma optimize("s", on)

class CAbilityTrackLevelDef {                           // retail 0x48 bytes (only the size and the destructor matter here)
public:
    virtual ~CAbilityTrackLevelDef();
    unsigned char _body[0x44];
};

namespace std {

struct __false_type { };

template <class _Tp> struct __type_traits {
    typedef __false_type has_trivial_destructor;
};

template <class _Tp> __forceinline void _Destroy(_Tp* __pointer)
{
    __pointer->~_Tp();
}

template <class _ForwardIterator>
__forceinline void __destroy_aux(_ForwardIterator __first, _ForwardIterator __last, const __false_type&)
{
    for ( ; __first != __last; ++__first)
        _Destroy(&*__first);
}

template <class _ForwardIterator, class _Tp>
__forceinline void __destroy(_ForwardIterator __first, _ForwardIterator __last, _Tp*)
{
    typedef typename __type_traits<_Tp>::has_trivial_destructor _Trivial_destructor;
    __destroy_aux(__first, __last, _Trivial_destructor());
}

template <class _ForwardIterator>
void __fastcall _Destroy(_ForwardIterator __first, _ForwardIterator __last)
{
    __destroy(__first, __last, (CAbilityTrackLevelDef*)0);
}

template void __fastcall _Destroy<CAbilityTrackLevelDef*>(CAbilityTrackLevelDef*, CAbilityTrackLevelDef*);

} // namespace std
