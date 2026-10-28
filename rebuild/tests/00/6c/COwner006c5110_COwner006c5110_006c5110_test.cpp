// Behaviour test for 0x006C5110 COwner_006c5110::COwner_006c5110: the real constructor with same-TU stubs for the
// CBaseIntelligentPointer out-of-line members; checks the temporary is built, read, copied into the member and destroyed.
#include <stdio.h>
class CThing;
class CBaseObject;
static int g_ctors = 0, g_dtors = 0, g_gets = 0, g_sets = 0;
static CBaseObject* const kFakeItem = (CBaseObject*)0x1234;
struct CBaseIntelligentPointer {
    virtual void v00() {}
    CBaseObject* m_pItem;                               // +0x04
    CBaseIntelligentPointer();
    virtual ~CBaseIntelligentPointer();
    CBaseObject* GetPItem() const;
    void SetPItem(CBaseObject* item);
};
CBaseIntelligentPointer::CBaseIntelligentPointer() : m_pItem(kFakeItem) { ++g_ctors; }
CBaseIntelligentPointer::~CBaseIntelligentPointer() { ++g_dtors; m_pItem = 0; }
CBaseObject* CBaseIntelligentPointer::GetPItem() const { ++g_gets; return m_pItem; }
void CBaseIntelligentPointer::SetPItem(CBaseObject* item) { ++g_sets; m_pItem = item; }
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer { public:
    virtual void v00() {}
    CIntelligentPointer() { }
    CIntelligentPointer(const CIntelligentPointer& other) { SetPItem(other.GetPItem()); }
};
struct CThingPointerHolder_006c5110 {
    CIntelligentPointer<CThing> PThing;
    CThingPointerHolder_006c5110(const CIntelligentPointer<CThing>& thing = CIntelligentPointer<CThing>()) : PThing(thing) { }
};
struct COwner_006c5110 {
    CThingPointerHolder_006c5110 Holder;
    COwner_006c5110();
};
COwner_006c5110::COwner_006c5110() { }
int main()
{
    {
        COwner_006c5110 owner;
        if (g_ctors != 2 || g_dtors != 1 || g_gets != 1 || g_sets != 1) { printf("006c5110_TEST FAIL counts %d %d %d %d\n", g_ctors, g_dtors, g_gets, g_sets); return 1; }
        if (owner.Holder.PThing.m_pItem != kFakeItem) { printf("006c5110_TEST FAIL item\n"); return 1; }
    }
    if (g_dtors != 2) { printf("006c5110_TEST FAIL member dtor\n"); return 1; }
    printf("006c5110_TEST PASS\n");
    return 0;
}