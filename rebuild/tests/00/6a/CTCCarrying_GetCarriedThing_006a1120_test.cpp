// Behaviour test for 006a1120: src/006a1120.cpp copied verbatim; the carry-slot map and intelligent pointers stubbed so
// the first carried thing with the wanted def index is returned (empty slots skipped) and 0 when none matches.
#include <cstdio>
#include <cstring>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
// ---- source (verbatim) ----
// CTCCarrying::GetCarriedThing(long defIndex) -- the first carried thing whose def index (+0x8c) matches, or null
// Retry 10b (getsw sweep, FIXES B-15): the def index is read through an inline getter `GetDefIndex()`; the plain member read gives the parked register/scheduling difference.
//
// Retail 0x006A1120, 129 bytes (`ret 4`). Callee: CBaseIntelligentPointer::GetPItem @ 0x00A01B50
// (twice per slot, a third time for the hit).
// Layout: CarrySlots at +0xc as in the landed 0x006A1060 / OnKill 0x006A0FF0 models; the thing's
// def index is the unsigned short at +0x8c (`movzx`).
// Shape: the IsCarryingThing 0x006A10A0 loop returning `GetPItem()` of the hit (pointer address in
// ebx, offset in ebp) and `xor eax,eax` after the loop.
struct CThing {
    unsigned char  _pad00[0x8c];
    unsigned short m_defIndex;                      // +0x8c (the thing's def/type index)
    unsigned short GetDefIndex() const { return m_defIndex; }
};

struct CBaseIntelligentPointer {                    // 8 bytes: vptr, item block
    void* m_vtable;
    void* m_pBlock;
    CThing* GetPItem() const;                       // 0x00A01B50
};

struct SCarrySlotEntry {                            // pair<long, CIntelligentPointer<CThing>>, 12 bytes
    long                    first;
    CBaseIntelligentPointer second;
};

struct CCarrySlotMap {                              // retail CVectorMap<long,CIntelligentPointer<CThing>>
    SCarrySlotEntry* m_pFirst;
    SCarrySlotEntry* m_pLast;
    SCarrySlotEntry* m_pEnd;
    bool             m_compare;
    bool             m_dirty;
    SCarrySlotEntry* begin() const { return m_pFirst; }
    unsigned long size() const { return m_pLast - m_pFirst; }
    SCarrySlotEntry& operator[](unsigned long n) const { return *(begin() + n); }
};

struct CTCCarrying {
    virtual void v00();
    unsigned char _pad_0x04[0x0c - 0x04];           // +0x04 .. +0x0c: CTCBase state
    CCarrySlotMap CarrySlots;                       // +0x0c
    CThing* GetCarriedThing(long defIndex);
};

CThing* CTCCarrying::GetCarriedThing(long defIndex)
{
    for (unsigned long i = 0; i < CarrySlots.size(); ++i)
    {
        CBaseIntelligentPointer& carried = CarrySlots[i].second;
        if (carried.GetPItem() && carried.GetPItem()->GetDefIndex() == defIndex)
            return carried.GetPItem();
    }
    return 0;
}
// ---- end source ----
void CTCCarrying::v00() {}
CThing* CBaseIntelligentPointer::GetPItem() const { return (CThing*)m_pBlock; }
int main()
{
    static CThing a, b, c; memset(&a, 0, sizeof(a)); memset(&b, 0, sizeof(b)); memset(&c, 0, sizeof(c));
    a.m_defIndex = 5; b.m_defIndex = 9; c.m_defIndex = 9;
    SCarrySlotEntry slots[4];
    memset(slots, 0, sizeof(slots));
    slots[0].second.m_pBlock = &a; slots[1].second.m_pBlock = 0; slots[2].second.m_pBlock = &b; slots[3].second.m_pBlock = &c;
    static unsigned char raw[sizeof(CTCCarrying)]; memset(raw, 0, sizeof(raw));
    CTCCarrying* t = (CTCCarrying*)raw; t->CarrySlots.m_pFirst = slots; t->CarrySlots.m_pLast = slots + 4;
    CHECK(t->GetCarriedThing(9) == &b);
    CHECK(t->GetCarriedThing(5) == &a);
    CHECK(t->GetCarriedThing(7) == 0);
    t->CarrySlots.m_pLast = slots; CHECK(t->GetCarriedThing(5) == 0);
    if (fails == 0) printf("006a1120_TEST PASS\n"); else printf("006a1120_TEST FAIL\n");
    return fails;
}