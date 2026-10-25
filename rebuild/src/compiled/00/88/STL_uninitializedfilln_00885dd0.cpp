// std::uninitialized_fill_n(CThemeWeight* first, unsigned int n, const CThemeWeight& x) -- copy-construct n 12-byte theme weights into raw storage, return first + n
// Leaf: ??$uninitialized_fill_n@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00885DD0, length 64 bytes (+ int3 padding), `ret 4`.
// Identity: the catalog label `_Fill_n<unsigned_long*,...>` is propagated from a byte twin. One caller, 0x008891B8 in the
//   landed vector<Element>::_M_fill_insert 0x008890F0 (12-byte elements), the theme-weight vector of the environment TU
//   (CEnvironmentThemePriorityStackEntry::Themes, whose copy constructor is 0x006B58F0); the element is modelled as
//   Ego_r.pdb CThemeWeight { long ThemeID; float Weight; bool LockWeight; } (12 bytes, trivially copyable).
// Shape: STLport 4.x `uninitialized_fill_n(first, n, x)` with the `__false_type` helper inlined (`ret 4` pops only &x),
//   `__fastcall` (first in ecx -> eax as `cur`, n in edx counted down, &x -> ecx); the trivial copy constructor is a
//   three-dword bitwise copy through ebx/esi behind the placement-new null test.
inline void* operator new(unsigned int, void* p) { return p; }   // placement new (the null test on the destination)

struct CThemeWeight {                           // 12 bytes
    long  ThemeID;                              // +0x00
    float Weight;                               // +0x04
    bool  LockWeight;                           // +0x08
};

namespace std {
struct __false_type { __false_type() { } };

template <class _T1, class _T2>
inline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }

template <class _ForwardIter, class _Size, class _Tp>
inline _ForwardIter __uninitialized_fill_n(_ForwardIter __first, _Size __n, const _Tp& __x, const __false_type&)
{
    _ForwardIter __cur = __first;
    for (; __n > 0; --__n, ++__cur)
        _Construct(&*__cur, __x);
    return __cur;
}

template <class _ForwardIter, class _Size, class _Tp>
_ForwardIter __fastcall uninitialized_fill_n(_ForwardIter __first, _Size __n, const _Tp& __x)
{
    return __uninitialized_fill_n(__first, __n, __x, __false_type());
}
template CThemeWeight* __fastcall uninitialized_fill_n<CThemeWeight*, unsigned int, CThemeWeight>(CThemeWeight*, unsigned int, const CThemeWeight&);
}