#define ADDR "008904e0"
#include <cstdio>
#include <cstring>
// Behaviour model: stub callees record a call trace (one letter each) and their arguments.
static char g_seq[64] = "";
static long g_arg[8];
static void rec(const char* s) { std::strcat(g_seq, s); }
static int g_strDtors = 0;
class CCharString { public:
    const char* s;
    CCharString() : s("") { }
    CCharString(const char* t) : s(t) { }
    CCharString(const char* t, long) : s(t) { }
    CCharString(const CCharString& o) : s(o.s) { }
    ~CCharString() { ++g_strDtors; }
    const CCharString& operator=(const CCharString& o) { s = o.s; return *this; }
    bool operator==(const char* t) const { return std::strcmp(s, t) == 0; }
    bool operator!=(const char* t) const { return std::strcmp(s, t) != 0; }
    bool IsEmpty() const { return s[0] == 0; }
};
static const char* g_strings[4] = { "", "DEF_ONE", "DEF_TWO", "DEF_THREE" };
struct CDefStringTable_Retail { CCharString GetString(long index) { return CCharString(g_strings[index]); } };
static CDefStringTable_Retail g_defStringTable;
class CDefString { public:
    long m_index;
    CDefString(long i) : m_index(i) { }
    CDefString(const CDefString& o) : m_index(o.m_index) { }
    bool operator==(const CCharString& name) const { return std::strcmp(g_strings[m_index], name.s) == 0; }
};
struct CTCBase_Retail { int x; };
struct CInterfaceMap_Retail {
    struct SEntry { long type; CTCBase_Retail* component; };
    SEntry* first; SEntry* last;
    SEntry* LowerBound(const long* type) { SEntry* it = first; while (it != last && it->type < *type) ++it; return it; }
};
class CScriptThing;
struct CThing_Retail {
    unsigned char m_type;
    unsigned long m_presentBits[9];
    CInterfaceMap_Retail m_interfaces;
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
    CInterfaceMap_Retail::SEntry m_entries[6]; long m_entryCount;
    bool IsPlayer() { rec("P"); return m_isPlayer; }
    CTCBase_Retail* AddTC(const CCharString& name, long a, long b) { rec("A"); g_arg[0] = (long)name.s; g_arg[1] = a; g_arg[2] = b; return 0; }
    void RemoveTC(long type) { rec("R"); g_arg[0] = type; }
    void FinishCurrentAction() { rec("F"); }
    void SheatheAllWeapons() { rec("S"); }
    void UnsheatheAllWeapons() { rec("U"); }
    void UnsheatheMeleeWeapon() { rec("M"); }
    void SetAsGivingHeroStatChangesOnBeingHit(bool giving) { rec("G"); g_arg[3] = giving; }
    template<class T> bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            CInterfaceMap_Retail* map = &m_interfaces;
            CInterfaceMap_Retail::SEntry* it = map->LowerBound(&type);
            if (it == map->last || interfaceType < it->type)
                it = map->last;
            *ppOut = (T*)it->component;
            return true;
        }
        return false;
    }
    unsigned long HasInterface(long interfaceType) const { return m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31)); }
    CThing_Retail() { Reset(); }
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
        m_entries[i].type = type; m_entries[i].component = (CTCBase_Retail*)comp; ++m_entryCount;
        m_interfaces.last = m_entries + m_entryCount; m_entries[m_entryCount].type = 0x7fffffff; m_entries[m_entryCount].component = 0;
        m_presentBits[type >> 5] |= (1UL << (type & 31));
    }
};
static bool g_valid = true;
class CScriptThing { public:
    CThing_Retail* m_pThing;
    CThing_Retail* GetThing() const { rec("g"); return m_pThing; }
    bool IsValid() const { rec("v"); return g_valid; }
};
// components
struct CExpression_Retail { long m_def; CDefString GetDefName() const { rec("D"); return CDefString(m_def); } };
struct CTCHeroExpression_Retail { CExpression_Retail* m_pCurrentExpression; };
struct CTCInventory_Retail {
    bool m_held;
    bool IsButtonHeldForExpression(long button) { rec("B"); g_arg[0] = button; return m_held; }
    float GetBestFishWeight(long defIndex) { rec("W"); g_arg[1] = defIndex; return 2.5f; }
};
struct CTCFishing_Retail { void GoFishing(const CScriptThing& spot) { rec("O"); g_arg[0] = (long)&spot; } };
struct CTCOracleMinigame_Retail { void* m_pGame; };
struct CTCChest_Retail { long GetNumberOfKeysToUnlock(CCharString& keyName) { rec("K"); keyName = "KEY"; return 3; } };
struct CGameDefinitionManager_Retail {
    static CGameDefinitionManager_Retail& Get();
    long GetDefGlobalIndexFromName(const CCharString& name) { rec("I"); return std::strcmp(name.s, "FISH") == 0 ? 77 : 0; }
};
static CGameDefinitionManager_Retail g_gameDefs;
CGameDefinitionManager_Retail& CGameDefinitionManager_Retail::Get() { return g_gameDefs; }
static const float g_noFishWeight = 0.0f;
struct CPlayer_Retail {
    CThing_Retail* m_pHero;
    CThing_Retail* GetHeroThing() { rec("h"); return m_pHero; }
    CThing_Retail* GetControlledCreature() { rec("c"); return m_pHero; }
};
static CPlayer_Retail g_player;
struct CPlayerManager_Retail {
    CPlayer_Retail* GetPlayer() { return &g_player; }
    CPlayer_Retail* GetMainPlayer() { return &g_player; }
    void UnfreezePlayerControls(long player) { rec("Z"); g_arg[5] = player; }
};
// world
struct CRegionDef_Retail {
    long m_refCount; long m_def; int m_deleted;
    CDefString m_wanderingPopulationScriptDef;
    CRegionDef_Retail() : m_refCount(1), m_def(1), m_deleted(0), m_wanderingPopulationScriptDef(2) { }
    void AddRef() { ++m_refCount; }
    void DeleteThis() { ++m_deleted; }
    CDefString GetDefName() const { return CDefString(m_def); }
};
template<class T> class CCountedPointer { public:
    T* Data;
    CCountedPointer(const CCountedPointer& other) : Data(other.Data) { if (Data) Data->AddRef(); }
    ~CCountedPointer() { if (Data && --Data->m_refCount == 0) Data->DeleteThis(); }
    explicit CCountedPointer(T* data) : Data(data) { if (Data) Data->AddRef(); }
    T* operator->() const { return Data; }
    bool IsNull() const { return Data == 0; }
};
struct CRegion_Retail {
    CCountedPointer<CRegionDef_Retail> m_pDef;
    bool m_creatureGenerationEnabled;
    CRegion_Retail() : m_pDef(0), m_creatureGenerationEnabled(false) { }
};
struct CWorldMap_Retail {
    CRegion_Retail m_regions[4]; long m_current;
    long GetCurrentRegionNumber() { return m_current; }
    CRegion_Retail& GetRegion(long region) { return m_regions[region]; }
    CRegion_Retail& GetRegionEntry(long region) { return m_regions[region]; }
    long GetRegionNumberFromName(const CCharString& name) { return std::strcmp(name.s, "TWO") == 0 ? 2 : (std::strcmp(name.s, "ZERO") == 0 ? 0 : -1); }
};
struct CCombatManager_Retail { void CreatureUnsheathWeapon(CThing_Retail* creature, long mode) { rec("C"); g_arg[0] = (long)creature; g_arg[1] = mode; } };
class C3DVector { public: float X, Y, Z; C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { } };
struct CMessageEventManager_Retail {
    void* AddMessage(long type, unsigned long delay, const C3DVector& position, CThing_Retail* from, CThing_Retail* to, float value1, float value2, long data)
    { rec("m"); g_arg[0] = type; g_arg[1] = (long)delay; g_arg[2] = (long)(position.X + position.Y + position.Z); g_arg[3] = (long)from + (long)to; g_arg[4] = (long)(value1 + value2); g_arg[6] = data; return 0; }
};
struct CAIGameCameraBase_Retail { void ScriptEnd() { rec("E"); } };
class CWorld { public:
    CWorldMap_Retail m_map; CCombatManager_Retail m_combat; CMessageEventManager_Retail m_messages; CAIGameCameraBase_Retail m_camera;
    CCombatManager_Retail* m_pCombatManager;
    CMessageEventManager_Retail* PMessageEventManager_Data;
    CAIGameCameraBase_Retail* PCurrentGameCamera;
    bool m_creatureGeneratorsEnabled;
    CWorldMap_Retail* GetWorldMap() { return &m_map; }
    CWorldMap_Retail* PeekWorldMap() { return &m_map; }
};
struct CQuestManager_Retail { bool m_allowed; bool IsCreatureGenerationAllowed() { return m_allowed; } };
static CQuestManager_Retail g_questManager;
static CQuestManager_Retail* g_pQuestManager = &g_questManager;
struct CPlayerGui_Retail { void Close() { rec("X"); } };
static CPlayerGui_Retail g_gui;
static CPlayerGui_Retail* g_pPlayerGui = &g_gui;
struct CDisplayEngine_Retail { void EndScreenFadeOut() { rec("Y"); } };
class CGameScriptInterface { public:
    CWorld* World;
    CDisplayEngine_Retail* DisplayEngine;
    CPlayerManager_Retail* PlayerManager;
    long CurrentPlayer;
    void SheatheHeroWeapons() const { rec("H"); }
    void GiveHeroItem(const CScriptThing& thing) const { rec("i"); g_arg[7] = (long)&thing; }
    bool GiveHeroItemsFromContainer(const CScriptThing& container, bool flag) const { rec("j"); g_arg[7] = (long)&container + flag; return true; }
    void EntitySetAsToAddToComboMultiplierWhenHit(const CScriptThing&, bool flag) const { rec("1"); g_arg[1] = flag; }
    void EntitySetAsToAddToStatChangesWhenHit(const CScriptThing&, bool flag) const { rec("2"); g_arg[2] = flag; }
    void EndLetterBox() { rec("L"); }
    void UnfreezeControls()
    {
        CThing_Retail* creature = PlayerManager->GetMainPlayer()->GetControlledCreature();
        if (creature && !creature->m_dead)
            PlayerManager->UnfreezePlayerControls(CurrentPlayer);
    }
    void EntitySetAsStolen(const CScriptThing& thing) const;
};
static CWorld g_world;
static CDisplayEngine_Retail g_display;
static CPlayerManager_Retail g_players;
static CGameScriptInterface g_gsi;
static CThing_Retail g_hero, g_thing;
static CScriptThing g_script;
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

void CGameScriptInterface::EntitySetAsStolen(const CScriptThing& thing) const
{
    if (thing.IsValid()) {
        GiveHeroItem(thing);
        C3DVector position(0.0f, 0.0f, 0.0f);
        CMessageEventManager_Retail* messages = World->PMessageEventManager_Data;
        messages->AddMessage(0x47, 0x32, position, 0, 0, 0.0f, 0.0f, (long)thing.GetThing());
    }
}

int main()
{
    setup(); g_gsi.EntitySetAsStolen(g_script);
    CHECK(SEQ("vigm") && g_arg[0] == 0x47 && g_arg[1] == 0x32 && g_arg[6] == (long)&g_thing && g_arg[7] == (long)&g_script);
    g_valid = false; g_seq[0] = 0; g_gsi.EntitySetAsStolen(g_script); CHECK(SEQ("v"));
    std::printf("008904e0_TEST PASS\n");
    return 0;
}