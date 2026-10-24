// Behaviour test for NUISystem::CFrontEndManager::DoPressStart (0x00599D5C): no profiles -> new-profile screen; one
// profile whose signature verifies -> load it and go to screen 0 clearing the stack; otherwise -> profile menu and
// re-apply the profile manager values. The joystick hand-over happens on every path, the GetProfileDir temporary
// is destroyed exactly when it was built, and the profile list is destroyed at the end.
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
inline void* operator new(unsigned int, void* p) { return p; }

static int g_strings = 0;
static char g_log[256];
static void Log(const char* s) { strcat(g_log, s); strcat(g_log, ";"); }

class CWideString
{
public:
    void* PData;
    CWideString() : PData(0) { ++g_strings; }
    CWideString(const CWideString& o) : PData(o.PData) { ++g_strings; }
    ~CWideString() { --g_strings; }
};

namespace std {
template <class T> class vector
{
public:
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    vector() : _M_start(0), _M_finish(0), _M_end_of_storage(0) {}
    ~vector() { for (T* p = _M_start; p != _M_finish; ++p) p->~T(); free(_M_start); Log("dtor"); }
    T* begin() const { return _M_start; }
    bool empty() const { return _M_start == _M_finish; }
    unsigned int size() const { return (unsigned int)(_M_finish - _M_start); }
    T& operator[](unsigned int n) const { return *(begin() + n); }
};
template <class K, class V> class map
{
public:
    V values[4];
    V& operator[](const K& key) { return values[key]; }
};
}

static bool g_verifies = false;
static int g_profileCount = 0;

class CUserProfileManager
{
public:
    unsigned char _pad00[0x0c];
    bool UseNewProfileValues;
    CWideString GetProfileDir(const CWideString& name, bool create) { Log(create ? "dir+" : "dir"); return name; }
    void Apply() { Log("apply"); }
};

template <class T> class CTBaseSingleton
{
public:
    static T& __fastcall Get() { static T instance; return instance; }
};

static long g_joystick = -1;
class CGameJoystickManager
{
public:
    static CGameJoystickManager& __fastcall Get() { static CGameJoystickManager m; return m; }
    void SetMainPlayerJoystickDeviceNumber(long device) { g_joystick = device; }
};

struct CNewFrontendGameComponent
{
    unsigned char _pad[0x140];
    long LastActiveJoystickDeviceNumber;
};
static CNewFrontendGameComponent g_frontend;
CNewFrontendGameComponent* g_pNewFrontendGameComponent = &g_frontend;

enum FRONTEND_SCREEN { FRONTEND_SCREEN_0 = 0 };

namespace NUISystem {
class CComponent { public: int id; };
class CFrontEndManager
{
public:
    unsigned char                          _pad00[0x54];
    std::map<FRONTEND_SCREEN, CComponent*> m_mapScreens;
    CComponent* lastScreen; bool lastClear;
    void GetProfileNames(std::vector<CWideString>& names)
    {
        names._M_start = (CWideString*)malloc(4 * sizeof(CWideString));
        for (int i = 0; i < g_profileCount; ++i) new (&names._M_start[i]) CWideString();
        names._M_finish = names._M_start + g_profileCount;
        names._M_end_of_storage = names._M_start + 4;
    }
    void GotoNewProfileScreen() { Log("new"); }
    bool VerifySignatureCached(const CWideString&) { Log("verify"); return g_verifies; }
    void LoadProfile(const CWideString&) { Log("load"); }
    void GotoNextScreen(CComponent* screen, bool clearStack) { lastScreen = screen; lastClear = clearStack; Log("next"); }
    void GotoProfileMenu() { Log("menu"); }
    void DoPressStart();
};

void CFrontEndManager::DoPressStart()
{
    std::vector<CWideString> profiles;
    GetProfileNames(profiles);
    if (profiles.empty())
    {
        GotoNewProfileScreen();
    }
    else
    {
        bool loadDirectly = profiles.size() == 1
            && VerifySignatureCached(CTBaseSingleton<CUserProfileManager>::Get().GetProfileDir(profiles[0], false));
        if (loadDirectly)
        {
            LoadProfile(profiles[0]);
            GotoNextScreen(m_mapScreens[FRONTEND_SCREEN_0], true);
        }
        else
        {
            GotoProfileMenu();
            CTBaseSingleton<CUserProfileManager>::Get().UseNewProfileValues = true;
            CTBaseSingleton<CUserProfileManager>::Get().Apply();
        }
    }
    CGameJoystickManager::Get().SetMainPlayerJoystickDeviceNumber(g_pNewFrontendGameComponent->LastActiveJoystickDeviceNumber);
}
}

static int Run(int count, bool verifies, const char* want, long joystick)
{
    NUISystem::CFrontEndManager fe;
    NUISystem::CComponent screen0;
    fe.m_mapScreens.values[0] = &screen0;
    fe.lastScreen = 0; fe.lastClear = false;
    g_log[0] = 0; g_profileCount = count; g_verifies = verifies; g_joystick = -1;
    g_frontend.LastActiveJoystickDeviceNumber = joystick;
    CTBaseSingleton<CUserProfileManager>::Get().UseNewProfileValues = false;
    fe.DoPressStart();
    if (strcmp(g_log, want) != 0) { printf("FAIL %d: %s\n", count, g_log); return 0; }
    if (g_joystick != joystick || g_strings != 0) { printf("FAIL %d joystick/strings\n", count); return 0; }
    if (strstr(want, "next") && (fe.lastScreen != &screen0 || !fe.lastClear)) { printf("FAIL screen\n"); return 0; }
    if ((strstr(want, "apply") != 0) != CTBaseSingleton<CUserProfileManager>::Get().UseNewProfileValues) { printf("FAIL flag\n"); return 0; }
    return 1;
}

int main()
{
    if (!Run(0, true, "new;dtor;", 3)) return 1;
    if (!Run(1, true, "dir;verify;load;next;dtor;", 1)) return 1;
    if (!Run(1, false, "dir;verify;menu;apply;dtor;", 2)) return 1;
    if (!Run(2, true, "menu;apply;dtor;", 0)) return 1;
    printf("FSE2_00599d5c_TEST PASS\n");
    return 0;
}
