// std::uninitialized_fill_n(CBlendedEnvironmentTheme* first, unsigned int n, const CBlendedEnvironmentTheme& x) -- copy-construct n blended environment themes into raw storage, return first + n
// Leaf: ??$uninitialized_fill_n@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00889800, length 48 bytes (+ int3 padding), `ret 4`.
// Identity: the catalog label `_Fill_n<unsigned_long*,...>` is propagated from a byte twin. One caller, 0x0088A245, the
//   vector<CBlendedEnvironmentTheme> fill-insert of the environment TU (which assigns through the landed
//   CBlendedEnvironmentTheme::operator= 0x006B5DF0 via the fill 0x008897D0 of the same element).
// Callees: the element's out-of-line copy constructor @ 0x008894A0 (implicit memberwise copy: a word, a dword block,
//   the nested CArrays).
// Shape: STLport 4.x `uninitialized_fill_n(first, n, x)` with the `__uninitialized_fill_n(..., __false_type)` helper
//   inlined (the tag leaves no slot: `ret 4` pops only &x), `__fastcall` (first in ecx -> esi as `cur`, n in edx -> edi
//   counted down, &x -> ebx); placement new null test; cur returned in eax.
inline void* operator new(unsigned int, void* p) { return p; }   // placement new (the null test on the destination)

namespace NEnvironmentInternal {
class CBlendedEnvironmentTheme {                // 500 bytes (retail 12-byte CArrays)
public:
    unsigned char _body[0x1F4];
    CBlendedEnvironmentTheme(const CBlendedEnvironmentTheme& other);   // 0x008894A0
};
}

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
template NEnvironmentInternal::CBlendedEnvironmentTheme* __fastcall uninitialized_fill_n<NEnvironmentInternal::CBlendedEnvironmentTheme*, unsigned int, NEnvironmentInternal::CBlendedEnvironmentTheme>(
    NEnvironmentInternal::CBlendedEnvironmentTheme*, unsigned int, const NEnvironmentInternal::CBlendedEnvironmentTheme&);
}