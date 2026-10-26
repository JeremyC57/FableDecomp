// Behaviour test for CThingFilter_IsPerceivingThing::operator() (0x006A80E0)
#include <cstdio>
class CTCBase;
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
class CThing { public:
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


class CTCPerceiveThing { public: const CThing* m_perceived; const CThing* GetPerceivedThing() const { return m_perceived; } };
class CThingFilter_IsPerceivingThing { public:
    const CThing* m_thing;
    bool operator()(const CThing& thing) const
    {
        CTCPerceiveThing* perceive = 0;          // retail leaves it uninitialised (never read on a thing without the component in practice)
        if (!thing.GetInterface(&perceive, 0x34))
            return false;                        // test-only guard: the retail body calls through the stale pointer
        return perceive->GetPerceivedThing() == m_thing;
    }
};
#define CHECK(c) if (!(c)) { std::printf("006a80e0_TEST FAIL\n"); return 1; }
int main()
{
    CThing other; other.Clear();
    CTCPerceiveThing p = { &other }, end = { 0 };
    STCEntry e[2] = { { 0x34, (CTCBase*)&p }, { 0, (CTCBase*)&end } };
    CThing t; t.Clear(); t.m_tcs.m_pFirst = e; t.m_tcs.m_pLast = e + 1; t.m_tcs.m_pEnd = e + 2;
    CThingFilter_IsPerceivingThing f = { &other };
    t.Add(0x34);
    CHECK(f(t));
    f.m_thing = &t;
    CHECK(!f(t));
    p.m_perceived = &t;
    CHECK(f(t));
    std::printf("006a80e0_TEST PASS\n");
    return 0;
}