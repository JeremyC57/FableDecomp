// Behaviour test for CThingFilter_IsOccupiableOfType::operator() (0x006A89E0)
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


class CTCSinglePersonOccupiable { public: unsigned long m_types; bool IsOfType(unsigned long typeMask) const { return (m_types & typeMask) != 0; } };
struct CThingFilter_IsOccupiableOfType {
    bool operator()(const CThing& thing) const
    {
        CTCSinglePersonOccupiable* occupiable;
        if (thing.GetInterface(&occupiable, 0xac) && occupiable->IsOfType(4))
            return true;
        return false;
    }
};
#define CHECK(c) if (!(c)) { std::printf("006a89e0_TEST FAIL\n"); return 1; }
int main()
{
    CTCSinglePersonOccupiable o = { 4 }, end = { 0 };
    STCEntry e[2] = { { 0xac, (CTCBase*)&o }, { 0, (CTCBase*)&end } };
    CThing t; t.Clear(); t.m_tcs.m_pFirst = e; t.m_tcs.m_pLast = e + 1; t.m_tcs.m_pEnd = e + 2;
    CThingFilter_IsOccupiableOfType f;
    CHECK(!f(t));
    t.Add(0xac);
    CHECK(f(t));
    o.m_types = 2;
    CHECK(!f(t));
    std::printf("006a89e0_TEST PASS\n");
    return 0;
}