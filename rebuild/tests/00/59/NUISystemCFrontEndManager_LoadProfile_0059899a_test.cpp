#include <cstdio>
class CWideString {
public:
    void* PStringData;
};
class CCharString {
public:
    void* PStringData;
    CCharString(const char* s, long length = -1);        // 0x0099EBF0
    ~CCharString();                                      // 0x0099EAE0
};
template <class T> class CTBaseSingleton {
public:
    static T& __fastcall Get();                          // 0x0040D2A0 for CUserProfileManager
};
class CUserProfileManager {
public:
    unsigned char _pad00[0xc];
    bool          UseNewProfileValues;                   // +0x0c
    void SetProfileName(const CWideString& name, bool load);   // 0x0040D400
};
namespace std {
template <class T> class vector {
public:
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    vector() : _M_start(0), _M_finish(0), _M_end_of_storage(0) {}
    ~vector();                                           // 0x00412130 for CWideString
    bool empty() const { return _M_start == _M_finish; }
};
}
namespace NUISystem {
class CFrontEndManager {
protected:
    void GetSavedGameNames(std::vector<CWideString>& names);   // 0x00597DF7
    void SetMainMenuDefName(const CCharString& name);          // 0x00595A06
    void AddProfileNameToScreens(const CWideString& name);     // 0x00595B24
    void LoadProfile(const CWideString& name);
    friend struct CFrontEndManager_Test;                 // behaviour-test hook
};
void CFrontEndManager::LoadProfile(const CWideString& name)
{
    CTBaseSingleton<CUserProfileManager>::Get().UseNewProfileValues = false;
    CTBaseSingleton<CUserProfileManager>::Get().SetProfileName(name, true);
    std::vector<CWideString> saves;
    GetSavedGameNames(saves);
    if (saves.empty())
    {
        CCharString def_name("UI_FRONTEND_MAIN_MENU_NO_LIVEAWARE_NO_CONTINUE");
        SetMainMenuDefName(def_name);
    }
    else
    {
        CCharString def_name("UI_FRONTEND_MAIN_MENU_NO_LIVEAWARE");
        SetMainMenuDefName(def_name);
    }
    AddProfileNameToScreens(name);
}
}

static unsigned char g_upmRaw[0x140]; static int g_gets, g_setName, g_setLoad = -1; static const CWideString* g_setArg;
template <class T> T& __fastcall CTBaseSingleton<T>::Get() { ++g_gets; return *(T*)g_upmRaw; }
void CUserProfileManager::SetProfileName(const CWideString& n, bool load) { ++g_setName; g_setArg = &n; g_setLoad = load; }
static int g_ctors, g_dtors; static const char* g_lastLiteral; static long g_lastLen;
CCharString::CCharString(const char* s, long len) { PStringData = (void*)s; g_lastLiteral = s; g_lastLen = len; ++g_ctors; }
CCharString::~CCharString() { ++g_dtors; }
static int g_vdtors; static int g_saveCount;
template <class T> std::vector<T>::~vector() { ++g_vdtors; }
static CWideString g_names[2];
namespace NUISystem {
static const char* g_menu; static const CWideString* g_shown; static int g_order, g_menuOrder, g_shownOrder;
void CFrontEndManager::GetSavedGameNames(std::vector<CWideString>& v) { v._M_start = g_names; v._M_finish = g_names + g_saveCount; }
void CFrontEndManager::SetMainMenuDefName(const CCharString& n) { g_menu = (const char*)n.PStringData; g_menuOrder = ++g_order; }
void CFrontEndManager::AddProfileNameToScreens(const CWideString& n) { g_shown = &n; g_shownOrder = ++g_order; }
struct CFrontEndManager_Test { static void Load(CFrontEndManager& m, const CWideString& n) { m.LoadProfile(n); } };
}
static bool Eq(const char* a, const char* b) { while (*a && *a == *b) { ++a; ++b; } return *a == *b; }
int main()
{
    using namespace NUISystem;
    CUserProfileManager& upm = *(CUserProfileManager*)g_upmRaw; upm.UseNewProfileValues = true;
    CFrontEndManager m; CWideString name; name.PStringData = 0;
    g_saveCount = 0; CFrontEndManager_Test::Load(m, name);
    bool ok = !upm.UseNewProfileValues && g_gets == 2 && g_setName == 1 && g_setArg == &name && g_setLoad == 1 &&
              Eq(g_menu, "UI_FRONTEND_MAIN_MENU_NO_LIVEAWARE_NO_CONTINUE") && g_lastLen == -1 && g_ctors == 1 && g_dtors == 1 &&
              g_shown == &name && g_menuOrder < g_shownOrder && g_vdtors == 1;
    g_saveCount = 2; CFrontEndManager_Test::Load(m, name);
    ok = ok && Eq(g_menu, "UI_FRONTEND_MAIN_MENU_NO_LIVEAWARE") && g_ctors == 2 && g_dtors == 2 && g_vdtors == 2;
    std::printf(ok ? "0059899a_TEST PASS\n" : "0059899a_TEST FAIL\n");
    return ok ? 0 : 1;
}