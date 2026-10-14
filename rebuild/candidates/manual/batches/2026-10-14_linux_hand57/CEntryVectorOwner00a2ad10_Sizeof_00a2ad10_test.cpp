#include <cstdio>
#include <cstdlib>
#include <cstring>
struct CEntry { unsigned long m_size; virtual void v000() { } virtual void Release() { } virtual unsigned long Sizeof() { return m_size; } }; struct CVec { CEntry** m_first; CEntry** m_last; CEntry** m_end; unsigned long size() const { return m_last - m_first; } };
struct COwner { CVec m_entries; unsigned long Sizeof() const { unsigned long t = 12 + m_entries.size() * 4; for (CEntry** e = m_entries.m_first; e != m_entries.m_last; ++e) t += *e ? (*e)->Sizeof() + 4 : 4; return t; } };

int main() {
    CEntry e1, e2; e1.m_size = 10; e2.m_size = 20; CEntry* p[3] = { &e1, 0, &e2 }; COwner o; o.m_entries.m_first = p; o.m_entries.m_last = p + 3; if (o.Sizeof() != 12 + 12 + 14 + 4 + 24) { std::printf("00a2ad10_TEST FAIL\n"); return 1; } o.m_entries.m_last = p; if (o.Sizeof() != 12) { std::printf("00a2ad10_TEST FAIL\n"); return 1; }
    std::printf("00a2ad10_TEST PASS\n"); return 0;
}
