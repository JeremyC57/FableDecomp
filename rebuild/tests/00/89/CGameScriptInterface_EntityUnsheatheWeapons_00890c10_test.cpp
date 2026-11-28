#define ADDR "00890c10"
#include <cstdio>
#include <cstring>
// Behaviour model: stub callees record a call trace (one letter each) and their arguments.
static char g_seq[64] = "";
static long g_arg[8];
static void rec(const char* s) { std::strcat(g_seq, s); }
static int g_strDtors = 0;
class FixtureCCharString { public:
    const char* s;
    FixtureCCharString() : s("") { }
    FixtureCCharString(const char* t) : s(t) { }
    FixtureCCharString(const char* t, long) : s(t) { }
    FixtureCCharString(const FixtureCCharString& o) : s(o.s) { }
    ~FixtureCCharString() { ++g_strDtors; }
    const FixtureCCharString& operator=(const FixtureCCharString& o) { s = o.s; return *this; }
    bool operator==(const char* t) const { return std::strcmp(s, t) == 0; }
    bool operator!=(const char* t) const { return std::strcmp(s, t) != 0; }
    bool IsEmpty() const { return s[0] == 0; }
};
static const char* g_strings[4] = { "", "DEF_ONE", "DEF_TWO", "DEF_THREE" };
struct FixtureCDefStringTable_Retail { FixtureCCharString GetString(long index) { return FixtureCCharString(g_strings[index]); } };
static FixtureCDefStringTable_Retail g_defStringTable;
class FixtureCDefString { public:
    long m_index;
    FixtureCDefString(long i) : m_index(i) { }
    FixtureCDefString(const FixtureCDefString& o) : m_index(o.m_index) { }
    bool operator==(const FixtureCCharString& name) const { return std::strcmp(g_strings[m_index], name.s) == 0; }
};
struct FixtureCTCBase_Retail { int x; };
struct FixtureCInterfaceMap_Retail {
    struct FixtureSEntry { long type; FixtureCTCBase_Retail* component; };
    FixtureSEntry* first; FixtureSEntry* last;
    FixtureSEntry* LowerBound(const long* type) { FixtureSEntry* it = first; while (it != last && it->type < *type) ++it; return it; }
};
class FixtureCScriptThing;
struct FixtureCThing_Retail {
    unsigned char m_type;
    unsigned long m_presentBits[9];
    FixtureCInterfaceMap_Retail m_interfaces;
    unsigned char m_flags;
    unsigned char m_dead : 1;
    unsigned char m_91_1 : 7;
    unsigned char m_bc_0 : 1;
    unsigned char m_givesHeroStatChangesOnHit : 1;
    unsigned char m_bc_2 : 6;
    unsigned char m_1c9_0 : 3;
    unsigned char m_marryable : 1;
    unsigned char m_marryableToHero : 1;
    unsigned char m_1c9_5 : 3;
    bool m_isPlayer;
    FixtureCInterfaceMap_Retail::FixtureSEntry m_entries[6]; long m_entryCount;
    bool IsPlayer() { rec("P"); return m_isPlayer; }
    FixtureCTCBase_Retail* AddTC(const FixtureCCharString& name, long a, long b) { rec("A"); g_arg[0] = (long)name.s; g_arg[1] = a; g_arg[2] = b; return 0; }
    void RemoveTC(long type) { rec("R"); g_arg[0] = type; }
    void FinishCurrentAction() { rec("F"); }
    void SheatheAllWeapons() { rec("S"); }
    void UnsheatheAllWeapons() { rec("U"); }
    void UnsheathePrimaryWeapon() { rec("M"); }
    void SetAsGivingHeroStatChangesOnBeingHit(bool giving) { rec("G"); g_arg[3] = giving; }
    template<class T> bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            FixtureCInterfaceMap_Retail* map = &m_interfaces;
            FixtureCInterfaceMap_Retail::FixtureSEntry* it = map->LowerBound(&type);
            if (it == map->last || interfaceType < it->type)
                it = map->last;
            *ppOut = (T*)it->component;
            return true;
        }
        return false;
    }
    unsigned long HasInterface(long interfaceType) const { return m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31)); }
    FixtureCThing_Retail() { Reset(); }
    void Reset()
    {
        m_type = 0; std::memset(m_presentBits, 0, sizeof(m_presentBits)); m_flags = 0; m_dead = 0; m_91_1 = 0;
        m_bc_0 = 0; m_givesHeroStatChangesOnHit = 0; m_bc_2 = 0; m_1c9_0 = 0; m_marryable = 0; m_marryableToHero = 0; m_1c9_5 = 0;
        m_isPlayer = false; m_entryCount = 0; m_interfaces.first = m_entries; m_interfaces.last = m_entries;
        m_entries[0].type = 0x7fffffff; m_entries[0].component = 0;
    }
    void AddComponent(long type, void* comp)   // keeps the entries sorted; the end entry carries a null component
    {
        long i = m_entryCount; while (i > 0 && m_entries[i - 1].type > type) { m_entries[i] = m_entries[i - 1]; --i; }
        m_entries[i].type = type; m_entries[i].component = (FixtureCTCBase_Retail*)comp; ++m_entryCount;
        m_interfaces.last = m_entries + m_entryCount; m_entries[m_entryCount].type = 0x7fffffff; m_entries[m_entryCount].component = 0;
        m_presentBits[type >> 5] |= (1UL << (type & 31));
    }
};
static bool g_valid = true;
class FixtureCScriptThing { public:
    FixtureCThing_Retail* m_pThing;
    FixtureCThing_Retail* GetThing() const { rec("g"); return m_pThing; }
    bool IsValid() const { rec("v"); return g_valid; }
};
// components
struct FixtureCExpression_Retail { long m_def; FixtureCDefString GetDefName() const { rec("D"); return FixtureCDefString(m_def); } };
struct FixtureCTCHeroExpression_Retail { FixtureCExpression_Retail* m_pCurrentExpression; };
struct FixtureCTCInventory_Retail {
    bool m_held;
    bool IsButtonHeldForExpression(long button) { rec("B"); g_arg[0] = button; return m_held; }
    float GetBestFishWeight(long defIndex) { rec("W"); g_arg[1] = defIndex; return 2.5f; }
};
struct FixtureCTCFishing_Retail { void GoFishing(const FixtureCScriptThing& spot) { rec("O"); g_arg[0] = (long)&spot; } };
struct FixtureCTCOracleMinigame_Retail { void* m_pGame; };
struct FixtureCTCChest_Retail { long GetNumberOfKeysToUnlock(FixtureCCharString& keyName) { rec("K"); keyName = "KEY"; return 3; } };
struct FixtureCGameDefinitionManager_Retail {
    static FixtureCGameDefinitionManager_Retail& Get();
    long GetDefGlobalIndexFromName(const FixtureCCharString& name) { rec("I"); return std::strcmp(name.s, "FISH") == 0 ? 77 : 0; }
};
static FixtureCGameDefinitionManager_Retail g_gameDefs;
FixtureCGameDefinitionManager_Retail& FixtureCGameDefinitionManager_Retail::Get() { return g_gameDefs; }
static const float g_noFishWeight = 0.0f;
struct FixtureCPlayer_Retail {
    FixtureCThing_Retail* m_pHero;
    FixtureCThing_Retail* GetHeroThing() { rec("h"); return m_pHero; }
    FixtureCThing_Retail* GetControlledCreature() { rec("c"); return m_pHero; }
};
static FixtureCPlayer_Retail g_player;
struct FixtureCPlayerManager_Retail {
    FixtureCPlayer_Retail* GetPlayer() { return &g_player; }
    FixtureCPlayer_Retail* GetMainPlayer() { return &g_player; }
    void UnfreezePlayerControls(long player) { rec("Z"); g_arg[5] = player; }
};
// world
struct FixtureCRegionDef_Retail {
    long m_refCount; long m_def; int m_deleted;
    FixtureCDefString m_wanderingPopulationScriptDef;
    FixtureCRegionDef_Retail() : m_refCount(1), m_def(1), m_deleted(0), m_wanderingPopulationScriptDef(2) { }
    void AddRef() { ++m_refCount; }
    void DeleteThis() { ++m_deleted; }
    FixtureCDefString GetDefName() const { return FixtureCDefString(m_def); }
};
template<class T> class FixtureCCountedPointer { public:
    T* Data;
    FixtureCCountedPointer(const FixtureCCountedPointer& other) : Data(other.Data) { if (Data) Data->AddRef(); }
    ~FixtureCCountedPointer() { if (Data && --Data->m_refCount == 0) Data->DeleteThis(); }
    explicit FixtureCCountedPointer(T* data) : Data(data) { if (Data) Data->AddRef(); }
    T* operator->() const { return Data; }
    bool IsNull() const { return Data == 0; }
};
struct FixtureCRegion_Retail {
    FixtureCCountedPointer<FixtureCRegionDef_Retail> m_pDef;
    bool m_creatureGenerationEnabled;
    FixtureCRegion_Retail() : m_pDef(0), m_creatureGenerationEnabled(false) { }
};
struct FixtureCWorldMap_Retail {
    FixtureCRegion_Retail m_regions[4]; long m_current;
    long GetCurrentRegionNumber() { return m_current; }
    FixtureCRegion_Retail& GetRegion(long region) { return m_regions[region]; }
    FixtureCRegion_Retail& GetRegionEntry(long region) { return m_regions[region]; }
    long GetRegionNumberFromName(const FixtureCCharString& name) { return std::strcmp(name.s, "TWO") == 0 ? 2 : (std::strcmp(name.s, "ZERO") == 0 ? 0 : -1); }
};
struct FixtureCCombatManager_Retail { void CreatureUnsheathWeapon(FixtureCThing_Retail* creature, long mode) { rec("C"); g_arg[0] = (long)creature; g_arg[1] = mode; } };
class FixtureC3DVector { public: float X, Y, Z; FixtureC3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { } };
struct FixtureCMessageEventManager_Retail {
    void* AddMessage(long type, unsigned long delay, const FixtureC3DVector& position, FixtureCThing_Retail* from, FixtureCThing_Retail* to, float value1, float value2, long data)
    { rec("m"); g_arg[0] = type; g_arg[1] = (long)delay; g_arg[2] = (long)(position.X + position.Y + position.Z); g_arg[3] = (long)from + (long)to; g_arg[4] = (long)(value1 + value2); g_arg[6] = data; return 0; }
};
struct FixtureCAIGameCameraBase_Retail { void ScriptEnd() { rec("E"); } };
class FixtureCWorld { public:
    FixtureCWorldMap_Retail m_map; FixtureCCombatManager_Retail m_combat; FixtureCMessageEventManager_Retail m_messages; FixtureCAIGameCameraBase_Retail m_camera;
    FixtureCCombatManager_Retail* m_pCombatManager;
    FixtureCMessageEventManager_Retail* PMessageEventManager_Data;
    FixtureCAIGameCameraBase_Retail* PCurrentGameCamera;
    bool m_creatureGeneratorsEnabled;
    FixtureCWorldMap_Retail* GetWorldMap() { return &m_map; }
    FixtureCWorldMap_Retail* PeekWorldMap() { return &m_map; }
};
struct FixtureCQuestManager_Retail { bool m_allowed; bool IsCreatureGenerationAllowed() { return m_allowed; } };
static FixtureCQuestManager_Retail g_questManager;
static FixtureCQuestManager_Retail* g_pQuestManager = &g_questManager;
struct FixtureCPlayerGui_Retail { void Close() { rec("X"); } };
static FixtureCPlayerGui_Retail g_gui;
static FixtureCPlayerGui_Retail* g_pPlayerGui = &g_gui;
struct FixtureCDisplayEngine_Retail { void EndScreenFadeOut() { rec("Y"); } };
class FixtureCGameScriptInterface { public:
    FixtureCWorld* World;
    FixtureCDisplayEngine_Retail* DisplayEngine;
    FixtureCPlayerManager_Retail* PlayerManager;
    long CurrentPlayer;
    void SheatheHeroWeapons() const { rec("H"); }
    void GiveHeroItem(const FixtureCScriptThing& thing) const { rec("i"); g_arg[7] = (long)&thing; }
    bool GiveHeroItemsFromContainer(const FixtureCScriptThing& container, bool flag) const { rec("j"); g_arg[7] = (long)&container + flag; return true; }
    void EntitySetAsToAddToComboMultiplierWhenHit(const FixtureCScriptThing&, bool flag) const { rec("1"); g_arg[1] = flag; }
    void EntitySetAsToAddToStatChangesWhenHit(const FixtureCScriptThing&, bool flag) const { rec("2"); g_arg[2] = flag; }
    void EndLetterBox() { rec("L"); }
    void UnfreezeControls()
    {
        FixtureCThing_Retail* creature = PlayerManager->GetMainPlayer()->GetControlledCreature();
        if (creature && !creature->m_dead)
            PlayerManager->UnfreezePlayerControls(CurrentPlayer);
    }
    void EntityUnsheatheWeapons(const FixtureCScriptThing& thing, bool flag) const;
};
static FixtureCWorld g_world;
static FixtureCDisplayEngine_Retail g_display;
static FixtureCPlayerManager_Retail g_players;
static FixtureCGameScriptInterface g_gsi;
static FixtureCThing_Retail g_hero, g_thing;
static FixtureCScriptThing g_script;
static void setup()
{
    g_seq[0] = 0; std::memset(g_arg, 0, sizeof(g_arg)); g_valid = true; g_strDtors = 0;
    g_hero.Reset(); g_thing.Reset();
    g_player.m_pHero = &g_hero; g_script.m_pThing = &g_thing;
    g_world.m_pCombatManager = &g_world.m_combat; g_world.PMessageEventManager_Data = &g_world.m_messages; g_world.PCurrentGameCamera = &g_world.m_camera;
    g_world.m_creatureGeneratorsEnabled = false; g_world.m_map.m_current = 0;
    g_gsi.World = &g_world; g_gsi.DisplayEngine = &g_display; g_gsi.PlayerManager = &g_players; g_gsi.CurrentPlayer = 3;
    g_questManager.m_allowed = true;
}
#define CHECK(c) do { if (!(c)) { std::printf("%s_TEST FAIL (line %d)\n", ADDR, __LINE__); return 1; } } while (0)
#define SEQ(x) (std::strcmp(g_seq, x) == 0)

void FixtureCGameScriptInterface::EntityUnsheatheWeapons(const FixtureCScriptThing& thing, bool flag) const
{
    if (thing.IsValid()) {
        FixtureCThing_Retail* creature = thing.GetThing();
        if (creature->m_flags & 8) {
            if (!creature->IsPlayer()) {
                creature->FinishCurrentAction();
                if (!flag)
                    creature->UnsheatheAllWeapons();
                else
                    World->m_pCombatManager->CreatureUnsheathWeapon(creature, 2);
            } else {
                creature->UnsheathePrimaryWeapon();
            }
        }
    }
}

int main()
{
    setup(); g_gsi.EntityUnsheatheWeapons(g_script, false); CHECK(SEQ("vg"));
    g_thing.m_flags = 8; g_seq[0] = 0; g_gsi.EntityUnsheatheWeapons(g_script, false); CHECK(SEQ("vgPFU"));
    g_seq[0] = 0; g_gsi.EntityUnsheatheWeapons(g_script, true); CHECK(SEQ("vgPFC") && g_arg[1] == 2);
    g_thing.m_isPlayer = true; g_seq[0] = 0; g_gsi.EntityUnsheatheWeapons(g_script, true); CHECK(SEQ("vgPM"));
    g_valid = false; g_seq[0] = 0; g_gsi.EntityUnsheatheWeapons(g_script, true); CHECK(SEQ("v"));
    std::printf("00890c10_TEST PASS\n");
    return 0;
}