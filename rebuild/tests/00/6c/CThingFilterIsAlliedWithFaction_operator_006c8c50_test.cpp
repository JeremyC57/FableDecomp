// Behaviour test for CThingFilter_IsAlliedWithFaction::operator() (0x006C8C50)
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


struct CTCEnemy { long m_faction; bool IsAlliedWithFaction(long faction) const { return m_faction == faction; } };
struct CThingFilter_IsAlliedWithFaction {
    long m_faction;
    bool operator()(const CThing& thing) const
    {
        CTCEnemy* enemy;
        if (thing.GetInterface(&enemy, 0x49))
            return enemy->IsAlliedWithFaction(m_faction);
        return false;
    }
};
#define CHECK(c) if (!(c)) { std::printf("006c8c50_TEST FAIL\n"); return 1; }
int main()
{
    CTCEnemy m = { 4 }, end = { 0 };
    STCEntry e[2] = { { 0x49, (CTCBase*)&m }, { 0, (CTCBase*)&end } };
    CThing t; t.Clear(); t.m_tcs.m_pFirst = e; t.m_tcs.m_pLast = e + 1; t.m_tcs.m_pEnd = e + 2;
    CThingFilter_IsAlliedWithFaction f = { 4 };
    CHECK(!f(t));
    t.Add(0x49);
    CHECK(f(t));
    f.m_faction = 5;
    CHECK(!f(t));
    std::printf("006c8c50_TEST PASS\n");
    return 0;
}