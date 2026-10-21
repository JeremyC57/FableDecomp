#include <cstdio>
class CWideString {
public:
    void* PStringData;
    ~CWideString();                                      // 0x0099B510
    long GetLength() const;                              // 0x0099B220
};
class CCharString {
public:
    void* PStringData;
    CCharString(const char* text, long length = -1);     // 0x0099EBF0
    ~CCharString();                                      // 0x0099EAE0
};
class CUserProfileManager {
public:
    unsigned char _pad00[0x0c];
    bool          UseNewProfileValues;                   // +0x0c
    unsigned char _pad0d[0x10 - 0x0d];
    bool          CameraYZInverted;                      // +0x10
    unsigned char _pad11[0xd0 - 0x11];
    bool          CameraYZInvertedNewProfile;            // +0xd0
    bool IsEnoughSpaceForProfile() const;                // 0x004067C0
    CWideString GetProfileDir(const CWideString& name, bool create);   // 0x004082D0
    void SetProfileName(const CWideString& name, bool load);           // 0x0040D400
    void SetDefaultValues();                             // 0x0040C790
    void ApplyNewProfileValues();                        // 0x0040CCA0
    bool IsCameraYZInverted() const { return UseNewProfileValues ? CameraYZInvertedNewProfile : CameraYZInverted; }
    void SetCameraYZInverted(bool inverted, bool apply); // 0x0040D2F0
    void Save();                                         // 0x0040A740
};
template <class T> class CTBaseSingleton {
public:
    static T& __fastcall Get();                          // 0x0040D2A0 for CUserProfileManager
};
namespace NUISystem {
class CFrontEndManager {
public:
    void SetMainMenuDefName(const CCharString& name);    // 0x00595A06
    void AddProfileNameToScreens(const CWideString& name);   // 0x00595B24
    bool CreateNewProfile(CWideString& name);
};
bool CFrontEndManager::CreateNewProfile(CWideString& name)
{
    bool ok = true;
    if (!CTBaseSingleton<CUserProfileManager>::Get().IsEnoughSpaceForProfile())
        return false;
    CTBaseSingleton<CUserProfileManager>::Get().UseNewProfileValues = false;
    if (CTBaseSingleton<CUserProfileManager>::Get().GetProfileDir(name, true).GetLength() == 0)
        ok = false;
    CTBaseSingleton<CUserProfileManager>::Get().SetProfileName(name, false);
    CTBaseSingleton<CUserProfileManager>::Get().SetDefaultValues();
    CTBaseSingleton<CUserProfileManager>::Get().ApplyNewProfileValues();
    CTBaseSingleton<CUserProfileManager>::Get().SetCameraYZInverted(CTBaseSingleton<CUserProfileManager>::Get().IsCameraYZInverted(), true);
    CTBaseSingleton<CUserProfileManager>::Get().Save();
    {
        CCharString def("UI_FRONTEND_MAIN_MENU_NO_LIVEAWARE_NO_CONTINUE");
        SetMainMenuDefName(def);
    }
    AddProfileNameToScreens(name);
    return ok;
}
}

#include <stdio.h>
#include <string.h>
static int g_log[32]; static int g_n;
static void L(int x) { if (g_n < 32) g_log[g_n++] = x; }
static int g_wdtors, g_cdtors;
CWideString::~CWideString() { ++g_wdtors; }
static long g_dirLen;
long CWideString::GetLength() const { return g_dirLen; }
static const char* g_defText; static long g_defLen;
CCharString::CCharString(const char* t, long n) { g_defText = t; g_defLen = n; PStringData = 0; }
CCharString::~CCharString() { ++g_cdtors; L(9); }
static CUserProfileManager g_upm;
template <class T> T& __fastcall CTBaseSingleton<T>::Get() { return g_upm; }
static bool g_space;
bool CUserProfileManager::IsEnoughSpaceForProfile() const { L(1); return g_space; }
static int g_dirCreate = -1;
CWideString CUserProfileManager::GetProfileDir(const CWideString& n, bool c) { L(2); g_dirCreate = c ? 1 : 0; CWideString r; r.PStringData = 0; return r; }
static int g_load = -1;
void CUserProfileManager::SetProfileName(const CWideString& n, bool l) { L(3); g_load = l ? 1 : 0; }
void CUserProfileManager::SetDefaultValues() { L(4); }
void CUserProfileManager::ApplyNewProfileValues() { L(5); UseNewProfileValues = true; }
static int g_inv = -1, g_apply = -1;
void CUserProfileManager::SetCameraYZInverted(bool i, bool a) { L(6); g_inv = i ? 1 : 0; g_apply = a ? 1 : 0; }
void CUserProfileManager::Save() { L(7); }
static int g_menuSet;
void NUISystem::CFrontEndManager::SetMainMenuDefName(const CCharString& n) { L(8); ++g_menuSet; }
static const CWideString* g_added;
void NUISystem::CFrontEndManager::AddProfileNameToScreens(const CWideString& n) { L(10); g_added = &n; }
static NUISystem::CFrontEndManager g_fem;
int main()
{
    static CWideString name;
    g_space = false;
    bool r = g_fem.CreateNewProfile(name);
    bool ok = !r && g_n == 1;
    g_n = 0; g_space = true; g_dirLen = 5; g_upm.UseNewProfileValues = true; g_upm.CameraYZInverted = false; g_upm.CameraYZInvertedNewProfile = true;
    r = g_fem.CreateNewProfile(name);
    static const int want[] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
    ok = ok && r && g_n == 10 && memcmp(g_log, want, sizeof(want)) == 0 && g_dirCreate == 1 && g_load == 0 && g_inv == 1 && g_apply == 1
        && g_added == &name && g_defLen == -1 && strcmp(g_defText, "UI_FRONTEND_MAIN_MENU_NO_LIVEAWARE_NO_CONTINUE") == 0;
    g_n = 0; g_dirLen = 0; g_upm.CameraYZInvertedNewProfile = false; g_upm.CameraYZInverted = true;
    r = g_fem.CreateNewProfile(name);      // empty folder path: still runs, returns false; ApplyNewProfileValues sets UseNew -> reads the NewProfile flag
    ok = ok && !r && g_n == 10 && g_inv == 0;
    printf(ok ? "0059697a_TEST PASS\n" : "0059697a_TEST FAIL\n");
    return ok ? 0 : 1;
}