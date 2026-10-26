// Behaviour test for CThing::CreateFromDef (0x004CA930): the component map is reserved for its current size plus the
// def's count, each def component is added by its string-table name with its own flag (forced on while a saved game
// loads for type-flag-2 things), and the def's kill-lock bit lands in the thing.
#include <cstdio>
#include <cstring>
class CCharString { public: const char* PStringData; CCharString() : PStringData(0) {} ~CCharString() { PStringData = 0; } };
static const char* g_names[4] = { "CTCPhysics", "CTCLight", "CTCSound", "CTCScript" };
struct CDefStringTable { CCharString GetString(long id) { CCharString s; s.PStringData = g_names[id]; return s; } };
static CDefStringTable g_DefStringTable;
class CTCBase; class CTCParamsBase;
struct SComponentPair { long first; CTCBase* second; };
static unsigned long g_reserved = 0; static int g_reserves = 0;
struct CComponentMap { SComponentPair* m_first; SComponentPair* m_last; SComponentPair* m_end; void reserve(unsigned long count) { ++g_reserves; g_reserved = count; } long size() const { return m_last - m_first; } };
class CWorld { public: unsigned char _pad00[0x102]; bool m_loadingSavedGame; };
class CThingManager { public: unsigned char _pad00[0x24]; CWorld* World; };
static CWorld g_world; static CThingManager g_manager; static CThingManager* g_pThingManager = &g_manager;
#pragma pack(push, 1)
struct STCDefEntry { long NameId; long _unused; bool Flag; };
struct CThingDef { unsigned char _pad00[0x39]; STCDefEntry* TCs; signed char NoTCs; unsigned char _pad3e[0x41 - 0x3e]; unsigned char DefBit0 : 1; unsigned char DefBit1 : 1; unsigned char KillLocked : 1; };
#pragma pack(pop)
static int g_adds = 0; static const char* g_addNames[8]; static bool g_addFlags[8]; static CTCParamsBase* g_addParams[8];
class CThing { public:
    unsigned char _pad00[0x44]; CComponentMap Components; unsigned char _pad50[0x6c - 0x50]; unsigned long ThingTypeFlags; unsigned char _pad70[0x91 - 0x70];
    unsigned char Dead : 1; unsigned char CreatedByOwnerThingCreation : 1; unsigned char IncludeInThingScans : 1; unsigned char Activated : 1; unsigned char GloballyLoaded : 1; unsigned char InLimbo : 1; unsigned char KillLocked : 1; unsigned char CreatedBySerialisation : 1;
    CTCBase* AddTC(const CCharString& name, bool a2, CTCParamsBase* params) { g_addNames[g_adds] = name.PStringData; g_addFlags[g_adds] = a2; g_addParams[g_adds] = params; ++g_adds; return 0; }
    void CreateFromDef(const CThingDef* def);
};
void CThing::CreateFromDef(const CThingDef* def)
{
    bool force = false;
    if ((ThingTypeFlags & 2) && g_pThingManager->World->m_loadingSavedGame) force = true;
    Components.reserve(Components.size() + def->NoTCs);
    for (long i = 0; i < def->NoTCs; ++i) {
        CCharString name = g_DefStringTable.GetString(def->TCs[i].NameId);
        bool flag = def->TCs[i].Flag;
        if (force) flag = true;
        AddTC(name, flag, 0);
    }
    KillLocked = def->KillLocked;
}
static int fail(const char* why) { std::printf("004ca930_TEST FAIL: %s\n", why); return 1; }
int main()
{
    g_manager.World = &g_world; g_world.m_loadingSavedGame = false;
    static STCDefEntry entries[3] = { { 0, 0, false }, { 3, 0, true }, { 1, 0, false } };
    static CThingDef def; def.TCs = entries; def.NoTCs = 3; def.KillLocked = 1; def.DefBit0 = 1;
    static SComponentPair existing[2];
    static CThing thing; thing.Components.m_first = existing; thing.Components.m_last = existing + 2; thing.ThingTypeFlags = 2; thing.KillLocked = 0; thing.GloballyLoaded = 1; thing.IncludeInThingScans = 1;
    thing.CreateFromDef(&def);
    if (g_reserves != 1 || g_reserved != 5) return fail("reserve size");
    if (g_adds != 3 || strcmp(g_addNames[0], "CTCPhysics") || strcmp(g_addNames[1], "CTCScript") || strcmp(g_addNames[2], "CTCLight")) return fail("names in def order");
    if (g_addFlags[0] || !g_addFlags[1] || g_addFlags[2] || g_addParams[1] != 0) return fail("own flags when not forced");
    if (!thing.KillLocked || !thing.GloballyLoaded || !thing.IncludeInThingScans) return fail("kill lock copied without touching neighbours");
    // forced: type flag 2 + saved game loading
    g_world.m_loadingSavedGame = true; g_adds = 0; thing.CreateFromDef(&def);
    if (!g_addFlags[0] || !g_addFlags[1] || !g_addFlags[2]) return fail("forced flags");
    // not forced when the type flag is missing
    thing.ThingTypeFlags = 1; g_adds = 0; thing.CreateFromDef(&def);
    if (g_addFlags[0] || g_addFlags[2]) return fail("type flag gate");
    def.KillLocked = 0; def.NoTCs = 0; g_adds = 0; thing.CreateFromDef(&def);
    if (g_adds != 0 || thing.KillLocked || g_reserved != 2) return fail("empty def clears the lock");
    std::printf("004ca930_TEST PASS\n");
    return 0;
}