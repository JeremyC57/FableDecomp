#include <cstdio>
struct E {}; struct I { float G(E* e) { return e ? 0.5f : 0.0f; } };
struct O { unsigned char pad[0x24]; I* i; float G(E* e) { return i->G(e); } };
int main()
{
    I i; O o; o.i = &i; E e; if (o.G(&e) != 0.5f) { std::printf("00687d10_TEST FAIL\n"); return 1; }

    std::printf("00687d10_TEST PASS\n");
    return 0;
}