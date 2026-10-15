// Behaviour test for CThingCreatureBase::OnKill (0x00664370)
#include <cstdio>
struct CTCBase { int m_cleared; };
struct STCEntry { long m_kind; CTCBase* m_pTC; };
struct CTCMap {
    STCEntry* m_pFirst; STCEntry* m_pLast; STCEntry* m_pEnd;
    STCEntry* LowerBound(const unsigned long& kind)
    {
        STCEntry* it = m_pFirst;
        while (it != m_pLast && it->m_kind < (long)kind) ++it;
        return it;
    }
    CTCBase* Find(long kind)
    {
        STCEntry* it = LowerBound((unsigned long)kind);
        if (it == m_pLast || kind < it->m_kind)
            it = m_pLast;
        return it->m_pTC;
    }
};
static int g_baseKills = 0;
struct CThing {
    unsigned long m_tcMask[8];
    CTCMap m_tcs;
    unsigned long HasTC(long kind) const { return m_tcMask[kind >> 5] & (1UL << (kind & 31)); }
    CTCBase* GetTC(long kind) { return m_tcs.Find(kind); }
    void OnKill() { ++g_baseKills; }
};
struct CThingCreatureBase : CThing {
    void OnKill()
    {
        if (HasTC(0x31))
            GetTC(0x31)->m_cleared++;
        CThing::OnKill();
    }
};
int main()
{
    CTCBase a = { 0 }, b = { 0 }, sentinel = { 0 };
    STCEntry entries[3] = { { 0x10, &a }, { 0x31, &b }, { 0, &sentinel } };
    CThingCreatureBase c;
    for (int i = 0; i < 8; ++i) c.m_tcMask[i] = 0;
    c.m_tcs.m_pFirst = entries; c.m_tcs.m_pLast = entries + 2; c.m_tcs.m_pEnd = entries + 3;
    c.OnKill();                                     // bit clear: only the base runs
    if (g_baseKills != 1 || b.m_cleared != 0) { std::printf("00664370_TEST FAIL\n"); return 1; }
    c.m_tcMask[1] = 0x20000;                        // kind 0x31 present
    c.OnKill();
    if (g_baseKills != 2 || b.m_cleared != 1 || a.m_cleared != 0) { std::printf("00664370_TEST FAIL\n"); return 1; }
    std::printf("00664370_TEST PASS\n");
    return 0;
}
