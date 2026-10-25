#include <cstdio>
// Behaviour test for vector<CTVertexSeaBackground>::erase(first, last) @ 0x00B80190: the tail moves down over the
// erased range, the finish pulls back, first is returned.
class CTVertexSeaBackground {
public:
    unsigned int Data[3];
};
namespace std {
template <class _RandomAccessIter, class _OutputIter, class _Distance>
inline _OutputIter __copy(_RandomAccessIter __first, _RandomAccessIter __last, _OutputIter __result, _Distance*)
{
    for (_Distance __n = __last - __first; __n > 0; --__n) { *__result = *__first; ++__first; ++__result; }
    return __result;
}
template <class _Tp> inline _Tp* copy(const _Tp* __first, const _Tp* __last, _Tp* __result)
{
    return __copy(__first, __last, __result, (int*)0);
}
template <class _Tp> class vector {
public:
    _Tp* _M_start;
    _Tp* _M_finish;
    _Tp* _M_end_of_storage;
    _Tp* erase(_Tp* __first, _Tp* __last)
    {
        _Tp* __i = copy((const _Tp*)__last, (const _Tp*)_M_finish, __first);
        _M_finish = __i;
        return __first;
    }
};
}
int main()
{
    static CTVertexSeaBackground buf[10];
    for (int i = 0; i < 10; ++i) {
        buf[i].Data[0] = i; buf[i].Data[1] = i * 2; buf[i].Data[2] = i * 100 + 11;
    }
    std::vector<CTVertexSeaBackground> v;
    v._M_start = buf; v._M_finish = buf + 9; v._M_end_of_storage = buf + 10;
    CTVertexSeaBackground* r = v.erase(buf + 2, buf + 5);
    if (r != buf + 2 || v._M_finish != buf + 6) { std::printf("00b80190_TEST FAIL\n"); return 1; }
    static const int expect[6] = { 0, 1, 5, 6, 7, 8 };
    for (int i = 0; i < 6; ++i) {
        if (buf[i].Data[0] != (unsigned int)expect[i] || buf[i].Data[1] != (unsigned int)expect[i] * 2) { std::printf("00b80190_TEST FAIL\n"); return 1; }
        if (buf[i].Data[2] != (unsigned int)(expect[i] * 100 + 11)) { std::printf("00b80190_TEST FAIL\n"); return 1; }
    }
    r = v.erase(buf + 6, buf + 6);                      // empty range: nothing moves
    if (r != buf + 6 || v._M_finish != buf + 6) { std::printf("00b80190_TEST FAIL\n"); return 1; }
    r = v.erase(buf, v._M_finish);                      // everything
    if (r != buf || v._M_finish != buf) { std::printf("00b80190_TEST FAIL\n"); return 1; }
    std::printf("00b80190_TEST PASS\n");
    return 0;
}