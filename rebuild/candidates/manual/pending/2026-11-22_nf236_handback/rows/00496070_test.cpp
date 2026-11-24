// Test for 0x00496070 CFrontendGameComponent::CFrontendGameComponent(CGame&, const CFrontendGameComponentInit&) on the shared headers
// (NF236, replaces the HEAD host model): the row's defines, includes, globals, inline helpers and body, verbatim. Stubbed (recording):
// the CGameComponent base ctor / dtor (0x0051D8E0; stores Quit / Running / Game), CTexture::Uninitialise, CWideString's default / text ctors and dtor,
// copy ctor, GFGetTime, CUserProfileManager::Get / GetDefaultProfileName / SetProfileName, GetProfileNames / GetSaveNames / GetWorldNames,
// InitGraphics, and the class's other out-of-line virtuals. The object is placement-constructed in a 0xCC-filled buffer of the class
// size and read at the CTOR_DTOR offsets.
// Case 1 (default-profile flag set): the default name is fetched once and that same temporary is handed to SetProfileName with
// load = true; mode 1, CursorHighlight 1, String zeroed, StringPos 0, no profile-list refresh; the counters / flags are zeroed, the
// movement time is GFGetTime's, InitGraphics runs once, and the key table holds the keypad / top-row digits and DIK_A / DIK_B.
// Case 2 (flag clear): no profile load; mode 12 refreshes ProfileNames (the member at +0x1E0C) once and zeroes ProfileNamesOffset.
#include <cstdio>
#include <cstring>
#include <new>
#include <string.h>
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CTEXTURE
#define FABLE_RETAIL_CUSERPROFILEMANAGER_DEFAULTPROFILENAME
#define FABLE_RETAIL_CFRONTENDGAMECOMPONENT_GETPROFILENAMES
#define FABLE_RETAIL_CFRONTENDGAMECOMPONENT_GETSAVENAMES
#define FABLE_RETAIL_CFRONTENDGAMECOMPONENT_SETQUIT
#define FABLE_RETAIL_CFRONTENDGAMECOMPONENT_CTOR_DTOR
#include "engine/CCountedPointer.h"
#include "stlport/fable_containers.h"
#include "fable_string.h"
#include "engine/CTexture.h"
#include "engine/GFGetTime.h"
#include "engine/CUserProfileManager.h"
#include "engine/CGameComponent.h"
#include "engine/CFrontendGameComponent.h"

extern bool g_bLoadDefaultProfile;                         // 0x013B8644
extern long g_KeyToNumber[122];                            // 0x013B87B0
bool g_bLoadDefaultProfile = false;                        // 0x013B8644
long g_KeyToNumber[122];                                   // 0x013B87B0
static int g_getDefault = 0, g_setProfile = 0, g_setLoad = -1, g_profileNames = 0, g_initGraphics = 0;
static const void* g_defaultMade = 0; static const void* g_setArg = 0; static const void* g_profileVec = 0;
static int g_textCtors = 0;
CBaseClass::CBaseClass() {}
CBaseClass::~CBaseClass() {}
CBaseClassNonCopyable::CBaseClassNonCopyable() {}
CBaseClassNonCopyable::~CBaseClassNonCopyable() {}
CGameComponent::CGameComponent(CGame& game) : Quit(false), Running(false), Game(game) {}
CGameComponent::~CGameComponent() {}
void CGameComponent::Init() {}
void CTexture::Uninitialise() {}
void CGameComponent::SetQuit() {}
long CWideString::NoStrings = 0;
CWideString::CWideString() : PStringData(0) {}
CWideString::CWideString(const wchar_t*) : PStringData(0) { ++g_textCtors; g_defaultMade = this; }
CWideString::CWideString(const CWideString&) : PStringData(0) {}
CWideString::~CWideString() {}
const CWideString& CWideString::operator=(const wchar_t*) { return *this; }
double FABLE_FASTCALL GFGetTime() { return 12.5; }
static unsigned char g_upm[sizeof(CUserProfileManager)];
CUserProfileManager& __fastcall CUserProfileManager::Get() { return *(CUserProfileManager*)g_upm; }
CWideString CUserProfileManager::GetDefaultProfileName() const { ++g_getDefault; return L"Default"; }
void CUserProfileManager::SetProfileName(const CWideString& name, bool load) { ++g_setProfile; g_setArg = &name; g_setLoad = load; }
void __fastcall CFrontendGameComponent::GetProfileNames(std::vector<CWideString>& names) { ++g_profileNames; g_profileVec = &names; }
void __fastcall CFrontendGameComponent::GetSaveNames(std::vector<CWideString>&) {}
void CFrontendGameComponent::GetWorldNames() {}
bool CFrontendGameComponent::InitGraphics() { ++g_initGraphics; return true; }
bool CFrontendGameComponent::Run(CGameComponent**) { return false; }
void CFrontendGameComponent::ChangeTextureColourDepth(long) {}
void CFrontendGameComponent::OnPreDeviceReset() {}
bool CFrontendGameComponent::OnPostDeviceReset() { return true; }
void CFrontendGameComponent::SetQuit() {}
CFrontendGameComponent::~CFrontendGameComponent() {}
inline void CFrontendGameComponent::LoadProfile(const CWideString& profileName, bool loadProfile)
{
    CUserProfileManager::Get().SetProfileName(profileName, loadProfile);
}

inline void CFrontendGameComponent::SetMode(EGameMode mode)
{
    Mode = mode;
    CursorHighlight = 1;
    memset(String, 0, sizeof(String));
    StringPos = 0;
    if (Mode == 10) {
        GetWorldNames();
        return;
    }
    if (mode == 11) {
        GetSaveNames(SaveNames);
        WorldToLoad = L"";
        SaveNamesOffset = 0;
        return;
    }
    if (mode == 12) {
        GetProfileNames(ProfileNames);
        ProfileNamesOffset = 0;
    }
}

CFrontendGameComponent::CFrontendGameComponent(CGame& game, const CFrontendGameComponentInit& init)
    : CGameComponent(game), CursorHighlight(1), TimeLastMovementHappened(GFGetTime()), StartedRepeatingMovement(false),
      CurrentMovement(NGameComponentHelpers::ECursorMovement())
{
    Mode = EGameMode();
    Quit = false;
    NoConnections = 0;
    NoSessions = 0;
    FirstSessionString = 0;
    LastSessionString = 0;
    NoSessionStrings = 0;
    if (g_bLoadDefaultProfile) {
        LoadProfile(CUserProfileManager::Get().GetDefaultProfileName(), true);
        SetMode(EGameMode(1));
    } else {
        SetMode(EGameMode(12));
    }
    Quit = false;
    CreateGame = false;
    CreateGameJoystickDeviceNumber = 0;
    CreateEditor = false;
    CreateReleaseFrontEnd = false;
    InitGraphics();
    memset(g_KeyToNumber, 0, sizeof(g_KeyToNumber));
    g_KeyToNumber[82] = 0;
    g_KeyToNumber[79] = 1;
    g_KeyToNumber[80] = 2;
    g_KeyToNumber[81] = 3;
    g_KeyToNumber[75] = 4;
    g_KeyToNumber[76] = 5;
    g_KeyToNumber[77] = 6;
    g_KeyToNumber[71] = 7;
    g_KeyToNumber[72] = 8;
    g_KeyToNumber[73] = 9;
    g_KeyToNumber[11] = 0;
    g_KeyToNumber[2] = 1;
    g_KeyToNumber[3] = 2;
    g_KeyToNumber[4] = 3;
    g_KeyToNumber[5] = 4;
    g_KeyToNumber[6] = 5;
    g_KeyToNumber[7] = 6;
    g_KeyToNumber[8] = 7;
    g_KeyToNumber[9] = 8;
    g_KeyToNumber[10] = 9;
    g_KeyToNumber[30] = 11;
    g_KeyToNumber[48] = 12;
}static double g_raw[sizeof(CFrontendGameComponent) / sizeof(double) + 1];
static unsigned char* B() { return (unsigned char*)g_raw; }
static long L(unsigned off) { long v; std::memcpy(&v, B() + off, 4); return v; }
static bool Zero(unsigned off, unsigned n) { for (unsigned i = 0; i < n; ++i) if (B()[off + i] != 0) return false; return true; }
static bool Common(CGame* game)
{
    double t; std::memcpy(&t, B() + 0x1E48, 8);
    void* g; std::memcpy(&g, B() + 0x0C, 4);
    bool ok = B()[8] == 0 && B()[9] == 0 && g == (void*)game;                      // Quit, Running, Game
    ok = ok && B()[0x10] == 0 && L(0x14) == 0 && B()[0x18] == 0 && B()[0x19] == 0;  // CreateGame .. CreateReleaseFrontEnd
    ok = ok && L(0x20) == 0 && L(0x24) == 0 && L(0x1DF0) == 0 && L(0x1DF4) == 0 && L(0x1DF8) == 0;
    ok = ok && Zero(0x168, 128) && L(0x1E8) == 0 && L(0x1E44) == 1 && t == 12.5 && B()[0x1E50] == 0 && L(0x1E54) == 0;
    ok = ok && Zero(0x1DFC, 16) && Zero(0x1E2C, 4) && Zero(0x1E40, 4);                // empty counted pointers / strings
    static const int key[][2] = { {82,0},{79,1},{80,2},{81,3},{75,4},{76,5},{77,6},{71,7},{72,8},{73,9},
                                  {11,0},{2,1},{3,2},{4,3},{5,4},{6,5},{7,6},{8,7},{9,8},{10,9},{30,11},{48,12} };
    int set = 0;
    for (int i = 0; i < 22; ++i) if (g_KeyToNumber[key[i][0]] != key[i][1]) ok = false;
    for (int k = 0; k < 122; ++k) if (g_KeyToNumber[k] != 0) ++set;
    return ok && set == 20 && g_initGraphics == 1;
}
int main()
{
    int fails = 0;
    static unsigned char gameBuf[16]; CGame& game = *(CGame*)gameBuf;
    CFrontendGameComponentInit init;
    std::memset(g_raw, 0xCC, sizeof(g_raw)); std::memset(g_KeyToNumber, 0x77, sizeof(g_KeyToNumber));
    g_bLoadDefaultProfile = true;
    new (g_raw) CFrontendGameComponent(game, init);
    if (g_getDefault != 1 || g_setProfile != 1 || g_setLoad != 1 || g_setArg != g_defaultMade || g_textCtors != 1) fails |= 1;
    if (L(0x1C) != 1 || g_profileNames != 0 || !Common(&game)) fails |= 2;
    std::memset(g_raw, 0xCC, sizeof(g_raw)); std::memset(g_KeyToNumber, 0x77, sizeof(g_KeyToNumber));
    g_bLoadDefaultProfile = false; g_initGraphics = 0;
    new (g_raw) CFrontendGameComponent(game, init);
    if (g_getDefault != 1 || g_setProfile != 1) fails |= 4;
    if (L(0x1C) != 12 || g_profileNames != 1 || g_profileVec != B() + 0x1E0C || L(0x1E18) != 0 || !Common(&game)) fails |= 8;
    if (fails) { std::printf("00496070_TEST FAIL %d\n", fails); return 1; }
    std::printf("00496070_TEST PASS\n");
    return 0;
}
