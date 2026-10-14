#include <cstdio>
#include <cstdlib>
#include <cstring>
struct SPair { void* m_first; void* m_second; }; struct CVec { SPair* m_first; SPair* m_last; SPair* m_end; unsigned long size() const { return m_last - m_first; } };
struct CPairList { long m_0; CVec m_pairs; bool Contains(void* item) { for (unsigned long i = 0; i < m_pairs.size(); ++i) { if (m_pairs.m_first[i].m_first == item || m_pairs.m_first[i].m_second == item) return true; } return false; } };

int main() {
    CPairList l; SPair p[3]; int a, b, c, d; p[0].m_first = &a; p[0].m_second = &b; p[1].m_first = &c; p[1].m_second = &c; p[2].m_first = 0; p[2].m_second = &d; l.m_pairs.m_first = p; l.m_pairs.m_last = p + 3;
    if (!l.Contains(&a) || !l.Contains(&b) || !l.Contains(&c) || !l.Contains(&d) || l.Contains(&l)) { std::printf("00ac7990_TEST FAIL\n"); return 1; } l.m_pairs.m_last = p; if (l.Contains(&a)) { std::printf("00ac7990_TEST FAIL\n"); return 1; }
    std::printf("00ac7990_TEST PASS\n"); return 0;
}
