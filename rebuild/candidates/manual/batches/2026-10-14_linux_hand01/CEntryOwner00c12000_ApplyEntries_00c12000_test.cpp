#include <cstdio>
struct CEntryOwner; static int g_applied = 0; static CEntryOwner* g_owner = 0;
struct CEntry { void* m_vptr; void* m_pTarget; void Apply(CEntryOwner* o) { ++g_applied; g_owner = o; } };
struct CCheckedArray { CEntry* m_pData; unsigned long m_count; CEntry* Get(unsigned long i) { return &m_pData[i]; } };
struct CEntryOwner { unsigned char _pad[0x224]; CCheckedArray m_entries;
    void ApplyEntries() { for (unsigned long i = 0; i < m_entries.m_count; ++i) { CEntry* e = m_entries.Get(i); if (e->m_pTarget) e->Apply(this); } } };
int main()
{
    CEntry e[3]; e[0].m_pTarget = (void*)1; e[1].m_pTarget = 0; e[2].m_pTarget = (void*)1;
    CEntryOwner o; o.m_entries.m_pData = e; o.m_entries.m_count = 3;
    o.ApplyEntries();
    if (g_applied != 2 || g_owner != &o) { std::printf("00c12000_TEST FAIL\n"); return 1; }
    std::printf("00c12000_TEST PASS\n"); return 0;
}
