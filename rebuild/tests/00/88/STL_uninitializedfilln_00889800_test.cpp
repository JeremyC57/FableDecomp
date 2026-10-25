#include <cstdio>
#include <cstdlib>
#include <new>
#ifndef _MSC_VER
#define __fastcall
#endif
// Model of std::uninitialized_fill_n<CBlendedEnvironmentTheme*> @ 0x00889800: copy-construct n themes into raw
// storage through the class's copy constructor, return first + n.
static int g_copies = 0;
class CBlendedEnvironmentTheme {
public:
    int Value;
    CBlendedEnvironmentTheme() : Value(0) { }
    CBlendedEnvironmentTheme(const CBlendedEnvironmentTheme& other) : Value(other.Value) { ++g_copies; }   // 0x008894A0
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
    CBlendedEnvironmentTheme* raw = (CBlendedEnvironmentTheme*)malloc(3 * sizeof(CBlendedEnvironmentTheme));
    CBlendedEnvironmentTheme x;
    x.Value = 9;
    if (UninitFillN(raw, 3u, x) != raw + 3 || g_copies != 3) ok = false;
    for (int i = 0; i < 3; ++i)
        if (raw[i].Value != 9) ok = false;
    if (UninitFillN(raw, 0u, x) != raw || g_copies != 3) ok = false;
    free(raw);
    if (!ok) { std::printf("00889800_TEST FAIL\n"); return 1; }
    std::printf("00889800_TEST PASS\n");
    return 0;
}