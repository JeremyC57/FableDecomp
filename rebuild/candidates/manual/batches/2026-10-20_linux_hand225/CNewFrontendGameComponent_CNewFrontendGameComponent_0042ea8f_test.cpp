// Behaviour test for 0x0042EA8F CNewFrontendGameComponent::CNewFrontendGameComponent: base bound to the game, pointers and
// vectors empty, both clock reads taken, inputs allowed, repeat delay 1/3 s, media player assumed installed, flags clear.
#include <cstdio>

static double g_clock; static int g_clockReads;
static double GFGetTime() { ++g_clockReads; return g_clock += 1.0; }
struct CGame { int id; };
struct CGameComponent { bool m_bShutDown; CGame* m_pGame; CGameComponent(CGame* g) : m_bShutDown(false), m_pGame(g) {} };
struct CCP { void* Data; void* Info; CCP() : Data(0), Info(0) {} };
struct CVec { void* s; void* f; void* e; CVec() : s(0), f(0), e(0) {} };
struct CWide { const char* t; CWide(const char* x) : t(x) {} CWide& operator=(const char* x) { t = x; return *this; } };
struct CInit { CWide StartLevel, StartPosition; CInit() : StartLevel("?"), StartPosition("?") {} };
struct CNewFrontendGameComponent : CGameComponent {
    CCP m_pMenuFont; long CurrentMovement; bool StartedRepeatingMovement; double TimeLastMovementHappened;
    bool m_bAllowInputs, m_bCreateGame, CreateDevFrontEnd; CInit m_bInitMainGame; void* m_pFrontEndDef;
    CCP m_pFrontEndGraphicBank, m_pMeshBank, PSampleBank, m_pEngine, m_pTextBank; CVec FileNames, FileTimes; CWide SaveGame;
    void* PAVITextureShow; void* PAVITextureDecode; bool AVIPlaying, XMVCodeLoaded; void* m_pFrontEndManager; double m_fLastRealUpdateTime;
    bool CreatedMarkerFile; float m_LastInterp[3]; float m_fRepeatDelay; unsigned long m_dwDPadTriggerState; long LastActiveJoystickDeviceNumber; bool WindowsMediaPlayerInstalled;
    CNewFrontendGameComponent(CGame* game, void*)
        : CGameComponent(game), CurrentMovement(0), StartedRepeatingMovement(false), TimeLastMovementHappened(GFGetTime()),
          m_bAllowInputs(true), m_bCreateGame(false), m_pFrontEndDef(0), SaveGame(""), PAVITextureShow(0), PAVITextureDecode(0),
          AVIPlaying(false), XMVCodeLoaded(false), m_pFrontEndManager(0), m_fLastRealUpdateTime(GFGetTime()),
          m_fRepeatDelay(0.333f), m_dwDPadTriggerState(0), LastActiveJoystickDeviceNumber(0), WindowsMediaPlayerInstalled(true)
    {
        m_LastInterp[0] = m_LastInterp[1] = m_LastInterp[2] = 0.0f;
        m_bInitMainGame.StartLevel = ""; m_bInitMainGame.StartPosition = "";
        CreatedMarkerFile = false; CreateDevFrontEnd = false;
    }
};
int main()
{
    CGame game = { 1 };
    CNewFrontendGameComponent c(&game, 0);
    bool ok = c.m_pGame == &game && !c.m_bShutDown && c.m_pMenuFont.Data == 0 && c.CurrentMovement == 0 && !c.StartedRepeatingMovement
        && c.TimeLastMovementHappened == 1.0 && c.m_fLastRealUpdateTime == 2.0 && g_clockReads == 2
        && c.m_bAllowInputs && !c.m_bCreateGame && !c.CreateDevFrontEnd && c.m_bInitMainGame.StartLevel.t[0] == 0 && c.m_bInitMainGame.StartPosition.t[0] == 0
        && c.m_pFrontEndDef == 0 && c.m_pTextBank.Info == 0 && c.FileNames.s == 0 && c.FileTimes.e == 0 && c.SaveGame.t[0] == 0
        && !c.AVIPlaying && !c.XMVCodeLoaded && c.m_pFrontEndManager == 0 && !c.CreatedMarkerFile && c.m_LastInterp[2] == 0.0f
        && c.m_fRepeatDelay == 0.333f && c.m_dwDPadTriggerState == 0 && c.LastActiveJoystickDeviceNumber == 0 && c.WindowsMediaPlayerInstalled;
    if (!ok) { std::printf("0042ea8f_TEST FAIL\n"); return 1; }
    std::printf("0042ea8f_TEST PASS\n");
    return 0;
}
