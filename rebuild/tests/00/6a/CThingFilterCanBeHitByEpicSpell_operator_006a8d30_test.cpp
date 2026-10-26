// Behaviour test for CThingFilter_CanBeHitByEpicSpell::operator() (0x006A8D30)
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


struct CTypedObject { long m_type; long GetType() const { return m_type; } };
struct CTCEpicSpellTarget { CTypedObject* m_object; CTypedObject* GetObject() const { return m_object; } };
class CThingFilter_CanBeHitByEpicSpell { public:
    bool operator()(const CThing& thing) const
    {
        if (thing.HasTC(0x31)) {
            long type = ((CTCEpicSpellTarget*)thing.GetTC(0x31))->GetObject()->GetType();
            if (type == 0x41 || type == 0x40)
                return false;
        }
        return true;
    }
};
#define CHECK(c) if (!(c)) { std::printf("006a8d30_TEST FAIL\n"); return 1; }
int main()
{
    CTypedObject o = { 0x41 };
    CTCEpicSpellTarget c = { &o }, end = { &o };
    STCEntry e[2] = { { 0x31, (CTCBase*)&c }, { 0, (CTCBase*)&end } };
    CThing t; t.Clear(); t.m_tcs.m_pFirst = e; t.m_tcs.m_pLast = e + 1; t.m_tcs.m_pEnd = e + 2;
    CThingFilter_CanBeHitByEpicSpell f;
    CHECK(f(t));
    t.Add(0x31);
    CHECK(!f(t));
    o.m_type = 0x40;
    CHECK(!f(t));
    o.m_type = 0x10;
    CHECK(f(t));
    std::printf("006a8d30_TEST PASS\n");
    return 0;
}