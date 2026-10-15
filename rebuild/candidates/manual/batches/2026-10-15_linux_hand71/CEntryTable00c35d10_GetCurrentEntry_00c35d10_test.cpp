#include <cstdio>

struct CEntry { int id; };

struct CEntryTable {
    CEntry** m_pEntries; unsigned long _unused04; void* m_pLive; unsigned long _unused0c; long m_current;
    CEntry* GetCurrentEntry() const
    {
        if (m_pLive)
            return m_pEntries[m_current];
        return 0;
    }
};

int main()
{
    CEntry a, b; CEntry* list[2] = { &a, &b }; CEntryTable t; t.m_pEntries = list; t.m_pLive = 0; t.m_current = 1;
    if (t.GetCurrentEntry() != 0) { std::printf("00c35d10_TEST FAIL\n"); return 1; }
    t.m_pLive = &t;
    if (t.GetCurrentEntry() != &b) { std::printf("00c35d10_TEST FAIL\n"); return 1; }
    t.m_current = 0;
    if (t.GetCurrentEntry() != &a) { std::printf("00c35d10_TEST FAIL\n"); return 1; }
    std::printf("00c35d10_TEST PASS\n");
    return 0;
}
