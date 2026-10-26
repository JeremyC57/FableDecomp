// Behaviour test for CTCCarrying::SetAsDrawable (0x006A1370): every non-null, non-dead slot thing gets the flag; each
// slot pointer is copied into a local (constructed and destroyed once per slot).
#include <cstdio>
static int g_ctors = 0, g_dtors = 0;
class CThing { public:
    int Calls; bool Last;
    virtual void SetAsDrawable(bool drawable) { ++Calls; Last = drawable; }
    unsigned char Dead : 1;
    unsigned char _91_1 : 7;
};
struct CBaseIntelligentPointer {
    virtual void v00() {}
    CThing* m_pItem;
    CBaseIntelligentPointer() : m_pItem(0) { ++g_ctors; }
    ~CBaseIntelligentPointer() { ++g_dtors; }
    CThing* GetPItem() const { return m_pItem; }
    void SetPItem(CThing* item) { m_pItem = item; }
};
struct CIntelligentPointer : CBaseIntelligentPointer {
    virtual void v00() {}
    CIntelligentPointer() {}
    CIntelligentPointer(const CIntelligentPointer& other) : CBaseIntelligentPointer() { SetPItem(other.GetPItem()); }
};
struct SCarrySlotEntry {
    long                first;
    CIntelligentPointer second;
};
struct CCarrySlotMap {
    SCarrySlotEntry* m_pFirst;
    SCarrySlotEntry* m_pLast;
    SCarrySlotEntry* m_pEnd;
    SCarrySlotEntry* begin() const { return m_pFirst; }
    unsigned long size() const { return m_pLast - m_pFirst; }
    SCarrySlotEntry& operator[](unsigned long n) const { return *(begin() + n); }
};
class CTCCarrying { public:
    virtual void v00() {}
    CCarrySlotMap CarrySlots;
    void SetAsDrawable(bool drawable);
};

void CTCCarrying::SetAsDrawable(bool drawable)
{
    for (unsigned long i = 0; i < CarrySlots.size(); ++i) {
        CIntelligentPointer thing(CarrySlots[i].second);
        if (thing.GetPItem() && !thing.GetPItem()->Dead)
            thing.GetPItem()->SetAsDrawable(drawable);
    }
}

int main()
{
    int bad = 0;
    CThing a, b, c;
    a.Calls = b.Calls = c.Calls = 0; a.Dead = 0; b.Dead = 1; c.Dead = 0; a._91_1 = b._91_1 = c._91_1 = 0;
    SCarrySlotEntry slots[4];
    slots[0].second.SetPItem(&a); slots[1].second.SetPItem(0); slots[2].second.SetPItem(&b); slots[3].second.SetPItem(&c);
    CTCCarrying carrying; carrying.CarrySlots.m_pFirst = slots; carrying.CarrySlots.m_pLast = slots + 4; carrying.CarrySlots.m_pEnd = slots + 4;
    int c0 = g_ctors, d0 = g_dtors;
    carrying.SetAsDrawable(true);
    if (a.Calls != 1 || !a.Last || b.Calls != 0 || c.Calls != 1 || !c.Last) bad |= 1;
    if (g_ctors - c0 != 4 || g_dtors - d0 != 4) bad |= 2;
    carrying.SetAsDrawable(false);
    if (a.Calls != 2 || a.Last || c.Last || b.Calls != 0) bad |= 4;
    carrying.CarrySlots.m_pLast = slots;
    carrying.SetAsDrawable(true);
    if (a.Calls != 2) bad |= 8;
    if (bad) { std::printf("006a1370_TEST FAIL\n"); return 1; }
    std::printf("006a1370_TEST PASS\n");
    return 0;
}