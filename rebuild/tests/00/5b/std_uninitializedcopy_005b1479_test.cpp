// behaviour test for 0x005B1479: each copied pointer adds one reference; null pointers are copied as null
#include <cstdio>
#include <new>
struct Def { long refs; };
struct P { Def* o; P(const P& x) : o(x.o) { if (o) ++o->refs; } P(Def* d) : o(d) { } };
static P* Copy(const P* f, const P* l, P* r) { for (; f != l; ++f, ++r) new (r) P(*f); return r; }
int main()
{
    Def d = { 1 }; P src[3] = { P(&d), P(0), P(&d) };
    void* raw = ::operator new(3 * sizeof(P)); P* dst = (P*)raw;
    P* end = Copy(src, src + 3, dst);
    bool ok = end == dst + 3 && d.refs == 3 && dst[1].o == 0 && dst[2].o == &d;
    ::operator delete(raw);
    if (!ok) { std::printf("005b1479_TEST FAIL\n"); return 1; }
    std::printf("005b1479_TEST PASS\n");
    return 0;
}