// Behaviour test for 004ca010 CThing::ConstructFromParams (initialise a new thing from its def index, owner and map)
#include <cstdio>
#include <cstring>
static char g_log[256];
static void LOG(const char* s) { std::strcat(g_log, s); std::strcat(g_log, ";"); }
struct CCharString {
    const char* text;
    CCharString() : text("") {}
    CCharString(const char* t, long length) : text(t) { (void)length; }
};
static CCharString g_DefaultSerialisationSectionName("default", -1);
inline CCharString GetDefaultSerialisationSectionName() { return g_DefaultSerialisationSectionName; }
struct CThingBaseDef { long DefaultScriptName; long DefaultScriptData; };
struct CThingBaseDefPointer { CThingBaseDef* m_p; };
static const char* g_strings[3] = { "", "ScriptA", "DataB" };
struct CDefStringTable { CCharString GetString(long index) { return CCharString(g_strings[index], -1); } };
static CDefStringTable g_DefStringTable;
static CThingBaseDef g_def = { 1, 2 };
struct CGameDefinitionManager {
    bool GetDef(long globalIndex, CThingBaseDefPointer* out) { LOG("getdef"); out->m_p = globalIndex == 7 ? &g_def : 0; return out->m_p != 0; }
};
struct CMainGameComponent { bool editor; bool IsEditorRunning() { return editor; } };
class CThing;
struct CThingManager {
    CMainGameComponent* Component;
    CGameDefinitionManager* DefinitionManager;
    void ReregisterAllocatedClass(CThing*, unsigned long type, bool) { LOG(type == 3 ? "rereg" : "rereg?"); }
    unsigned long GetThingTypeFlags(long thingClass) { return 0x100u + (unsigned long)thingClass; }
};
static CThingManager* g_pThingManager;
static long GFGetCurrentFrame() { return 1234; }
class CThing {
public:
    CThingBaseDef* seenDef;
    unsigned char ThingClass;
    CCharString SerialisationSectionName;
    long WorldFrameCreatedAt;
    unsigned long ThingTypeFlags;
    CThingBaseDefPointer PBaseDef;
    CCharString ScriptName, ScriptData;
    CThing *RenderListPrev, *RenderListNext, *UpdateListPrev, *UpdateListNext;
    unsigned short DefGlobalIndex;
    short CurrentMapNumber;
    signed char PlayerNumber;
    unsigned char LevelPersistent : 1;
    void OnBaseDefSet(CThingBaseDef* def) { seenDef = def; LOG("onbasedef"); }
    void CreateUID() { LOG("uid"); }
    void AddTC(const CCharString& name, bool, void*) { LOG(name.text); }
    bool ConstructFromParams(long defGlobalIndex, long playerNumber, long mapNumber, bool reregister);
};
bool CThing::ConstructFromParams(long defGlobalIndex, long playerNumber, long mapNumber, bool reregister)
{
    if (reregister)
        g_pThingManager->ReregisterAllocatedClass(this, ThingClass, reregister);
    DefGlobalIndex = (unsigned short)defGlobalIndex;
    PlayerNumber = (signed char)playerNumber;
    CurrentMapNumber = (short)mapNumber;
    WorldFrameCreatedAt = GFGetCurrentFrame();
    RenderListNext = 0;
    RenderListPrev = 0;
    UpdateListNext = 0;
    UpdateListPrev = 0;
    SerialisationSectionName = GetDefaultSerialisationSectionName();
    LevelPersistent = false;
    if (DefGlobalIndex > 0)
        g_pThingManager->DefinitionManager->GetDef(DefGlobalIndex, &PBaseDef);
    CreateUID();
    ThingTypeFlags = g_pThingManager->GetThingTypeFlags(ThingClass);
    if (PlayerNumber == 5 || PlayerNumber < 0 || PlayerNumber >= 4)
        PlayerNumber = 4;
    if (PBaseDef.m_p) {
        OnBaseDefSet(PBaseDef.m_p);
        { CCharString name = g_DefStringTable.GetString(PBaseDef.m_p->DefaultScriptName); ScriptName = name; }
        { CCharString data = g_DefStringTable.GetString(PBaseDef.m_p->DefaultScriptData); ScriptData = data; }
    }
    if (g_pThingManager->Component->IsEditorRunning()) {
        CCharString editor("CTCEditor", -1);
        AddTC(editor, false, 0);
    }
    {
        CCharString mapwho("CTCMapwho", -1);
        AddTC(mapwho, false, 0);
    }
    return true;
}
static int Fail() { std::printf("004ca010_TEST FAIL\n"); return 1; }
int main()
{
    CMainGameComponent comp; comp.editor = false;
    CGameDefinitionManager defs;
    CThingManager mgr; mgr.Component = &comp; mgr.DefinitionManager = &defs;
    g_pThingManager = &mgr;
    CThing dummy;
    CThing t; t.ThingClass = 3; t.PBaseDef.m_p = 0; t.seenDef = 0; t.LevelPersistent = 1; t.RenderListNext = &dummy;
    g_log[0] = 0;
    if (!t.ConstructFromParams(7, 2, 11, true)) return Fail();
    if (std::strcmp(g_log, "rereg;getdef;uid;onbasedef;CTCMapwho;") != 0) return Fail();
    if (t.DefGlobalIndex != 7 || t.PlayerNumber != 2 || t.CurrentMapNumber != 11 || t.WorldFrameCreatedAt != 1234
        || t.RenderListNext || t.LevelPersistent || t.ThingTypeFlags != 0x103 || t.seenDef != &g_def
        || std::strcmp(t.ScriptName.text, "ScriptA") != 0 || std::strcmp(t.ScriptData.text, "DataB") != 0
        || std::strcmp(t.SerialisationSectionName.text, "default") != 0) return Fail();
    comp.editor = true;
    CThing u; u.ThingClass = 3; u.PBaseDef.m_p = 0; u.seenDef = 0;
    g_log[0] = 0;
    u.ConstructFromParams(0, 5, -1, false);
    if (std::strcmp(g_log, "uid;CTCEditor;CTCMapwho;") != 0 || u.PlayerNumber != 4 || u.seenDef) return Fail();
    u.ConstructFromParams(0, -2, 0, false);
    if (u.PlayerNumber != 4) return Fail();
    u.ConstructFromParams(0, 3, 0, false);
    if (u.PlayerNumber != 3) return Fail();
    std::printf("004ca010_TEST PASS\n");
    return 0;
}