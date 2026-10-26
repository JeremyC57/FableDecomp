#include <cstdio>
// Behaviour test for CTCInventory::ConstructScreenPC (00591e5e): same logic over a copy of the model.
class CThing { public: int id; };
struct CComponent { int id; };
class CTCInventory;
static CTCInventory* g_inventory = 0;
static CThing* g_thing = 0;
static int g_order = 0, g_invOrder = 0, g_thingOrder = 0, g_buildOrder = 0;
static long g_sum = 0;
struct CFrontEndManager_Calls {
    virtual void SetActiveInventory(CTCInventory* inventory) { g_inventory = inventory; g_invOrder = ++g_order; }
    virtual void SetActiveThing(CThing* thing) { g_thing = thing; g_thingOrder = ++g_order; }
    static CFrontEndManager_Calls* GetInstance() { static CFrontEndManager_Calls s; return &s; }
};
class CTCInventory { public:
    CThing* PThing;
    CComponent* GetItemsListUIComponentPC(CComponent* parent) { g_buildOrder = ++g_order; return parent; }
    CComponent* ConstructScreenPC(CComponent* parent)
    {
        CFrontEndManager_Calls::GetInstance()->SetActiveInventory(this);
        CFrontEndManager_Calls::GetInstance()->SetActiveThing(PThing);
        return GetItemsListUIComponentPC(parent);
    }
};
int main()
{
    CThing thing; CComponent comp; CTCInventory inv; inv.PThing = &thing;
    CComponent* r = inv.ConstructScreenPC(&comp);
    if (!( r == &comp && g_inventory == &inv && g_thing == &thing && g_invOrder == 1 && g_thingOrder == 2 && g_buildOrder == 3)) { std::printf("00591e5e_TEST FAIL\n"); return 1; }
    std::printf("00591e5e_TEST PASS\n");
    return 0;
}