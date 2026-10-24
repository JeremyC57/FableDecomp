#include <cstdio>
#include <new>
struct __false_type { __false_type() {} };
static int g_stringCopies = 0;
static int g_arrayCopies = 0;
struct CCharString {
    const char* PStringData;
    CCharString(const char* s) : PStringData(s) {}
    CCharString(const CCharString& other) : PStringData(other.PStringData) { ++g_stringCopies; }
};
struct SRec { long a, b, c; SRec(long s) : a(s), b(s + 1), c(s + 2) {} SRec(const SRec& o) : a(o.a), b(o.b), c(o.c) {} };
struct SElem {
    long m0, m4, m8, mc; SRec rec;
    SElem(long s) : m0(s), m4(s + 1), m8(s + 2), mc(s + 3), rec(s + 4) {}
    SElem(const SElem& o) : m0(o.m0), m4(o.m4), m8(o.m8), mc(o.mc), rec(o.rec) {}
    bool Check(long s) const { return m0 == s && m4 == s + 1 && m8 == s + 2 && mc == s + 3 && rec.a == s + 4 && rec.c == s + 6; }
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
    if (!ok) { std::printf("00543870_TEST FAIL\n"); return 1; }
    std::printf("00543870_TEST PASS\n");
    return 0;
}