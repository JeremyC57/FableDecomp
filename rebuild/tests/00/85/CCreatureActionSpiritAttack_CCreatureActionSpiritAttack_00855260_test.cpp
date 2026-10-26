// behaviour test for 0x00855260 CCreatureAction_SpiritAttack::CCreatureAction_SpiritAttack(const CCreatureAction_SpiritAttack&): the base part is copied first, then each intelligent pointer observes the same pointee as the source's (not the source pointer object itself), and the source is left untouched
#include <cstdio>
class CBaseObject { public: int Id; };
static int gOrder = 0, gBaseOrder = 0, gPtrOrder = 0;
struct CBaseIntelligentPointer {
    CBaseObject* Item;
    CBaseIntelligentPointer() : Item(0) { }
    CBaseObject* GetPItem() const { return Item; }
    void SetPItem(CBaseObject* item) { Item = item; if (!gPtrOrder) gPtrOrder = ++gOrder; }
};
struct CPtr : CBaseIntelligentPointer {
    CPtr() { }
    CPtr(const CPtr& other) : CBaseIntelligentPointer() { SetPItem(other.GetPItem()); }
};
struct CBase {
    int State;
    CBase() : State(0) { }
    CBase(const CBase& o) : State(o.State) { gBaseOrder = ++gOrder; }
};
struct CAction : CBase {
    CPtr P1, P2;
    CAction() { }
    CAction(const CAction& o) : CBase(o), P1(o.P1), P2(o.P2) { }   // the function under test (member-wise)
};
int main()
{
    int fail = 0;
    CBaseObject a, b; a.Id = 1; b.Id = 2;
    CAction src; src.State = 5; src.P1.SetPItem(&a); src.P2.SetPItem(&b);
    gOrder = 0; gPtrOrder = 0;
    CAction copy(src);
    if (copy.State != 5) fail |= 1;
    if (copy.P1.GetPItem() != &a || copy.P2.GetPItem() != &b) fail |= 2;
    if (gBaseOrder != 1 || gPtrOrder != 2) fail |= 4;
    if (src.P1.GetPItem() != &a || src.P2.GetPItem() != &b || src.State != 5) fail |= 8;
    if (fail) { std::printf("00855260_TEST FAIL %d\n", fail); return 1; }
    std::printf("00855260_TEST PASS\n");
    return 0;
}