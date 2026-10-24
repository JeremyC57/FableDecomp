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
struct SPair { long a, b; CCharString c; SPair(long s) : a(s), b(s + 1), c("c") {} SPair(const SPair& o) : a(o.a), b(o.b), c(o.c) {} };
struct SElem {
    CCharString name; SPair rec;
    SElem(long s) : name("marker"), rec(s) {}
    SElem(const SElem& o) : name(o.name), rec(o.rec) {}
    bool Check(long s) const { return name.PStringData[0] == 'm' && rec.a == s && rec.b == s + 1 && rec.c.PStringData[0] == 'c'; }
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
    if (g_stringCopies != 10) ok = false;
    if (!ok) { std::printf("00af2c00_TEST FAIL\n"); return 1; }
    std::printf("00af2c00_TEST PASS\n");
    return 0;
}