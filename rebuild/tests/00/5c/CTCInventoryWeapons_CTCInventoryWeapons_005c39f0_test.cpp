// Behaviour test for CTCInventoryWeapons::~CTCInventoryWeapons (0x005C39F0): both weapon pointers are cleared to
// null through a temporary, then the members die in reverse order (list, map, handles, ranged, melee pointer),
// then the screen base and the inventory base.
#include <cstdio>
#include <cstring>

class CThing { public: int id; };
static char g_log[256]; static void Log(const char* s) { std::strcat(g_log, s); std::strcat(g_log, ","); }
struct CBaseIntelligentPointer {
    CThing* m_pItem; const char* Name;
    CBaseIntelligentPointer() : m_pItem(0), Name("tmp") {}
    ~CBaseIntelligentPointer() { Log(Name); }
};
struct CIntelligentPointer_CThing : CBaseIntelligentPointer {
    CIntelligentPointer_CThing(CThing* thing) { m_pItem = thing; }
    CIntelligentPointer_CThing& operator=(const CIntelligentPointer_CThing& other) { m_pItem = other.m_pItem; Log(m_pItem ? "set" : "null"); return *this; }
};
struct CComponentList { ~CComponentList() { Log("list"); } };
struct CClothingItemMap { ~CClothingItemMap() { Log("map"); } };
struct CRetailHandleVector { ~CRetailHandleVector() { Log("handles"); } };
namespace NInventory { class CTCInventoryBase { public: virtual ~CTCInventoryBase() { Log("inventorybase"); } }; }
namespace NHeroInformationScreens { class CBase { public: virtual ~CBase() { Log("screenbase"); } }; }
class CTCInventoryWeapons : public NInventory::CTCInventoryBase, public NHeroInformationScreens::CBase {
public:
    CIntelligentPointer_CThing PActiveMeleeWeapon;
    CIntelligentPointer_CThing PActiveRangedWeapon;
    CRetailHandleVector ActiveWeaponPrimitiveHandles;
    CClothingItemMap WeaponMenuItemMap;
    CComponentList AvailableWeaponsEntries;
    CTCInventoryWeapons(CThing* m, CThing* r) : PActiveMeleeWeapon(m), PActiveRangedWeapon(r)
    { PActiveMeleeWeapon.Name = "melee"; PActiveRangedWeapon.Name = "ranged"; }
    virtual ~CTCInventoryWeapons();
};
CTCInventoryWeapons::~CTCInventoryWeapons()
{
    PActiveMeleeWeapon = 0;
    PActiveRangedWeapon = 0;
}
int main()
{
    CThing a, b;
    CTCInventoryWeapons* w = new CTCInventoryWeapons(&a, &b);
    g_log[0] = 0;
    delete w;
    if (std::strcmp(g_log, "null,tmp,null,tmp,list,map,handles,ranged,melee,screenbase,inventorybase,") != 0) { std::printf("005c39f0_TEST FAIL\n"); return 1; }
    std::printf("005c39f0_TEST PASS\n");
    return 0;
}