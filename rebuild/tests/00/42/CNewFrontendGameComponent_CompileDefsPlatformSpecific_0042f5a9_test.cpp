// Behaviour test for 0x0042F5A9 CNewFrontendGameComponent::CompileDefsPlatformSpecific: picks the file, installs a new
// startup manager, registers FRONT_END / UI_MISC_THINGS_DEF / UI_ICONS_DEF (+ CONTROL_SCHEME on PC) and compiles.
#include <cstdio>
#include <cstring>

static int g_live = 0;
class CWideString {
public:
    CWideString() { m_text = ""; ++g_live; }
    CWideString(const CWideString& o) { m_text = o.m_text; ++g_live; }
    ~CWideString() { --g_live; }
    const CWideString& operator=(const char* text) { m_text = text; return *this; }
    const char* m_text;
};
class CCharString {
public:
    CCharString(const char* text, long length = -1) { m_text = text; ++g_live; }
    CCharString(const CCharString& o) { m_text = o.m_text; ++g_live; }
    ~CCharString() { --g_live; }
    const char* m_text;
};
class CDefClassBase { public: int m_id; };
typedef CDefClassBase* (*TDefClassAllocFunc)();
class CDefClassInit {
public:
    CCharString Name;
    TDefClassAllocFunc AllocFunc;
    CDefClassInit(const CCharString& name, TDefClassAllocFunc allocFunc) : Name(name), AllocFunc(allocFunc) { }
};
class CDefinitionManager {
public:
    const char* m_names[8]; TDefClassAllocFunc m_allocs[8]; int m_count;
    CDefinitionManager() : m_count(0) {}
    void AddDefClass(const CDefClassInit& init) { m_names[m_count] = init.Name.m_text; m_allocs[m_count++] = init.AllocFunc; }
};
class CStartupDefinitionManager : public CDefinitionManager {
public:
    const char* m_file; int m_compiled; bool m_xbox;
    CStartupDefinitionManager(const CWideString& file) : m_file(file.m_text), m_compiled(0), m_xbox(false) {}
    static CStartupDefinitionManager* s_instance;
    static void Set(CStartupDefinitionManager* m) { delete s_instance; s_instance = m; }
    static CStartupDefinitionManager& Get() { return *s_instance; }
    void InitAndCompilePlatformSpecific(bool xbox) { ++m_compiled; m_xbox = xbox; }
};
CStartupDefinitionManager* CStartupDefinitionManager::s_instance = 0;

static CDefClassBase* AllocFrontEnd() { return 0; }
static CDefClassBase* AllocMisc() { return 0; }
static CDefClassBase* AllocIcons() { return 0; }
static CDefClassBase* AllocControls() { return 0; }

void CompileDefsPlatformSpecific(bool xbox)
{
    CWideString definitionsFile;
    if (xbox)
        definitionsFile = "xfrontend.bin";
    else
        definitionsFile = "frontend.bin";
    CStartupDefinitionManager::Set(new CStartupDefinitionManager(definitionsFile));
    CStartupDefinitionManager& manager = CStartupDefinitionManager::Get();
    manager.AddDefClass(CDefClassInit("FRONT_END", AllocFrontEnd));
    manager.AddDefClass(CDefClassInit("UI_MISC_THINGS_DEF", AllocMisc));
    manager.AddDefClass(CDefClassInit("UI_ICONS_DEF", AllocIcons));
    if (!xbox)
        manager.AddDefClass(CDefClassInit("CONTROL_SCHEME", AllocControls));
    manager.InitAndCompilePlatformSpecific(xbox);
}

int main()
{
    int ok = 1;
    CompileDefsPlatformSpecific(true);
    CStartupDefinitionManager* m = &CStartupDefinitionManager::Get();
    if (std::strcmp(m->m_file, "xfrontend.bin") != 0 || m->m_count != 3 || m->m_compiled != 1 || !m->m_xbox) ok = 0;
    if (std::strcmp(m->m_names[2], "UI_ICONS_DEF") != 0 || m->m_allocs[0] != AllocFrontEnd) ok = 0;
    CompileDefsPlatformSpecific(false);
    m = &CStartupDefinitionManager::Get();
    if (std::strcmp(m->m_file, "frontend.bin") != 0 || m->m_count != 4 || m->m_xbox) ok = 0;
    if (std::strcmp(m->m_names[3], "CONTROL_SCHEME") != 0 || m->m_allocs[3] != AllocControls) ok = 0;
    if (std::strcmp(m->m_names[1], "UI_MISC_THINGS_DEF") != 0 || m->m_allocs[1] != AllocMisc) ok = 0;
    if (g_live != 0) ok = 0;
    CStartupDefinitionManager::Set(0);
    if (!ok) { std::printf("0042f5a9_TEST FAIL\n"); return 1; }
    std::printf("0042f5a9_TEST PASS\n");
    return 0;
}