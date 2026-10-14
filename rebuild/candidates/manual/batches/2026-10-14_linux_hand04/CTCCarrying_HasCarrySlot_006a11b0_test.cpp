#include <cstdio>
struct E { int first; long second; };
struct Map { E* f; E* l; E* e; bool c, d;
    E* LB(const int& k) { E* p = f; while (p != l && p->first < k) ++p; return p; }
    E* End() { return l; }
    E* Find(const int& k) { E* it = LB(k); if (it == End() || k < it->first) it = End(); return it; } };
struct V { virtual void v00() {} unsigned char pad[8]; Map m; bool Has(int t) { return m.Find(t) != m.End(); } };
int main()
{
    E es[3] = { { 1, 0 }, { 3, 0 }, { 5, 0 } }; V v; v.m.f = es; v.m.l = es + 3; v.m.e = es + 3;
    if (!v.Has(3) || v.Has(2) || v.Has(6) || !v.Has(1)) { std::printf("006a11b0_TEST FAIL\n"); return 1; }
    std::printf("006a11b0_TEST PASS\n");
    return 0;
}
