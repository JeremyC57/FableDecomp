#include <cstdio>
struct E { unsigned char b[8]; };
struct V { E* f; E* l; E* e; static E* C(E* a, E* b, E* r) { for (long n = b - a; n > 0; --n) { *r = *a; ++a; ++r; } return r; } E* erase(E* a, E* b) { E* s = C(b, l, a); l = s; return a; } };
int main()
{
    E buf[3]; buf[0].b[0] = 1; buf[1].b[0] = 2; buf[2].b[0] = 3; V v; v.f = buf; v.l = buf + 3; v.e = buf + 3; E* r = v.erase(buf, buf + 1); if (r != buf || v.l != buf + 2 || buf[0].b[0] != 2 || buf[1].b[0] != 3) { std::printf("00ab3fb0_TEST FAIL\n"); return 1; }

    std::printf("00ab3fb0_TEST PASS\n");
    return 0;
}