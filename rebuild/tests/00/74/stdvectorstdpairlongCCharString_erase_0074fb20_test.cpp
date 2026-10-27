// behaviour test for 0x0074FB20 std::vector<std::pair<long, CCharString>>::erase(first, last) with a counting stand-in element
#include <cstdio>
#include <cstddef>
static int g_dtors = 0;
struct _Elem { int key; ~_Elem() { ++g_dtors; } };
namespace std {
struct __false_type { };
struct random_access_iterator_tag { };
}
template <class _RandomAccessIter, class _OutputIter, class _Distance>
_OutputIter __copy(_RandomAccessIter __first, _RandomAccessIter __last, _OutputIter __result, const std::random_access_iterator_tag&, _Distance*)
{
    for (_Distance __n = __last - __first; __n > 0; --__n) { *__result = *__first; ++__first; ++__result; }
    return __result;
}
template <class _Tp> inline _Tp* __copy_ptrs(_Tp* __first, _Tp* __last, _Tp* __result, const std::__false_type&)
{ return __copy(__first, __last, __result, std::random_access_iterator_tag(), (ptrdiff_t*)0); }
template <class _Tp> inline void _Destroy1(_Tp* __pointer) { __pointer->~_Tp(); }
template <class _Tp> inline void _Destroy(_Tp* __first, _Tp* __last) { for ( ; __first != __last; ++__first) _Destroy1(&*__first); }
struct TVec {
    _Elem* _M_start; _Elem* _M_finish; _Elem* _M_end;
    _Elem* erase(_Elem* __first, _Elem* __last)
    {
        _Elem* __i = __copy_ptrs(__last, _M_finish, __first, std::__false_type());
        _Destroy(__i, _M_finish);
        _M_finish = __i;
        return __first;
    }
};
int main()
{
    static double raw[8];
    _Elem* e = (_Elem*)raw;
    for (int i = 0; i < 6; ++i) e[i].key = i * 10;
    TVec v; v._M_start = e; v._M_finish = e + 6; v._M_end = e + 6;
    g_dtors = 0;
    _Elem* r = v.erase(e + 1, e + 3);                    // drop 10, 20
    int ok = r == e + 1 && v._M_finish == e + 4 && g_dtors == 2 && e[0].key == 0 && e[1].key == 30 && e[3].key == 50;
    g_dtors = 0;
    r = v.erase(e, v._M_finish);                         // clear
    ok = ok && r == e && v._M_finish == e && g_dtors == 4;
    if (!ok) { std::printf("0074fb20_TEST FAIL\n"); return 1; }
    std::printf("0074fb20_TEST PASS\n");
    return 0;
}