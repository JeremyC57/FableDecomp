// Behaviour test for CTCInventoryQuests::OnKill (0x0061A1F0): the screen subobject (null-checked this -> CBase) is deregistered from
// the player GUI's hero-information screen manager, then the base inventory OnKill runs.
#include <cstdio>
static int g_log[8]; static int g_n = 0;
static const void* g_deregistered = 0;
namespace NHeroInformationScreens {
class CBase { public: virtual ~CBase() { } int ScreenId; };
class CManager { public: void DeregisterScreen(const CBase& screen) { g_deregistered = &screen; g_log[g_n++] = 1; } };
}
class CPlayerGui { public:
    NHeroInformationScreens::CManager Manager;
    NHeroInformationScreens::CManager* GetPHeroInfoScreensManager() const { return (NHeroInformationScreens::CManager*)&Manager; }
};
static CPlayerGui g_gui;
template <class T> class CTBaseSingletonNoDefaultAlloc { public: static T& Get() { return g_gui; } };
namespace NInventory {
class CTCInventoryBase { public: int Body[8]; virtual ~CTCInventoryBase() { } virtual void OnKill() { g_log[g_n++] = 2; } };
}
class CTCInventoryQuests : public NInventory::CTCInventoryBase, public NHeroInformationScreens::CBase { public:
    virtual void OnKill()
    {
        NHeroInformationScreens::CBase* screen = this;
        CTBaseSingletonNoDefaultAlloc<CPlayerGui>::Get().GetPHeroInfoScreensManager()->DeregisterScreen(*screen);
        NInventory::CTCInventoryBase::OnKill();
    }
};
int main()
{
    CTCInventoryQuests inv;
    NInventory::CTCInventoryBase& b = inv;
    b.OnKill();
    const NHeroInformationScreens::CBase* expected = &inv;
    if (g_n != 2 || g_log[0] != 1 || g_log[1] != 2 || g_deregistered != expected || (const void*)expected == (const void*)&inv) {
        std::printf("0061a1f0_TEST FAIL\n"); return 1;
    }
    std::printf("0061a1f0_TEST PASS\n");
    return 0;
}