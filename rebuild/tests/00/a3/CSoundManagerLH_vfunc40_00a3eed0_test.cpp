#include <cstdio>
struct CEntry { long a, b; virtual void Set(long x, long y) { a = x; b = y; } };
struct CSoundManagerLH {
    CEntry* m_entry;
    CEntry* Lookup(long key, long name) { return (key + name) ? m_entry : 0; }
    virtual bool vfunc_40(long key, long name, long a, long b)
    {
        CEntry* entry = Lookup(key, name);
        if (!entry) return false;
        entry->Set(a, b);
        return true;
    }
};
int main()
{
    CEntry e; e.a = e.b = 0;
    CSoundManagerLH m; m.m_entry = &e;
    bool ok = !m.vfunc_40(0, 0, 1, 2) && e.a == 0;
    ok = ok && m.vfunc_40(1, 2, 7, 8) && e.a == 7 && e.b == 8;
    if (!ok) { std::printf("00a3eed0_TEST FAIL\n"); return 1; }
    std::printf("00a3eed0_TEST PASS\n");
    return 0;
}