#include <cstdio>
#include <new>
struct __false_type { __false_type() {} };
static int g_stringCopies = 0;
static int g_arrayCopies = 0;
class CCharString { public:
    const char* PStringData;
    CCharString(const char* s) : PStringData(s) {}
    CCharString(const CCharString& other) : PStringData(other.PStringData) { ++g_stringCopies; }
};
struct SRec { long a, b, c; SRec(long s) : a(s), b(s + 1), c(s + 2) {} SRec(const SRec& o) : a(o.a), b(o.b), c(o.c) {} };
struct CArray12 { long* m_first; long* m_last; long* m_end; CArray12() : m_first(0), m_last(0), m_end(0) {} CArray12(const CArray12& o) : m_first(o.m_first), m_last(o.m_last), m_end(o.m_end) { ++g_arrayCopies; } };
struct S16 { long a, b, c, d; };
struct SElem {
    CArray12 arr; long mc; S16 s; long m20; SRec rec;
    SElem(long v) : mc(v), m20(v + 1), rec(v + 2) { s.a = v + 10; s.b = v + 11; s.c = v + 12; s.d = v + 13; }
    SElem(const SElem& o) : arr(o.arr), mc(o.mc), s(o.s), m20(o.m20), rec(o.rec) {}
    bool Check(long v) const { return mc == v && m20 == v + 1 && rec.a == v + 2 && s.a == v + 10 && s.d == v + 13 && arr.m_first == 0; }
};
namespace stl {
template <class T> T* uninitialized_fill_n(T* first, unsigned long n, const T& x, const __false_type&)
{
    T* cur = first;
    for (; n > 0; --n, ++cur)
        new (cur) T(x);
    return cur;
}
}
int main()
{
    unsigned char buffer[sizeof(SElem) * 5];
    SElem* first = (SElem*)buffer;
    SElem x(100);
    SElem* end = stl::uninitialized_fill_n(first, 5, x, __false_type());
    bool ok = end == first + 5;
    unsigned long i = 0;
    for (; i < 5; ++i)
        if (!first[i].Check(100))
            ok = false;
    SElem* none = stl::uninitialized_fill_n(first, 0, x, __false_type());
    if (none != first)
        ok = false;
    if (g_arrayCopies != 5) ok = false;
    if (!ok) { std::printf("009f6c90_TEST FAIL\n"); return 1; }
    std::printf("009f6c90_TEST PASS\n");
    return 0;
}