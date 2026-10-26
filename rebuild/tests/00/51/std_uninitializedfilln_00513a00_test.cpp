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
#pragma pack(push, 1)
struct SElem {
    unsigned char first; SRec second;
    SElem(long s) : first((unsigned char)s), second(s + 10) {}
    SElem(const SElem& o) : first(o.first), second(o.second) {}
    bool Check(long s) const { return first == (unsigned char)s && second.a == s + 10 && second.b == s + 11 && second.c == s + 12; }
};
#pragma pack(pop)
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
    if (sizeof(SElem) != 1 + 3 * sizeof(long)) ok = false;
    if (!ok) { std::printf("00513a00_TEST FAIL\n"); return 1; }
    std::printf("00513a00_TEST PASS\n");
    return 0;
}