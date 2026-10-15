// Behaviour test for CThingFilter_IsInFaction::operator() (0x006A8880)
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


struct CTCFactionMembership { long m_faction; bool IsMember(long faction) const { return m_faction == faction; } };
struct CThingFilter_IsInFaction {
    long m_faction;
    bool operator()(const CThing& thing) const
    {
        CTCFactionMembership* membership;
        if (thing.GetInterface(&membership, 0x57))
            return membership->IsMember(m_faction);
        return false;
    }
};
#define CHECK(c) if (!(c)) { std::printf("006a8880_TEST FAIL\n"); return 1; }
int main()
{
    CTCFactionMembership m = { 7 }, end = { 0 };
    STCEntry e[2] = { { 0x57, (CTCBase*)&m }, { 0, (CTCBase*)&end } };
    CThing t; t.Clear(); t.m_tcs.m_pFirst = e; t.m_tcs.m_pLast = e + 1; t.m_tcs.m_pEnd = e + 2;
    CThingFilter_IsInFaction f = { 7 };
    CHECK(!f(t));
    t.Add(0x57);
    CHECK(f(t));
    f.m_faction = 3;
    CHECK(!f(t));
    std::printf("006a8880_TEST PASS\n");
    return 0;
}
