// Behaviour test for CThingCreatureBase::IsBlocking (0x00664D00)
#include <cstdio>
struct CTCBase;
struct STCEntry { long m_kind; CTCBase* m_pTC; };
struct CTCMap {
    STCEntry* m_pFirst; STCEntry* m_pLast; STCEntry* m_pEnd;
    STCEntry* LowerBound(long kind) const
    {
        STCEntry* it = m_pFirst;
        while (it != m_pLast && it->m_kind < kind) ++it;
        return it;
    }
};
struct CThing {
    unsigned long m_tcMask[8];
    CTCMap m_tcs;
    unsigned long HasTC(long kind) const { return m_tcMask[kind >> 5] & (1UL << (kind & 31)); }
    CTCBase* GetTC(long kind) const
    {
        STCEntry* it = m_tcs.LowerBound(kind);
        if (it == m_tcs.m_pLast || kind < it->m_kind)
            it = m_tcs.m_pLast;
        return it->m_pTC;
    }
    template<class T> bool GetInterface(T** ppOut, long kind) const
    {
        if (HasTC(kind)) { *ppOut = (T*)GetTC(kind); return true; }
        return false;
    }
    void Clear() { for (int i = 0; i < 8; ++i) m_tcMask[i] = 0; }
    void Add(long kind) { m_tcMask[kind >> 5] |= 1UL << (kind & 31); }
};


struct CTCCombatBlocking { bool m_blocking; bool IsBlocking() const { return m_blocking; } };
struct CThingCreatureBase : CThing {
    bool IsBlocking() const
    {
        if (HasTC(0x30))
            return ((CTCCombatBlocking*)GetTC(0x30))->IsBlocking();
        return false;
    }
};
#define CHECK(c) if (!(c)) { std::printf("00664d00_TEST FAIL\n"); return 1; }
int main()
{
    CTCCombatBlocking b = { true }, end = { true };
    STCEntry e[3] = { { 0x10, (CTCBase*)&end }, { 0x30, (CTCBase*)&b }, { 0, (CTCBase*)&end } };
    CThingCreatureBase c; c.Clear(); c.m_tcs.m_pFirst = e; c.m_tcs.m_pLast = e + 2; c.m_tcs.m_pEnd = e + 3;
    CHECK(!c.IsBlocking());
    c.Add(0x30);
    CHECK(c.IsBlocking());
    b.m_blocking = false;
    CHECK(!c.IsBlocking());
    std::printf("00664d00_TEST PASS\n");
    return 0;
}