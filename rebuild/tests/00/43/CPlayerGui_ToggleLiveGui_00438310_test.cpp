// Behaviour test for CPlayerGui::ToggleLiveGui @ 0x00438310: refused (no change, no event) while live-GUI mode is off
// and the hero-information screens are open; otherwise the mode flips and the main game component receives show event
// 0x1a carrying the long 0x2b, or hide event 0x42 with no data, stamped with the main player number. The screens
// manager is only consulted when the mode is off.
#include <cstdio>
#include <cstring>
static int g_fail = 0;
#define CHECK(c) do { if (!(c)) { g_fail = 1; std::printf("check failed line %d\n", __LINE__); } } while (0)

struct CMemoryBuffer {
    void Copy(unsigned long length, const void* data);   // 0x009F1760
};
class CGameEvent : public CMemoryBuffer { public:
    long          Type;                                 // +0x00
    signed char   Player;                               // +0x04
    unsigned char Data[32];                             // +0x05
    unsigned char EndPos;                               // +0x25
    bool          Valid;                                // +0x26
    bool          Replacement;                          // +0x27
    CGameEvent(long type) : Type(type), Player(-1), EndPos(0), Valid(true), Replacement(false) { }
    void AddLong(long value) { Copy(sizeof(value), &value); }
};
void CMemoryBuffer::Copy(unsigned long length, const void* data)
{
    CGameEvent* e = static_cast<CGameEvent*>(this);
    std::memcpy(e->Data + e->EndPos, data, length); e->EndPos = (unsigned char)(e->EndPos + length);
}

static long g_mainPlayer = 3;
static int g_playerCalls = 0;
class CPlayerManager { public:
    long GetMainPlayerNumber() { ++g_playerCalls; return g_mainPlayer; }   // 0x00449700
};
static int g_events = 0;
static CGameEvent g_last(0);
class CMainGameComponent { public:
    unsigned char   _pad00[0x1c];
    CPlayerManager* PlayerManager;                      // +0x1c
    void AddGameEvent(const CGameEvent& event) { ++g_events; g_last = event; }   // 0x0041649C
};
CMainGameComponent* g_pMainGameComponent;               // 0x013B86A0

enum {
    GAME_EVENT_LIVE_GUI_ON  = 0x1a,
    GAME_EVENT_LIVE_GUI_OFF = 0x42
};

namespace NHeroInformationScreens {
class CManager { public:
    unsigned char _pad00[0x34];
    bool          Open;                                 // +0x34
};
}

class CPlayerGui { public:
    unsigned char _pad000[0x2be];
    bool          LiveGuiMode;                          // +0x2be
    unsigned char _pad2bf[0x32c - 0x2bf];
    NHeroInformationScreens::CManager* PHeroInfoScreensManager;   // +0x32c scoped_ptr<NHeroInformationScreens::CManager>
    void ToggleLiveGui();
};

void CPlayerGui::ToggleLiveGui()
{
    if (!LiveGuiMode && PHeroInfoScreensManager->Open)
        return;
    LiveGuiMode = !LiveGuiMode;
    if (LiveGuiMode) {
        CGameEvent event(GAME_EVENT_LIVE_GUI_ON);
        event.AddLong(0x2b);
        event.Player = (signed char)g_pMainGameComponent->PlayerManager->GetMainPlayerNumber();
        g_pMainGameComponent->AddGameEvent(event);
    } else {
        CGameEvent event(GAME_EVENT_LIVE_GUI_OFF);
        event.Player = (signed char)g_pMainGameComponent->PlayerManager->GetMainPlayerNumber();
        g_pMainGameComponent->AddGameEvent(event);
    }
}

static CPlayerGui g_gui;

int main()
{
    CPlayerManager pm; CMainGameComponent mgc; mgc.PlayerManager = &pm; g_pMainGameComponent = &mgc;
    NHeroInformationScreens::CManager screens; std::memset(&screens, 0, sizeof(screens));
    g_gui.PHeroInfoScreensManager = &screens;

    // Off + screens open: refused.
    g_gui.LiveGuiMode = false; screens.Open = true;
    g_gui.ToggleLiveGui();
    CHECK(!g_gui.LiveGuiMode && g_events == 0 && g_playerCalls == 0);

    // Off + screens closed: on, show event 0x1a with the long 0x2b.
    screens.Open = false; g_mainPlayer = 3;
    g_gui.ToggleLiveGui();
    CHECK(g_gui.LiveGuiMode && g_events == 1 && g_playerCalls == 1);
    long v = 0; std::memcpy(&v, g_last.Data, 4);
    CHECK(g_last.Type == 0x1a && g_last.Player == 3 && g_last.EndPos == 4 && v == 0x2b && g_last.Valid && !g_last.Replacement);

    // On: off regardless of the screens (manager not even dereferenced), hide event 0x42 with no data.
    g_gui.PHeroInfoScreensManager = 0; g_mainPlayer = 1;
    g_gui.ToggleLiveGui();
    CHECK(!g_gui.LiveGuiMode && g_events == 2 && g_playerCalls == 2);
    CHECK(g_last.Type == 0x42 && g_last.Player == 1 && g_last.EndPos == 0 && g_last.Valid && !g_last.Replacement);

    if (g_fail) { std::printf("00438310_TEST FAIL\n"); return 1; }
    std::printf("00438310_TEST PASS\n");
    return 0;
}