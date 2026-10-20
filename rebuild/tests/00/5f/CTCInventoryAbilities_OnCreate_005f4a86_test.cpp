// Behaviour test for 005f4a86: body copied from src/005f4a86.cpp (CTCInventoryAbilities::OnCreate), callees stubbed.
#include <cstdio>
#include <cstring>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
static int g_destroyed = 0;
struct CDefPointeeBase {
    virtual void v00() {}
    virtual void Destroy() { ++g_destroyed; }
    long m_refCount;
};
template <class T> struct CDefPointer {
    T* Object;
    T* operator->() const { return Object; }
};
struct CInventoryAbilitiesDef : CDefPointeeBase {
    unsigned char _pad08[0x84 - 0x08];
    long InventoryDefIndex;
};
static CInventoryAbilitiesDef* g_subDef = 0; static int g_getSubDef = 0; static long g_countAtGet = -1;
struct CDefClassBase : CDefPointeeBase {
    bool GetSubDef(CDefPointer<CInventoryAbilitiesDef>* out)
    {
        ++g_getSubDef; g_countAtGet = m_refCount;
        if (!g_subDef) return false;
        ++g_subDef->m_refCount; out->Object = g_subDef; return true;
    }
};
struct CThing {
    unsigned char _pad00[0x70];
    CDefPointer<CDefClassBase> PDef;
};
static int g_registered = 0; static const void* g_registeredScreen = 0;
namespace NHeroInformationScreens {
struct CBase {
    virtual void v00() {}
    virtual void v01() {}
    virtual void v02() {}
};
struct CManager {
    void RegisterScreen(const CBase* screen) { ++g_registered; g_registeredScreen = screen; }
};
}
static NHeroInformationScreens::CManager g_manager;
struct CPlayerGui {
    NHeroInformationScreens::CManager* GetPHeroInfoScreensManager() const { return &g_manager; }
};
static CPlayerGui g_gui; CPlayerGui* g_pPlayerGui = &g_gui;
struct CWorld {
    unsigned char _pad00[0x101];
    bool SerialisingAboutToLoadHeroState;
};
static CWorld g_world;
static int g_setPDef = 0; static long g_pdefIndex = -1; static int g_baseOnCreate = 0; static int g_initAbilities = 0, g_initExpressions = 0;
struct CTCInventoryBase_Abilities {
    virtual void v00() {}
    virtual void v01() {}
    virtual void v02() {}
    virtual void v03() {}
    virtual void OnCreate() { ++g_baseOnCreate; }
    CThing* ParentThing;
    unsigned char _pad08[0x130 - 0x08];
    void SetPDef(long globalDefIndex) { ++g_setPDef; g_pdefIndex = globalDefIndex; }
    CWorld* GetWorld() const { return &g_world; }
};
class CTCInventoryAbilities : public CTCInventoryBase_Abilities, public NHeroInformationScreens::CBase {
public:
    unsigned char _pad_0x134[0x0D];
    bool          Initialised;
    void InitialiseAbilities() { ++g_initAbilities; }
    void InitialiseExpressions() { ++g_initExpressions; }
    virtual void OnCreate();
};

void CTCInventoryAbilities::OnCreate()
{
    g_pPlayerGui->GetPHeroInfoScreensManager()->RegisterScreen(this);
    CDefClassBase* thingDef = ParentThing->PDef.Object;
    CDefPointer<CInventoryAbilitiesDef> def;
    def.Object = 0;
    if (thingDef)
        ++thingDef->m_refCount;
    thingDef->GetSubDef(&def);
    if (thingDef && --thingDef->m_refCount == 0)
        thingDef->Destroy();
    CInventoryAbilitiesDef* abilitiesDef = def.Object;
    SetPDef(abilitiesDef->InventoryDefIndex);
    CTCInventoryBase_Abilities::OnCreate();
    if (!Initialised) {
        if (!GetWorld()->SerialisingAboutToLoadHeroState) {
            InitialiseAbilities();
            InitialiseExpressions();
            Initialised = true;
        }
    }
    if (--abilitiesDef->m_refCount == 0)
        abilitiesDef->Destroy();
}
int main()
{
    static CThing thing; static CDefClassBase classDef; static CInventoryAbilitiesDef subDef;
    thing.PDef.Object = &classDef; classDef.m_refCount = 1; subDef.m_refCount = 1; subDef.InventoryDefIndex = 0x321; g_subDef = &subDef;
    CTCInventoryAbilities inv; inv.ParentThing = &thing; inv.Initialised = false; g_world.SerialisingAboutToLoadHeroState = false;
    inv.OnCreate();
    CHECK(g_registered == 1 && g_registeredScreen == (const NHeroInformationScreens::CBase*)&inv);
    CHECK(g_getSubDef == 1 && g_countAtGet == 2 && classDef.m_refCount == 1);
    CHECK(g_setPDef == 1 && g_pdefIndex == 0x321 && g_baseOnCreate == 1);
    CHECK(g_initAbilities == 1 && g_initExpressions == 1 && inv.Initialised); CHECK(subDef.m_refCount == 1 && g_destroyed == 0);
    // already initialised: no second initialisation
    inv.OnCreate(); CHECK(g_initAbilities == 1 && g_setPDef == 2 && g_baseOnCreate == 2);
    // not initialised but a hero state is about to load: initialisation deferred
    inv.Initialised = false; g_world.SerialisingAboutToLoadHeroState = true; inv.OnCreate(); CHECK(g_initAbilities == 1 && !inv.Initialised);
    // defs whose only references are the lookup are destroyed
    classDef.m_refCount = 0; subDef.m_refCount = 0; inv.OnCreate(); CHECK(g_destroyed == 2);
    if (fails == 0) printf("005f4a86_TEST PASS\n"); else printf("005f4a86_TEST FAIL\n");
    return fails;
}