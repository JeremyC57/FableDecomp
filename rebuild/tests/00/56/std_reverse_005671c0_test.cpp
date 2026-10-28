// Behaviour test for 0x005671C0 std::__reverse over pair<long, CWideString> (a stand-in CWideString that owns a
// heap copy of its value, so leaks or double frees in the swap would show up).
#include <cstdio>
#include <cstdlib>
#include <new>

static int g_live = 0;
class CWideString { public:
    int* m_pData;
    explicit CWideString(int v) : m_pData((int*)std::malloc(sizeof(int))) { *m_pData = v; ++g_live; }
    CWideString(const CWideString& o) : m_pData((int*)std::malloc(sizeof(int))) { *m_pData = *o.m_pData; ++g_live; }
    const CWideString& operator=(const CWideString& o) { *m_pData = *o.m_pData; return *this; }
    ~CWideString() { std::free(m_pData); --g_live; }
};

namespace stl {
struct random_access_iterator_tag { };
template <class _T1, class _T2> struct pair {
    _T1 first;
    _T2 second;
    pair(const _T1& __a, const _T2& __b) : first(__a), second(__b) { }
    pair(const pair& __o) : first(__o.first), second(__o.second) { }
};
template <class _ForwardIter1, class _ForwardIter2, class _Tp>
inline void __iter_swap(_ForwardIter1 __a, _ForwardIter2 __b, _Tp*)
{
    _Tp __tmp = *__a;
    *__a = *__b;
    *__b = __tmp;
}
template <class _Tp> inline _Tp* __value_type(const _Tp*) { return (_Tp*)0; }
template <class _ForwardIter1, class _ForwardIter2>
inline void iter_swap(_ForwardIter1 __i1, _ForwardIter2 __i2) { __iter_swap(__i1, __i2, __value_type(__i1)); }
template <class _RandomAccessIter>
void __reverse(_RandomAccessIter __first, _RandomAccessIter __last, const random_access_iterator_tag&)
{
    for (; __first < __last; ++__first)
        iter_swap(__first, --__last);
}
}

typedef stl::pair<long, CWideString> Entry;

static int Check(int n)
{
    Entry* v = (Entry*)std::malloc(sizeof(Entry) * (n ? n : 1));
    int i;
    for (i = 0; i < n; ++i) new (&v[i]) Entry(i * 10, CWideString(i * 100));
    stl::__reverse(v, v + n, stl::random_access_iterator_tag());
    int ok = 1;
    for (i = 0; i < n; ++i)
        if (v[i].first != (n - 1 - i) * 10 || *v[i].second.m_pData != (n - 1 - i) * 100) ok = 0;
    for (i = 0; i < n; ++i) v[i].~Entry();
    std::free(v);
    return ok;
}

int main()
{
    int ok = Check(0) && Check(1) && Check(2) && Check(5) && Check(6);
    if (!ok || g_live != 0) { std::printf("005671c0_TEST FAIL\n"); return 1; }
    std::printf("005671c0_TEST PASS\n");
    return 0;
}