// CSubtitleRenderer::Pause(bool updateWhilePaused) -- mark the game paused by the subtitles, remember whether to keep updating, and queue a pause game event (type 9) carrying one byte 1
// Grade: DIFFER(100v98) -- retail homes AddUBYTE's by-value byte in the unused top byte of the bool parameter slot ([ebp+0Bh]) inside a 0x2c frame; the lane opens a 0x30 frame for it ([ebp-30h]) and copies `this` to eax (lane gap: short-lived local in a dead argument slot)
// Flags: /GS
#pragma optimize("y", off)
#pragma optimize("s", on)
// Retail: Fable.exe (TLC, VC7.1) @ 0x0048DD94, 98 bytes; `ret 4`. Callees: CMemoryBuffer::Copy(unsigned long, const void*)
//          @ 0x009F1760, CMainGameComponent::AddGameEvent(const CGameEvent&) @ 0x0041649C on the global @ 0x013B86A0,
//          __security_check_cookie @ 0x00BFE9F9.
// Layout: CSubtitleRenderer retail flags sit 8 bytes below engine/CSubtitleRenderer.h (PausedGame +0xdc, UpdateWhilePaused +0xe6);
//         CGameEvent as the landed AddCloseInventoryEvent 0x005BC527 (Type, Player = -1, Data[32], EndPos, Valid, Replacement).
// Shape: frame-pointer size TU with /GS (the event's data array); AddUBYTE(1) is inlined and its by-value byte is homed in the
//        dead parameter slot.
struct CMemoryBuffer {
    void Copy(unsigned long length, const void* data);   // 0x009F1760
};
class CGameEvent : public CMemoryBuffer { public:
    long          Type;                           // +0x00
    signed char   Player;                         // +0x04
    unsigned char Data[32];                       // +0x05
    unsigned char EndPos;                         // +0x25
    bool          Valid;                          // +0x26
    bool          Replacement;                    // +0x27
    CGameEvent(long type) : Type(type), Player(-1), EndPos(0), Valid(true), Replacement(false) { }
    void AddUBYTE(unsigned char value) { Copy(1, &value); }
};
class CMainGameComponent { public:
    void AddGameEvent(const CGameEvent& event);   // 0x0041649C
};
extern CMainGameComponent* g_pMainGameComponent;  // 0x013B86A0
enum { GAME_EVENT_PAUSE = 9 };
class CSubtitleRenderer { public:
    unsigned char _pad000[0xdc];
    bool          PausedGame;                     // +0xdc
    unsigned char _pad0dd[9];
    bool          UpdateWhilePaused;              // +0xe6
    void Pause(bool updateWhilePaused);
};

void CSubtitleRenderer::Pause(bool updateWhilePaused)
{
    UpdateWhilePaused = updateWhilePaused;
    PausedGame = true;
    CGameEvent event(GAME_EVENT_PAUSE);
    event.AddUBYTE(1);
    g_pMainGameComponent->AddGameEvent(event);
}