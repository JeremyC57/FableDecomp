#include <cstdio>
// Behaviour test for CTCInventory::ConstructStyleCardsScreenPC (0058a729): same logic over a copy of the model (NF95 2026-11-19: the PDB
// void members on NUISystem::CComponent).
class CThing { public: int id; };
namespace NUISystem { class CComponent { public: int id; }; }
using NUISystem::CComponent;
class CTCInventory;
static CTCInventory* g_inventory = 0;
static CThing* g_thing = 0;
static int g_order = 0, g_invOrder = 0, g_thingOrder = 0, g_buildOrder = 0;
static long g_sum = 0;
namespace NUISystem { class CManager { public:
    virtual void SetActiveInventory(CTCInventory* inventory) { g_inventory = inventory; g_invOrder = ++g_order; }
    virtual void SetActiveThing(CThing* thing) { g_thing = thing; g_thingOrder = ++g_order; }
    static CManager* __fastcall GetInstance() { static CManager s; return &s; }
}; }
class CTCInventory { public:
    CThing* PThing;
    CComponent* g_parent;
    void GetStyleCardsListUIComponent(long a, long b, long c, CComponent* parent) { g_buildOrder = ++g_order; g_sum = a + b + c; g_parent = parent; }
    void ConstructStyleCardsScreenPC(long a, long b, long c, CComponent* parent)
    {
        NUISystem::CManager::GetInstance()->SetActiveInventory(this);
        NUISystem::CManager::GetInstance()->SetActiveThing(PThing);
        GetStyleCardsListUIComponent(a, b, c, parent);
    }
};
int main()
{
    CThing thing; CComponent comp; CTCInventory inv; inv.PThing = &thing;
    inv.ConstructStyleCardsScreenPC(1, 2, 3, &comp);
    CComponent* r = inv.g_parent;
    if (!(g_sum == 6 && r == &comp && g_inventory == &inv && g_thing == &thing && g_invOrder == 1 && g_thingOrder == 2 && g_buildOrder == 3)) { std::printf("0058a729_TEST FAIL\n"); return 1; }
    std::printf("0058a729_TEST PASS\n");
    return 0;
}