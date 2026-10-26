// behaviour test for 0x0056EECA CTCInventoryExperience::OnKill(): the screen handed to the manager is the inventory's CBase subobject, the base OnKill runs after the deregistration, and nothing else is touched
#include <cstdio>
struct CBase { int ScreenTag; };
static const CBase* gDeregistered = 0;
static int gOrder = 0, gDeregOrder = 0, gBaseOrder = 0;
struct CManager { void DeregisterScreen(const CBase& s) { gDeregistered = &s; gDeregOrder = ++gOrder; } };
static CManager gManager;
class CPlayerGui { public: CManager* GetPHeroInfoScreensManager() const { return &gManager; } };
static CPlayerGui gGui;
static CPlayerGui* g_pPlayerGui = &gGui;
struct CTCInventoryBase {
    bool Dying;
    CTCInventoryBase() : Dying(false) { }
    void BaseOnKill() { Dying = true; gBaseOrder = ++gOrder; }
};
struct CInventory : CTCInventoryBase, CBase {
    void OnKill()                                   // the function under test
    {
        CBase* screen = this;
        g_pPlayerGui->GetPHeroInfoScreensManager()->DeregisterScreen(*screen);
        CTCInventoryBase::BaseOnKill();
    }
};
int main()
{
    int fail = 0;
    CInventory inv;
    inv.ScreenTag = 0x130;
    inv.OnKill();
    if (gDeregistered != static_cast<CBase*>(&inv) || gDeregistered->ScreenTag != 0x130) fail |= 1;
    if (!inv.Dying) fail |= 2;
    if (gDeregOrder != 1 || gBaseOrder != 2) fail |= 4;
    if (fail) { std::printf("0056eeca_TEST FAIL %d\n", fail); return 1; }
    std::printf("0056eeca_TEST PASS\n");
    return 0;
}