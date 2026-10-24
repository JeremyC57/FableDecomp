#include <cstdio>
#include <cstdlib>
#include <new>
#ifndef _MSC_VER
#define __fastcall
#endif
// Model of _STL::uninitialized_fill_n<CThemeWeight*> @ 0x00885DD0: copy-construct n theme weights into raw storage
// through the inline `__false_type` helper, return first + n.
struct CThemeWeight {
    int   ThemeID;
    float Weight;
    bool  LockWeight;
};
struct __false_type { __false_type() { } };
template <class _T1, class _T2>
inline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }
template <class _ForwardIter, class _Size, class _Tp>
inline _ForwardIter UninitFillNAux(_ForwardIter __first, _Size __n, const _Tp& __x, const __false_type&)
{
    _ForwardIter __cur = __first;
    for (; __n > 0; --__n, ++__cur)
        _Construct(&*__cur, __x);
    return __cur;
}
template <class _ForwardIter, class _Size, class _Tp>
_ForwardIter __fastcall UninitFillN(_ForwardIter __first, _Size __n, const _Tp& __x)
{
    return UninitFillNAux(__first, __n, __x, __false_type());
}
int main()
{
    bool ok = true;
    CThemeWeight* raw = (CThemeWeight*)malloc(5 * sizeof(CThemeWeight));
    CThemeWeight w = { 42, 0.75f, true };
    CThemeWeight* end = UninitFillN(raw, 5u, w);
    if (end != raw + 5) ok = false;
    for (int i = 0; i < 5; ++i)
        if (raw[i].ThemeID != 42 || raw[i].Weight != 0.75f || !raw[i].LockWeight) ok = false;
    if (UninitFillN(raw, 0u, w) != raw) ok = false;
    free(raw);
    if (!ok) { std::printf("00885dd0_TEST FAIL\n"); return 1; }
    std::printf("00885dd0_TEST PASS\n");
    return 0;
}