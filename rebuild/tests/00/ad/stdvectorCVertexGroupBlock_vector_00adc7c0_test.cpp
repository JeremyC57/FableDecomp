// behaviour test for 0x00ADC7C0 std::vector<CVertexGroupBlock>::vector(const vector&) (model copy; element of the retail size)
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <new>
#ifndef _MSC_VER
#define __fastcall
#endif
struct _Elem { unsigned char Data[24]; };
struct __false_type { };
template <class _InputIter, class _ForwardIter>
_ForwardIter __uninitialized_copy(_InputIter __first, _InputIter __last, _ForwardIter __result, const __false_type&)
{
    for (; __first != __last; ++__first, ++__result) new (__result) _Elem(*__first);
    return __result;
}
struct TVec {
    _Elem* _M_start; _Elem* _M_finish; _Elem* _M_end;
    TVec() : _M_start(0), _M_finish(0), _M_end(0) { }
    TVec(const TVec& __x) : _M_start(0), _M_finish(0), _M_end(0)
    {
        unsigned int __n = (unsigned int)(__x._M_finish - __x._M_start);
        _M_start = __n != 0 ? (_Elem*)std::malloc(__n * sizeof(_Elem)) : 0;
        _M_finish = _M_start;
        _M_end = _M_start + __n;
        _M_finish = __uninitialized_copy((const _Elem*)__x._M_start, (const _Elem*)__x._M_finish, _M_start, __false_type());
    }
};
int main()
{
    _Elem src[3];
    for (int i = 0; i < 3; ++i) std::memset(src[i].Data, 0x10 + i, sizeof(src[i].Data));
    TVec a; a._M_start = src; a._M_finish = src + 3; a._M_end = src + 3;
    TVec b(a);
    int ok = b._M_finish - b._M_start == 3 && b._M_end == b._M_finish && b._M_start != src &&
             std::memcmp(b._M_start, src, sizeof(src)) == 0;
    TVec e; TVec f(e);
    ok = ok && f._M_start == 0 && f._M_finish == 0 && f._M_end == 0;
    std::free(b._M_start);
    if (!ok) { std::printf("00adc7c0_TEST FAIL\n"); return 1; }
    std::printf("00adc7c0_TEST PASS\n");
    return 0;
}