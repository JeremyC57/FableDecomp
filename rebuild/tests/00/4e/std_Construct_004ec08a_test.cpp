// behaviour test for 0x004EC08A: the record is copy-constructed in place with both members
#include <cstdio>
#include <new>
struct R { long base; float a; long b; R(const R& o) : base(o.base), a(o.a), b(o.b) { } R() : base(1), a(2.5f), b(3) { } };
int main()
{
    R src; double raw[4]; R* p = (R*)raw;
    new (p) R(src);
    if (p->base != 1 || p->a != 2.5f || p->b != 3) { std::printf("004ec08a_TEST FAIL\n"); return 1; }
    std::printf("004ec08a_TEST PASS\n");
    return 0;
}