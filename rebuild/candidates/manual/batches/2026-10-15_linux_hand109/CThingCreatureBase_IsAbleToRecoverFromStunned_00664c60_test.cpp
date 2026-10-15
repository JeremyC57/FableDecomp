// Behaviour test for CThingCreatureBase::IsAbleToRecoverFromStunned (0x00664C60)
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


struct CTCCreatureModeQuery { long m_mode; bool IsInMode(long mode) const { return m_mode == mode; } };
struct CThingCreatureBase : CThing {
    bool IsAbleToRecoverFromStunned() const
    {
        CTCCreatureModeQuery* modes;
        if (GetInterface(&modes, 0x42) && modes->IsInMode(13))
            return false;
        return true;
    }
};
#define CHECK(c) if (!(c)) { std::printf("00664c60_TEST FAIL\n"); return 1; }
int main()
{
    CTCCreatureModeQuery q = { 13 }, end = { 0 };
    STCEntry e[2] = { { 0x42, (CTCBase*)&q }, { 0, (CTCBase*)&end } };
    CThingCreatureBase c; c.Clear(); c.m_tcs.m_pFirst = e; c.m_tcs.m_pLast = e + 1; c.m_tcs.m_pEnd = e + 2;
    CHECK(c.IsAbleToRecoverFromStunned());          // no component
    c.Add(0x42);
    CHECK(!c.IsAbleToRecoverFromStunned());         // mode 13: stunned for good
    q.m_mode = 2;
    CHECK(c.IsAbleToRecoverFromStunned());
    std::printf("00664c60_TEST PASS\n");
    return 0;
}
