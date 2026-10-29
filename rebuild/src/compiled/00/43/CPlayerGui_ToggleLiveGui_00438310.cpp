// CPlayerGui::ToggleLiveGui() -- flip the live-GUI mode (refused while it is off and the hero-information screens are open) and report the change to the game as an event: show-GUI event 0x1A carrying 0x2B, or hide event 0x42, for the main player
// Grade: RELOCATION_MATCH with cl 13.10.3077 (RTM); the DDK 13.10.4035 build emits a different /GS prologue for this body.
// Flags: /GS
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00438310, length 192 bytes, plain `ret`; /GS frame (cookie 0x0139C8A8,
// __security_check_cookie 0x00BFE9F9).
// Callees: CMemoryBuffer::Copy(unsigned long, const void*) 0x009F1760 (the inline CGameEvent::AddLong),
// CPlayerManager::GetMainPlayerNumber 0x00449700 (g_pMainGameComponent 0x013B86A0 -> PlayerManager +0x1C),
// CMainGameComponent::AddGameEvent(const CGameEvent&) 0x0041649C.
// Layout: CPlayerGui retail offsets (as the landed ClearGui 0x00438230: engine/CPlayerGui.h is the PDB layout, 0x10
// high here): LiveGuiMode +0x2BE, PHeroInfoScreensManager (scoped_ptr<NHeroInformationScreens::CManager>) +0x32C;
// the manager's open flag at +0x34. CGameEvent (Ego_r.pdb, 40 bytes): Type +0, Player +4, Data[32] +5, EndPos +0x25,
// Valid +0x26, Replacement +0x27, deriving from CMemoryBuffer (landed 0x004159E0).
// Shape: one zero byte in dl serves the compare, the toggle and the event's zero stores; each arm builds its own
// event (sibling scopes, one frame slot) so VC hoists the shared constructor stores above the branch and tail-merges
// the player-number store and AddGameEvent call.
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

class CPlayerManager { public:
    long GetMainPlayerNumber() const;                         // 0x00449700
};
class CMainGameComponent { public:
    unsigned char   _pad00[0x1c];
    CPlayerManager* PlayerManager;                      // +0x1c
    void AddGameEvent(const CGameEvent& event);         // 0x0041649C
};
extern CMainGameComponent* g_pMainGameComponent;        // 0x013B86A0

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