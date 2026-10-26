// Test for 0x006A8A80 CThingFilter_IsOnFire::operator(): true only when the thing holds component 0x6d and that
// component reports IsOnFire.
#include <stdio.h>
#include <string.h>
class CTCBase;

struct STCEntry {                               // one CVectorMap<ETCInterfaceType, CTCBase*> element
    long     m_kind;
    CTCBase* m_pTC;
};

struct CTCMap {                                 // the thing's component map at +0x44: a sorted vector of pairs
    STCEntry* m_pFirst;
    STCEntry* m_pLast;
    STCEntry* m_pEnd;
    STCEntry* LowerBound(const unsigned long& kind) const;   // 0x004365B0 (CVectorMap<ETCInterfaceType,...>::LowerBound)
    CTCBase* Find(long kind) const                 // the inlined map find: lower bound, then end when the key is absent
    {
        STCEntry* it = LowerBound((unsigned long)kind);
        if (it == m_pLast || kind < it->m_kind)
            it = m_pLast;
        return it->m_pTC;
    }
};

class CThing {
public:
    unsigned char  _pad00[0x20];
    unsigned long  m_tcMask[8];                 // +0x20 which component kinds the thing holds
    unsigned char  _pad40[4];
    CTCMap         m_tcs;                       // +0x44
    unsigned long HasTC(long kind) const { return m_tcMask[kind >> 5] & (1UL << (kind & 31)); }
    CTCBase* GetTC(long kind) const { return m_tcs.Find(kind); }
};

class CTCFlammable {                            // component kind 0x6d
public:
    bool IsOnFire() const;                      // 0x00780400
};

class CThingFilter_IsOnFire {
public:
    bool operator()(const CThing& thing) const;
};


struct SFlammable { long burning; };
bool CTCFlammable::IsOnFire() const { return ((const SFlammable*)this)->burning != 0; }
STCEntry* CTCMap::LowerBound(const unsigned long& kind) const
{
    STCEntry* it = m_pFirst;
    while (it != m_pLast && (unsigned long)it->m_kind < kind) ++it;
    return it;
}
int main()
{
    SFlammable on = { 1 }, off = { 0 }, endv = { 1 };
    STCEntry e[2] = { { 0x6d, (CTCBase*)&on }, { 0, (CTCBase*)&endv } };
    CThing t; memset(&t, 0, sizeof t);
    t.m_tcs.m_pFirst = e; t.m_tcs.m_pLast = e + 1; t.m_tcs.m_pEnd = e + 2;
    CThingFilter_IsOnFire f;
    if (f(t)) return 1;                                         // no 0x6d bit
    t.m_tcMask[0x6d >> 5] |= 1UL << (0x6d & 31);
    if (!f(t)) return 2;                                        // burning
    e[0].m_pTC = (CTCBase*)&off;
    if (f(t)) return 3;                                         // not burning
    puts("006a8a80_TEST PASS");
    return 0;
}
