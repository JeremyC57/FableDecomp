#include <cstdio>
#include <string.h>
class CWideString {
public:
    void* PStringData;
    ~CWideString();                                      // 0x0099B510
};
class CUserProfileManager {
public:
    CWideString GetDefaultProfileName() const;           // 0x004065D0
    void SetProfileName(const CWideString& name, bool load);   // 0x0040D400
};
template <class T> class CTBaseSingleton {
public:
    static T& __fastcall Get();                          // 0x0040D2A0 for CUserProfileManager
};
namespace std {
template <class T> class vector {
public:
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    unsigned int size() const { return unsigned int(_M_finish - _M_start); }
    T* begin() { return _M_start; }
    T& operator[](unsigned int n) { return *(begin() + n); }
};
}
enum EGameMode { GAME_MODE_1 = 1, GAME_MODE_10 = 10 };
class CFrontendGameComponent {
public:
    unsigned char            _pad0000[0x1c];
    EGameMode                Mode;                       // +0x1c
    unsigned char            _pad0020[0x168 - 0x20];
    char                     String[128];                // +0x168
    long                     StringPos;                  // +0x1e8
    unsigned char            _pad01ec[0x1e0c - 0x1ec];
    std::vector<CWideString> ProfileNames;               // +0x1e0c
    long                     ProfileNamesOffset;         // +0x1e18
    unsigned char            _pad1e1c[0x1e44 - 0x1e1c];
    long                     CursorHighlight;            // +0x1e44
    CWideString CreateNewProfile();                      // 0x004945B0
    void GetWorldNames();                                // 0x00495890
    void ClearString() { memset(String, 0, sizeof(String)); StringPos = 0; }
    __forceinline void SetMode(EGameMode mode)
    {
        Mode = mode;
        CursorHighlight = 1;
        ClearString();
        if (Mode == GAME_MODE_10)
            GetWorldNames();
    }
    __forceinline void LoadProfile(const CWideString& name, bool load)
    {
        CTBaseSingleton<CUserProfileManager>::Get().SetProfileName(name, load);
    }
    void ChooseProfileNumber(long number);
};
void CFrontendGameComponent::ChooseProfileNumber(long number)
{
    long index = ProfileNamesOffset + number - 1;
    if (index == 11)
    {
        LoadProfile(CTBaseSingleton<CUserProfileManager>::Get().GetDefaultProfileName(), true);
        SetMode(GAME_MODE_1);
    }
    else if (index == 12)
    {
        LoadProfile(CreateNewProfile(), false);
        SetMode(GAME_MODE_1);
    }
    else if (index >= 0 && index < ProfileNames.size())
    {
        LoadProfile(ProfileNames[index], true);
        SetMode(GAME_MODE_1);
    }
}

#include <stdio.h>
static int g_dtors;
CWideString::~CWideString() { ++g_dtors; }
static int g_defTag, g_newTag;
static CUserProfileManager g_upm;
template <class T> T& __fastcall CTBaseSingleton<T>::Get() { return g_upm; }
CWideString CUserProfileManager::GetDefaultProfileName() const { CWideString r; r.PStringData = &g_defTag; return r; }
static void* g_setName; static int g_setLoad = -1; static int g_sets;
void CUserProfileManager::SetProfileName(const CWideString& n, bool l) { g_setName = n.PStringData; g_setLoad = l ? 1 : 0; ++g_sets; }
CWideString CFrontendGameComponent::CreateNewProfile() { CWideString r; r.PStringData = &g_newTag; return r; }
static int g_worlds;
void CFrontendGameComponent::GetWorldNames() { ++g_worlds; }
static CFrontendGameComponent g_c;
static CWideString g_names[3];
static int g_tags[3];
static void Reset() { g_c.Mode = (EGameMode)5; g_c.CursorHighlight = 7; g_c.StringPos = 9; for (int i = 0; i < 128; ++i) g_c.String[i] = 'x'; }
static bool Cleared() { for (int i = 0; i < 128; ++i) if (g_c.String[i]) return false; return g_c.Mode == 1 && g_c.CursorHighlight == 1 && g_c.StringPos == 0; }
int main()
{
    for (int i = 0; i < 3; ++i) g_names[i].PStringData = &g_tags[i];
    g_c.ProfileNames._M_start = g_names; g_c.ProfileNames._M_finish = g_names + 3; g_c.ProfileNames._M_end_of_storage = g_names + 3;
    g_c.ProfileNamesOffset = 2;
    Reset(); g_c.ChooseProfileNumber(10);     // index 11: default profile, load
    bool ok = g_sets == 1 && g_setName == &g_defTag && g_setLoad == 1 && Cleared() && g_worlds == 0 && g_dtors >= 1;
    int d = g_dtors;
    Reset(); g_c.ChooseProfileNumber(11);     // index 12: new profile, no load
    ok = ok && g_sets == 2 && g_setName == &g_newTag && g_setLoad == 0 && Cleared() && g_dtors > d;
    Reset(); g_c.ChooseProfileNumber(0);      // index 1: listed profile
    ok = ok && g_sets == 3 && g_setName == &g_tags[1] && g_setLoad == 1 && Cleared();
    Reset(); g_c.ChooseProfileNumber(5);      // index 6: out of range, nothing
    ok = ok && g_sets == 3 && g_c.Mode == 5 && g_c.StringPos == 9;
    Reset(); g_c.ChooseProfileNumber(-5);     // negative, nothing
    ok = ok && g_sets == 3 && g_c.CursorHighlight == 7;
    printf(ok ? "004962c0_TEST PASS\n" : "004962c0_TEST FAIL\n");
    for (int i = 0; i < 3; ++i) g_names[i].PStringData = 0;
    return ok ? 0 : 1;
}