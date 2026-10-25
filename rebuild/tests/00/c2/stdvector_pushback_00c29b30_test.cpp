#include <cstdio>
#include <cstring>
#include <cstdlib>

static int g_slow = 0; static unsigned long g_slowItem = 0;
struct CVec {
    long alloc; unsigned long* b; unsigned long* e; unsigned long* c;
    unsigned long GetSize() const { return b == 0 ? 0 : (unsigned long)(e - b); }
    unsigned long GetCapacity() const { return b == 0 ? 0 : (unsigned long)(c - b); }
    static unsigned long* Fill(unsigned long* w, const unsigned long& i) { *w = i; return w + 1; }
    void InsertRange(unsigned long* where, unsigned long n, const unsigned long& item) { ++g_slow; g_slowItem = item; }
    void push_back(const unsigned long& item) { if (GetSize() < GetCapacity()) e = Fill(e, item); else InsertRange(e, 1, item); }
};
int main()
{
    unsigned long buf[4]; CVec v; v.b = buf; v.e = buf; v.c = buf + 2;
    unsigned long a = 7, b2 = 9, c = 11;
    v.push_back(a); v.push_back(b2);
    if (v.e != buf + 2 || buf[0] != 7 || buf[1] != 9 || g_slow != 0) { std::printf("00c29b30_TEST FAIL\n"); return 1; }
    v.push_back(c); if (g_slow != 1 || g_slowItem != 11 || v.e != buf + 2) { std::printf("00c29b30_TEST FAIL\n"); return 1; }
    CVec n; n.b = 0; n.e = 0; n.c = 0; n.push_back(a); if (g_slow != 2) { std::printf("00c29b30_TEST FAIL\n"); return 1; }
    std::printf("00c29b30_TEST PASS\n");
    return 0;
}