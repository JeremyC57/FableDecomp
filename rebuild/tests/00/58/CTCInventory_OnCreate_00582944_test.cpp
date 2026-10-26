// Behaviour test for 00582944: body copied from src/00582944.cpp (CTCInventory::OnCreate), callees stubbed.
#include <cstdio>
#include <cstring>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
static int g_destroyed = 0;
struct CRefCounted {
    virtual void v00() {}
    virtual void Destroy() { ++g_destroyed; }
    long RefCount;
};
struct CInventorySubDef : CRefCounted {
    unsigned char _pad_0x08[0x70];
    long          InventoryDefIndex;
};
template <class T> class CDefPointer { public:
    T* Object;
    CDefPointer() : Object(0) {}
    __forceinline CDefPointer(const CDefPointer& other) : Object(other.Object) { if (Object) ++Object->RefCount; }
    __forceinline ~CDefPointer() { if (Object && --Object->RefCount == 0) Object->Destroy(); }
    T* operator->() const { return Object; }
};
template <class T> struct CRawDefPointer {
    T* Object;
    CRawDefPointer() : Object(0) {}
};
static CInventorySubDef* g_subDef = 0; static int g_getSubDef = 0;
struct CThingDef : CRefCounted {
    bool GetSubDef(CRawDefPointer<CInventorySubDef>* out) const;
};
bool CThingDef::GetSubDef(CRawDefPointer<CInventorySubDef>* out) const
{
    ++g_getSubDef;
    if (!g_subDef) return false;
    ++g_subDef->RefCount; out->Object = g_subDef; return true;
}
class CThing { public:
    unsigned char           _pad_0x00[0x70];
    CDefPointer<CThingDef>  PDef;
    __forceinline CDefPointer<CThingDef> GetDef() const { return PDef; }
};
struct CScreenBase { virtual void v00() {} };
static CScreenBase* g_registered = 0; static const char* g_log[8]; static int g_logN = 0;
struct CScreenManager { void RegisterScreen(CScreenBase* screen) { g_registered = screen; g_log[g_logN++] = "register"; } };
static CScreenManager g_screens;
class CPlayerGui { public: CScreenManager* GetScreenManager() { return &g_screens; } };
static CPlayerGui g_gui; CPlayerGui* g_pPlayerGui = &g_gui;
static long g_setPDef = -1; static bool g_quickAssign = true;
struct CTCInventoryBase {
    virtual void v00() {}
    CThing*       ParentThing;
    unsigned char _pad_0x08[0x128];
    void SetPDef(long globalDefIndex) { g_setPDef = globalDefIndex; g_log[g_logN++] = "setpdef"; }
    void OnCreate() { g_log[g_logN++] = "base"; }
};
class CTCInventory : public CTCInventoryBase, public CScreenBase { public:
    unsigned char _pad_0x134[0x9A];
    bool          QuickAccessAssignPending;
    void SetQuickAccessAssign(bool assign) { g_quickAssign = assign; g_log[g_logN++] = "quick"; }
    void OnCreate();
};
void CTCInventory::OnCreate()
{
    g_pPlayerGui->GetScreenManager()->RegisterScreen(this);
    CRawDefPointer<CInventorySubDef> pdef;
    ParentThing->GetDef()->GetSubDef(&pdef);
    CInventorySubDef* def = pdef.Object;
    SetPDef(def->InventoryDefIndex);
    CTCInventoryBase::OnCreate();
    SetQuickAccessAssign(false);
    QuickAccessAssignPending = false;
    if (--def->RefCount == 0)
        def->Destroy();
}
int main()
{
    static CThing thing; static CThingDef thingDef; static CInventorySubDef subDef;
    memset(&thing, 0, sizeof(thing)); thingDef.RefCount = 1; thing.PDef.Object = &thingDef; subDef.RefCount = 1; subDef.InventoryDefIndex = 88; g_subDef = &subDef;
    CTCInventory inv; inv.ParentThing = &thing; inv.QuickAccessAssignPending = true;
    inv.OnCreate();
    CHECK(g_registered == (CScreenBase*)&inv); CHECK(g_getSubDef == 1); CHECK(g_setPDef == 88); CHECK(!g_quickAssign && !inv.QuickAccessAssignPending);
    CHECK(g_logN == 4 && strcmp(g_log[0], "register") == 0 && strcmp(g_log[1], "setpdef") == 0 && strcmp(g_log[2], "base") == 0 && strcmp(g_log[3], "quick") == 0);
    CHECK(subDef.RefCount == 1 && thingDef.RefCount == 1 && g_destroyed == 0);
    subDef.RefCount = 0; inv.OnCreate(); CHECK(g_destroyed == 1);
    if (fails == 0) printf("00582944_TEST PASS\n"); else printf("00582944_TEST FAIL\n");
    return fails;
}