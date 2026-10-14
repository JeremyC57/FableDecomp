#include <cstdio>
#include <cstdlib>
#include <cstring>
static long g_deleted = 0; struct CEntry { virtual ~CEntry() { ++g_deleted; } };
struct CVec { CEntry** m_first; CEntry** m_last; CEntry** m_end; CEntry** Copy(CEntry** f, CEntry** l, CEntry** d) { if (f == l) return d; long n = (char*)l - (char*)f; return (CEntry**)((char*)std::memmove(d, f, n) + n); } void erase(CEntry** f, CEntry** l) { CEntry** k = Copy(l, m_last, f); m_last = k; }
    void DeleteAll() { for (unsigned long i = 0; i < (unsigned long)(m_last - m_first); ++i) { if (m_first[i]) delete m_first[i]; } erase(m_first, m_last); } };

int main() {
    CEntry* p[3] = { new CEntry, 0, new CEntry }; CVec v; v.m_first = p; v.m_last = p + 3; v.m_end = p + 3; v.DeleteAll(); if (g_deleted != 2 || v.m_last != p) { std::printf("00b29680_TEST FAIL\n"); return 1; }
    std::printf("00b29680_TEST PASS\n"); return 0;
}
