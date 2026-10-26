// Behaviour test for 00658ec3: body copied from src/00658ec3.cpp (CTCVillage::BuildingContainsCorrectVillagers), callees stubbed.
#include <cstdio>
#include <cstring>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
class C3DVector { public: float X, Y, Z; };
class CThingBuilding;
struct CBaseIntelligentPointer {
    void* _vfptr;
    void* m_pItem;
};
template <class T> class CIntelligentPointer { public:
    void* _vfptr;
    T*    m_pItem;
    T* GetPItem() const { return m_pItem; }
};
class CThingAICreature {
public:
    unsigned char _pad_0x00[0x168];
    CIntelligentPointer<CThingBuilding> PHome;
    C3DVector position;
    bool alive;
    C3DVector* GetPosition() { return &position; }
    CThingBuilding* GetHome() const { return PHome.GetPItem(); }
};
static int g_insideCalls = 0;
class CThingBuilding {
public:
    float minX, maxX; bool alive;
    bool IsPointInside(const C3DVector& pos) { ++g_insideCalls; return pos.X >= minX && pos.X <= maxX; }
};
bool __fastcall IS_THING_ALIVE(const CIntelligentPointer<CThingAICreature>& thing) { return thing.m_pItem != 0 && thing.m_pItem->alive; }
bool __fastcall IS_THING_ALIVE(CThingBuilding* const& thing) { return thing != 0 && thing->alive; }
struct SVillagerNode {
    SVillagerNode* _M_next;
    SVillagerNode* _M_prev;
    CIntelligentPointer<CThingAICreature> _M_data;
};
class CTCVillage { public:
    unsigned char  _pad_0x00[0x2c];
    SVillagerNode* VillagePeople;
    bool BuildingContainsCorrectVillagers(CThingBuilding* building);
};
bool CTCVillage::BuildingContainsCorrectVillagers(CThingBuilding* building)
{
    if (!IS_THING_ALIVE(building))
        return false;
    for (SVillagerNode* node = VillagePeople->_M_next; node != VillagePeople; node = node->_M_next) {
        CIntelligentPointer<CThingAICreature>& villager = node->_M_data;
        if (IS_THING_ALIVE(villager)) {
            CThingBuilding* home = villager.GetPItem()->GetHome();
            bool inside = building->IsPointInside(*villager.GetPItem()->GetPosition());
            if (home == building) {
                if (!inside)
                    return false;
            } else if (inside) {
                return false;
            }
        }
    }
    return true;
}
int main()
{
    static CThingBuilding house, other; house.minX = 0; house.maxX = 10; house.alive = true; other.minX = 100; other.maxX = 110; other.alive = true;
    static CThingAICreature a, b, c, dead; memset(&a, 0, sizeof(a)); memset(&b, 0, sizeof(b)); memset(&c, 0, sizeof(c)); memset(&dead, 0, sizeof(dead));
    a.alive = b.alive = c.alive = true; a.PHome.m_pItem = &house; a.position.X = 5; b.PHome.m_pItem = &other; b.position.X = 105; c.PHome.m_pItem = 0; c.position.X = 50; dead.PHome.m_pItem = &house; dead.position.X = 500;
    SVillagerNode head, n1, n2, n3, n4; head._M_next = &n1; n1._M_next = &n2; n2._M_next = &n3; n3._M_next = &n4; n4._M_next = &head;
    n1._M_data.m_pItem = &a; n2._M_data.m_pItem = &b; n3._M_data.m_pItem = &c; n4._M_data.m_pItem = &dead;
    CTCVillage v; v.VillagePeople = &head;
    // a dead / null building: false without walking
    house.alive = false; CHECK(!v.BuildingContainsCorrectVillagers(&house)); CHECK(!v.BuildingContainsCorrectVillagers(0)); CHECK(g_insideCalls == 0); house.alive = true;
    // every live villager is where they belong (the dead one is skipped): true
    CHECK(v.BuildingContainsCorrectVillagers(&house)); CHECK(g_insideCalls == 3);
    // a villager of this house standing outside: false
    a.position.X = 50; CHECK(!v.BuildingContainsCorrectVillagers(&house)); a.position.X = 5;
    // a stranger standing inside: false
    c.position.X = 3; CHECK(!v.BuildingContainsCorrectVillagers(&house)); c.position.X = 50;
    // empty list: true
    head._M_next = &head; CHECK(v.BuildingContainsCorrectVillagers(&house));
    if (fails == 0) printf("00658ec3_TEST PASS\n"); else printf("00658ec3_TEST FAIL\n");
    return fails;
}