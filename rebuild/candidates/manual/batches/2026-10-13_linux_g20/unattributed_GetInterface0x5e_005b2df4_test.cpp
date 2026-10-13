#include <cstdio>
struct Bits { bool test(long b) { return b == 0x42; } }; struct E { long t; void* c; };
struct Map { E* f; E* l; E* LB(const long* k) { E* p = f; while (p != l && p->t < *k) ++p; return p; } };
struct X { unsigned char p0[0x20]; Bits b; unsigned char p1[0x23]; Map m; bool Get(void** out) { if (b.test(0x42)) { long k = 0x42; Map* mp = &m; E* it = mp->LB(&k); if (it == mp->l || 0x42 < it->t) it = mp->l; *out = it->c; return true; } return false; } };
int main()
{
    int v; E e[2] = { { 0x42, &v }, { 0, 0 } }; X x; x.m.f = e; x.m.l = e + 1; void* o = 0; if (!x.Get(&o) || o != &v) { std::printf("005b2df4_TEST FAIL\n"); return 1; }

    std::printf("005b2df4_TEST PASS\n");
    return 0;
}
