// CTCFireheartMinigame::Answer(bool) -- answer the pending quit prompt: on yes force-quit the game; either way queue game event 9 with a zero byte, clear the subtitle prompt and drop the pending flag
// Grade: DIFFER(143v143) -- only the inline AddUBYTE byte's home: retail stores it in the dead bool parameter slot (mov byte [esp+40h],bl ; lea eax,[esp+38h]) under /GS so its frame is 0x2c, ours opens a separate frame byte (sub esp,30h, every offset +4); tried by-value inline, __forceinline, const-reference AddUBYTE (lane gap: /GS one-byte local in a bool parameter slot)
// Leaf: Answer
// Flags: /GS
//
// Retail 0x007E7C00, 143 bytes (`ret 4`, /GS cookie 0x0139C8A8 / __security_check_cookie @ 0x00BFE9F9).
// Ego_r.pdb `?Answer@CTCFireheartMinigame@@QAEX_N@Z`. Callees: CTCFireheartMinigame::Quit() @ 0x007E7A40,
// CMemoryBuffer::Copy(unsigned long, const void*) @ 0x009F1760 (the inlined CGameEvent::AddUBYTE),
// CMainGameComponent::AddGameEvent(const CGameEvent&) @ 0x0041649C on g_pMainGameComponent @ 0x013B86A0,
// CSubtitleRenderer::Clear(bool) @ 0x00492BAB on g_pPlayerGui @ 0x013B8790 (+0x18).
// Layout = Ego_r.pdb CTCFireheartMinigame: ForceQuit +0x4d, QuitWithDelay +0x88; CGameEvent (40 bytes)
// is the landed CTCInventoryBase::AddCloseInventoryEvent model (0x005BC527).
// Shape: one zero register (ebx) for the event's EndPos/Replacement, the AddUBYTE byte, Clear's
// argument and the flag reset; the inline AddUBYTE's by-value byte is homed in the dead `yes` slot.
struct CMemoryBuffer {
    void Copy(unsigned long length, const void* data);   // 0x009F1760
};

struct CGameEvent : CMemoryBuffer {
    long          Type;                           // +0x00
    signed char   Player;                         // +0x04
    unsigned char Data[32];                       // +0x05
    unsigned char EndPos;                         // +0x25
    bool          Valid;                          // +0x26
    bool          Replacement;                    // +0x27
    CGameEvent(long type) : Type(type), Player(-1), EndPos(0), Valid(true), Replacement(false) { }
    void AddUBYTE(unsigned char value) { Copy(1, &value); }
};

struct CMainGameComponent {
    void AddGameEvent(const CGameEvent& event);   // 0x0041649C
};
extern CMainGameComponent* g_pMainGameComponent;  // 0x013B86A0

struct CSubtitleRenderer {
    void Clear(bool immediately);                 // 0x00492BAB
};
struct CPlayerGui {
    unsigned char     _pad00[0x18];
    CSubtitleRenderer SubtitleRenderer;           // +0x18
};
extern CPlayerGui* g_pPlayerGui;                  // 0x013B8790

enum { GAME_EVENT_FIREHEART_ANSWERED = 9 };

struct CThing;

struct CTCFireheartMinigame {
    virtual void v00();
    CThing*       ParentThing;                    // +0x04 (CTCBase)
    unsigned char _pad08[0x4c - 0x08];
    bool          InSession;                      // +0x4c
    bool          ForceQuit;                      // +0x4d
    unsigned char _pad4e[0x88 - 0x4e];
    bool          QuitWithDelay;                  // +0x88
    void Quit();                                  // 0x007E7A40
    void Answer(bool yes);
};

void CTCFireheartMinigame::Answer(bool yes)
{
    if (QuitWithDelay)
    {
        if (yes)
        {
            ForceQuit = true;
            Quit();
        }
        CGameEvent event(GAME_EVENT_FIREHEART_ANSWERED);
        event.AddUBYTE(0);
        g_pMainGameComponent->AddGameEvent(event);
        g_pPlayerGui->SubtitleRenderer.Clear(false);
        QuitWithDelay = false;
    }
}
