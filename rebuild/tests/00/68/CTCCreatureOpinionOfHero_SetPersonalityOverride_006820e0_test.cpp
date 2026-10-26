// Behaviour test for 006820e0: body copied from src/006820e0.cpp (CTCCreatureOpinionOfHero::SetPersonalityOverride), callees stubbed.
#include <cstdio>
#include <cstring>
static int g_lookups = 0, g_sets = 0, g_opEq = 0; static long g_lookupResult = 0, g_setIndex = -1; static bool g_setForce = false, g_opEqResult = false;
static const void* g_lookupName = 0;
struct CBasicStringChar {
    char* PData; unsigned long StringLength; unsigned long DataLength : 31; bool UseFastExtend : 1;
    bool operator==(const char* text) const { ++g_opEq; return g_opEqResult; }
};
class CCharStringData { public: CBasicStringChar Data; long NoRefs; };
class CCharString {
public:
    CCharStringData* PStringData;
    bool operator==(const char* text) const
    {
        if (PStringData == 0)
            return strcmp(text, "") == 0;
        return PStringData->Data == text;
    }
};
class CGameDefinitionManager { public:
    long GetDefGlobalIndexFromName(const CCharString& name) const { ++g_lookups; g_lookupName = &name; return g_lookupResult; }
};
class CWorld { public: unsigned char _pad00[0x10]; CGameDefinitionManager* DefinitionManager; };
static CWorld g_world; static CGameDefinitionManager g_defs;
class CTCCreatureOpinionOfHero { public:
    void* __vftable;
    CWorld* GetWorld() const { return &g_world; }
    void SetPersonalityOverride(long defIndex, bool force) { ++g_sets; g_setIndex = defIndex; g_setForce = force; }
    void SetPersonalityOverride(const CCharString& defName, bool force);
};
void CTCCreatureOpinionOfHero::SetPersonalityOverride(const CCharString& defName, bool force)
{
    CGameDefinitionManager* definitions = GetWorld()->DefinitionManager;
    long defIndex = definitions->GetDefGlobalIndexFromName(defName);
    if (defIndex != 0)
        goto set;
    if (defName.PStringData == 0) {
        if (strcmp("", "") == 0)
            goto set;
    } else if (defName.PStringData->Data == "")
        goto set;
    return;
set:
    SetPersonalityOverride(defIndex, force);
}
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
static void reset() { g_lookups = g_sets = g_opEq = 0; g_setIndex = -1; g_setForce = false; g_lookupName = 0; }
int main()
{
    g_world.DefinitionManager = &g_defs;
    CTCCreatureOpinionOfHero tc; CCharString name; CCharStringData data; memset(&data, 0, sizeof(data)); name.PStringData = &data;
    // found def -> set with its index (no string test)
    reset(); g_lookupResult = 42; tc.SetPersonalityOverride(name, true); CHECK(g_lookups == 1 && g_lookupName == &name && g_sets == 1 && g_setIndex == 42 && g_setForce == true && g_opEq == 0);
    // not found, non-empty name -> nothing
    reset(); g_lookupResult = 0; g_opEqResult = false; tc.SetPersonalityOverride(name, false); CHECK(g_sets == 0 && g_opEq == 1);
    // not found, empty name (data) -> clear the override (index 0)
    reset(); g_opEqResult = true; tc.SetPersonalityOverride(name, false); CHECK(g_sets == 1 && g_setIndex == 0 && g_setForce == false);
    // not found, null data (empty) -> clear the override
    reset(); name.PStringData = 0; tc.SetPersonalityOverride(name, true); CHECK(g_sets == 1 && g_setIndex == 0 && g_setForce == true && g_opEq == 0);
    if (fails == 0) printf("006820e0_TEST PASS\n"); else printf("006820e0_TEST FAIL\n");
    return fails;
}