// Behaviour test for 0x0084C540 CCreatureAction_DowseFire copy constructor: base copied, both pointers re-pointed at the
// source items through SetPItem, the fire number copied.
#include <cstdio>
#include <cstring>

struct CBaseObject { int id; };
struct CBaseIntelligentPointer {
    CBaseObject* m_pItem; int m_sets;
    CBaseIntelligentPointer() : m_pItem(0), m_sets(0) { }
    CBaseObject* GetPItem() const { return m_pItem; }
    void SetPItem(CBaseObject* p) { m_pItem = p; ++m_sets; }
};
template <class T> struct CIntelligentPointer : CBaseIntelligentPointer {
    CIntelligentPointer() { }
    CIntelligentPointer(const CIntelligentPointer& other) : CBaseIntelligentPointer() { SetPItem(other.GetPItem()); }
};
struct CCreatureActionBase {
    int m_baseValue; int m_copies;
    CCreatureActionBase() : m_baseValue(0), m_copies(0) { }
    CCreatureActionBase(const CCreatureActionBase& o) : m_baseValue(o.m_baseValue), m_copies(o.m_copies + 1) { }
};
struct CThing; struct CThingAICreature; struct CThingBuilding;
struct CCreatureAction_DowseFire : CCreatureActionBase {
    CIntelligentPointer<CThingAICreature> PAICreature; CIntelligentPointer<CThingBuilding> PBuilding; unsigned long FireNumber;
    CCreatureAction_DowseFire() : FireNumber(0) { }
    CCreatureAction_DowseFire(const CCreatureAction_DowseFire& o)
        : CCreatureActionBase(o), PAICreature(o.PAICreature), PBuilding(o.PBuilding), FireNumber(o.FireNumber) { }
};
int main()
{
    CBaseObject ai = { 1 }, bld = { 2 };
    CCreatureAction_DowseFire src; src.m_baseValue = 9; src.PAICreature.SetPItem(&ai); src.PBuilding.SetPItem(&bld); src.FireNumber = 4;
    CCreatureAction_DowseFire c(src);
    if (c.m_baseValue != 9 || c.m_copies != 1 || c.PAICreature.m_pItem != &ai || c.PAICreature.m_sets != 1 ||
        c.PBuilding.m_pItem != &bld || c.FireNumber != 4) { std::printf("0084c540_TEST FAIL\n"); return 1; }
    std::printf("0084c540_TEST PASS\n");
    return 0;
}
