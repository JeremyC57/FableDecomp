// Behaviour test for 0x00f2a0f0 CScriptInfoManager::RegisterDefs
#include <cstdio>
#include <cstring>

static int g_live = 0;
struct CCharString {
    char text[32];
    CCharString(const char* t, long = -1) { std::strcpy(text, t); ++g_live; }
    CCharString(const CCharString& o) { std::strcpy(text, o.text); ++g_live; }
    ~CCharString() { --g_live; }
};
class CDefClassBase { public: int kind; };
struct CDefClassInit {
    CCharString Name; CDefClassBase* (*AllocFunc)();
    CDefClassInit(const CCharString& name, CDefClassBase* (*allocFunc)()) : Name(name), AllocFunc(allocFunc) { }
};
static CDefClassBase g_defs[3];
static CDefClassBase* AllocCScriptDef() { return &g_defs[0]; }
static CDefClassBase* AllocCCutsceneDef() { return &g_defs[1]; }
static CDefClassBase* AllocCRegionScriptDef() { return &g_defs[2]; }
class CDefinitionManager {
public:
    char names[3][32]; CDefClassBase* (*allocs[3])(); int count;
    void AddDefClass(const CDefClassInit& init) { std::strcpy(names[count], init.Name.text); allocs[count] = init.AllocFunc; ++count; }
};
static CDefinitionManager g_manager;
static CDefinitionManager* GFGetPDefinitionManager() { return &g_manager; }
struct CScriptInfoManager {
    void RegisterDefs()
    {
        CDefinitionManager* manager = GFGetPDefinitionManager();
        manager->AddDefClass(CDefClassInit("CScriptDef", AllocCScriptDef));
        manager->AddDefClass(CDefClassInit("CCutsceneDef", AllocCCutsceneDef));
        manager->AddDefClass(CDefClassInit("CRegionScriptDef", AllocCRegionScriptDef));
    }
};

int main()
{
    g_manager.count = 0;
    CScriptInfoManager m; m.RegisterDefs();
    if (g_manager.count != 3 || std::strcmp(g_manager.names[0], "CScriptDef") != 0 || std::strcmp(g_manager.names[1], "CCutsceneDef") != 0
        || std::strcmp(g_manager.names[2], "CRegionScriptDef") != 0 || g_manager.allocs[0]() != &g_defs[0]
        || g_manager.allocs[1]() != &g_defs[1] || g_manager.allocs[2]() != &g_defs[2] || g_live != 0) {
        std::printf("00f2a0f0_TEST FAIL\n"); return 1;
    }
    std::printf("00f2a0f0_TEST PASS\n");
    return 0;
}
