#include <cstdio>
struct E { unsigned long first; unsigned char second[8]; };
struct Map { E* f; E* l; E* e; bool c, d;
    E* LB(const unsigned long& k) { E* p = f; while (p != l && p->first < k) ++p; return p; }
    E* End() { return l; }
    E* Find(const unsigned long& k) { E* it = LB(k); if (it == End() || k < it->first) it = End(); return it; } };
struct U { unsigned char pad[0x78]; Map m; void* Find(unsigned long k) { E* it = m.Find(k); if (it == m.End()) return 0; return it->second; } };
int main()
{
    E es[3] = { { 1 }, { 3 }, { 5 } }; U u; u.m.f = es; u.m.l = es + 3; u.m.e = es + 3;
    if (u.Find(3) != es[1].second || u.Find(2) != 0 || u.Find(6) != 0 || u.Find(5) != es[2].second) { std::printf("009ad350_TEST FAIL\n"); return 1; }
    std::printf("009ad350_TEST PASS\n");
    return 0;
}
