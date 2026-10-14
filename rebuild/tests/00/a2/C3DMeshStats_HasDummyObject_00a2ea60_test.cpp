#include <cstdio>
struct HP { float m[12]; long bone; };
struct E { unsigned long first; HP second; };
struct Map { bool c; E* el; unsigned long n; bool d;
    E* LB(const unsigned long& k) { E* p = el; while (p != el + n && p->first < k) ++p; return p; }
    E* End() { return el + n; }
    E* Find(const unsigned long& k) { E* it = LB(k); if (it == End() || k < it->first) it = End(); return it; } };
struct M { virtual void v00() {} unsigned char pad[0x44]; Map hp; bool Has(unsigned long k) { return hp.Find(k) != hp.End(); } };
int main()
{
    E es[4] = { { 1 }, { 3 }, { 3 }, { 7 } }; M m; m.hp.el = es; m.hp.n = 4;
    if (!m.Has(3) || m.Has(2) || m.Has(9) || !m.Has(7)) { std::printf("00a2ea60_TEST FAIL\n"); return 1; }
    std::printf("00a2ea60_TEST PASS\n");
    return 0;
}