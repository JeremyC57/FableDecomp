// Behaviour test for 00747480: body copied from src/00747480.cpp (CTCStealth::UpdateViewability), callees stubbed.
#include <cstdio>
#include <cstring>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
class C3DVector { public: float X, Y, Z; };
static C3DVector g_pos = { 1.0f, 2.0f, 3.0f };
class CThing { public: const C3DVector& GetPosition() const { return g_pos; } };
static long g_theme = 0; static const C3DVector* g_themePos = 0; static int g_themeCalls = 0;
class CWorldMap { public:
    long GetStrongestEngineThemeAt(const C3DVector& pos, long* weight) { ++g_themeCalls; g_themePos = &pos; *weight = 9; return g_theme; }
};
static int g_destroyed = 0;
class CEngineThemeDef { public:
    virtual void v00() {}
    virtual void Destroy() const { ++g_destroyed; }
    mutable long RefCount;
    unsigned char _pad08[0x88 - 0x08];
    float StealthFactor;
};
template<class T> class CDefPointer { public:
    T* Object;
    CDefPointer() : Object(0) { }
    ~CDefPointer()
    {
        if (Object && --Object->RefCount == 0)
            Object->Destroy();
    }
};
static CEngineThemeDef* g_def = 0; static long g_askedIndex = -1; static int g_getDef = 0;
class CDefinitionManager { public:
    bool GetDef(long index, CDefPointer<const CEngineThemeDef>* out);
};
bool CDefinitionManager::GetDef(long index, CDefPointer<const CEngineThemeDef>* out)
{
    ++g_getDef; g_askedIndex = index;
    if (!g_def) return false;
    ++g_def->RefCount; out->Object = g_def; return true;
}
static CWorldMap g_map; static CDefinitionManager g_manager;
class CTCBase { public:
    virtual void v00() {}
    CThing* ParentThing;
    bool ToBeRemoved;
    bool IsInGlobalUpdateTCs;
    CWorldMap* GetWorldMap_00686d30() { return &g_map; }
    CDefinitionManager* GetDefinitionManager() { return &g_manager; }
};
class CTCStealth : public CTCBase { public:
    bool Viewable;
    bool Smellable;
    bool Hearable;
    bool Sneaking;
    void UpdateViewability();
};
void CTCStealth::UpdateViewability()
{
    Viewable = true;
    if (Sneaking) {
        const C3DVector& position = ParentThing->GetPosition();
        long weight;
        long theme = GetWorldMap_00686d30()->GetStrongestEngineThemeAt(position, &weight);
        CDefPointer<const CEngineThemeDef> def;
        GetDefinitionManager()->GetDef(theme, &def);
        if (def.Object) {
            if (def.Object->StealthFactor > 1e-4f)
                Viewable = false;
        }
    }
}
int main()
{
    static CThing thing; static CEngineThemeDef def; def.RefCount = 1; def.StealthFactor = 0.5f; g_def = &def; g_theme = 6;
    CTCStealth tc; tc.ParentThing = &thing;
    // 1: not sneaking: viewable, no lookups
    tc.Sneaking = false; tc.Viewable = false; tc.UpdateViewability(); CHECK(tc.Viewable && g_themeCalls == 0);
    // 2: sneaking in a stealthy theme: hidden
    tc.Sneaking = true; tc.UpdateViewability(); CHECK(!tc.Viewable); CHECK(g_themeCalls == 1 && g_themePos == &g_pos); CHECK(g_getDef == 1 && g_askedIndex == 6); CHECK(def.RefCount == 1 && g_destroyed == 0);
    // 3: theme with no stealth factor: viewable
    def.StealthFactor = 0.0f; tc.UpdateViewability(); CHECK(tc.Viewable);
    def.StealthFactor = 1e-4f; tc.UpdateViewability(); CHECK(tc.Viewable);
    // 4: no def for the theme
    g_def = 0; tc.Viewable = false; tc.UpdateViewability(); CHECK(tc.Viewable && g_destroyed == 0);
    // 5: a def held only by the lookup is destroyed
    g_def = &def; def.RefCount = 0; def.StealthFactor = 2.0f; tc.UpdateViewability(); CHECK(!tc.Viewable && g_destroyed == 1);
    if (fails == 0) printf("00747480_TEST PASS\n"); else printf("00747480_TEST FAIL\n");
    return fails;
}