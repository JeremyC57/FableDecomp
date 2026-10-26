// Behaviour test for CFrontendGameComponent::CFrontendGameComponent (0x00496070): member defaults, the default-profile path
// (mode 1) versus the profile-list path (mode 12), and the key-to-number table.
#include <cstdio>
#include <cstring>
static int g_profileLoads, g_profileListRefreshes, g_initGraphics, g_prepareWorld; static bool g_lastLoadFlag;
static const char* g_lastProfile;
static bool g_bLoadDefaultProfile;
static int g_KeyToNumber[122];
class CUserProfileManager { public:
    const char* GetDefaultProfileName() const { return "Default"; }
    void SetProfileName(const char* name, bool load) { g_lastProfile = name; g_lastLoadFlag = load; ++g_profileLoads; }
    static CUserProfileManager& Get() { static CUserProfileManager m; return m; }
    static void LoadProfile(const char* name, bool load) { Get().SetProfileName(name, load); }
};
struct CFrontendGameComponent {
    bool m_bShutDown, CreateGame, CreateEditor, CreateReleaseFrontEnd;
    int CreateGameJoystickDeviceNumber, Mode, NoConnections, NoSessions;
    char String[128]; int StringPos;
    int FirstSessionString, LastSessionString, NoSessionStrings;
    int ProfileNamesOffset, SaveNamesOffset, CursorHighlight;
    double TimeLastMovementHappened; bool StartedRepeatingMovement; int CurrentMovement;
    void PrepareWorldList() { ++g_prepareWorld; }
    void InitGraphics() { ++g_initGraphics; }
    void SetMode(int mode)
    {
        Mode = mode;
        CursorHighlight = 1;
        std::memset(String, 0, sizeof(String));
        StringPos = 0;
        if (Mode == 10) { PrepareWorldList(); return; }
        if (mode == 11) { SaveNamesOffset = 0; return; }
        if (mode == 12) { ++g_profileListRefreshes; ProfileNamesOffset = 0; }
    }
    CFrontendGameComponent() : CursorHighlight(1), TimeLastMovementHappened(42.0), StartedRepeatingMovement(false), CurrentMovement(0)
    {
        Mode = 0; m_bShutDown = false; NoConnections = 0; NoSessions = 0;
        FirstSessionString = 0; LastSessionString = 0; NoSessionStrings = 0;
        StringPos = 7; String[0] = 'x'; ProfileNamesOffset = 9;
        if (g_bLoadDefaultProfile) {
            CUserProfileManager::LoadProfile(CUserProfileManager::Get().GetDefaultProfileName(), true);
            SetMode(1);
        } else {
            SetMode(12);
        }
        m_bShutDown = false; CreateGame = false; CreateGameJoystickDeviceNumber = 0; CreateEditor = false; CreateReleaseFrontEnd = false;
        InitGraphics();
        std::memset(g_KeyToNumber, 0, sizeof(g_KeyToNumber));
        g_KeyToNumber[82] = 0; g_KeyToNumber[79] = 1; g_KeyToNumber[80] = 2; g_KeyToNumber[81] = 3; g_KeyToNumber[75] = 4;
        g_KeyToNumber[76] = 5; g_KeyToNumber[77] = 6; g_KeyToNumber[71] = 7; g_KeyToNumber[72] = 8; g_KeyToNumber[73] = 9;
        g_KeyToNumber[11] = 0; g_KeyToNumber[2] = 1; g_KeyToNumber[3] = 2; g_KeyToNumber[4] = 3; g_KeyToNumber[5] = 4;
        g_KeyToNumber[6] = 5; g_KeyToNumber[7] = 6; g_KeyToNumber[8] = 7; g_KeyToNumber[9] = 8; g_KeyToNumber[10] = 9;
        g_KeyToNumber[30] = 11; g_KeyToNumber[48] = 12;
    }
};
int main()
{
    g_KeyToNumber[100] = 55;
    g_bLoadDefaultProfile = true;
    CFrontendGameComponent a;
    int ok = a.Mode == 1 && a.CursorHighlight == 1 && a.StringPos == 0 && a.String[0] == 0 && g_profileLoads == 1 && g_lastLoadFlag
        && std::strcmp(g_lastProfile, "Default") == 0 && g_profileListRefreshes == 0 && a.ProfileNamesOffset == 9 && g_initGraphics == 1
        && a.TimeLastMovementHappened == 42.0 && !a.CreateGame && a.NoSessionStrings == 0;
    g_bLoadDefaultProfile = false;
    CFrontendGameComponent b;
    ok = ok && b.Mode == 12 && g_profileLoads == 1 && g_profileListRefreshes == 1 && b.ProfileNamesOffset == 0 && g_prepareWorld == 0;
    ok = ok && g_KeyToNumber[2] == 1 && g_KeyToNumber[11] == 0 && g_KeyToNumber[10] == 9 && g_KeyToNumber[79] == 1
        && g_KeyToNumber[73] == 9 && g_KeyToNumber[30] == 11 && g_KeyToNumber[48] == 12 && g_KeyToNumber[100] == 0;
    if (!ok) { std::printf("00496070_TEST FAIL\n"); return 1; }
    std::printf("00496070_TEST PASS\n");
    return 0;
}