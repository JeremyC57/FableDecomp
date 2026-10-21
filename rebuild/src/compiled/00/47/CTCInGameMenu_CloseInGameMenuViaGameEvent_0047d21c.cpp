// CTCInGameMenu::CloseInGameMenuViaGameEvent() -- queues a "close in-game menu" game event (type 0x1C) for the main player's hero while it is alive
// Leaf: CloseInGameMenuViaGameEvent
// Flags: /GS
// Retry 10b (B-12 sweep): the hero is a const REFERENCE to the call result (`CThingCreatureBase* const& hero = ...->GetCharacterThing();`): retail's dead `Player = -1` byte store survives before the player-number store (a value local lets VC7.1 drop it).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0047D21C, 109 bytes; frame-pointer size TU
// (`#pragma optimize("y", off)` + `optimize("s", on)`) with /GS (the event's 32-byte data array gets the cookie,
// 0x0139C8A8 stored at [ebp-4], __security_check_cookie @ 0x00BFE9F9).
// Callees: CTCBase::GetPlayerManager() @ 0x00686D40, CPlayerManager::GetMainPlayer() @ 0x00449970,
//          CPlayer::GetCharacterThing() @ 0x00487DC0, IS_THING_ALIVE<CThing*>(CThing* const&) @ 0x0043DBD0 (__fastcall),
//          CMainGameComponent::AddGameEvent(const CGameEvent&) @ 0x0041649C on the global component @ 0x013B86A0.
// Shape: the hero is a creature pointer kept in esi and IS_THING_ALIVE<CThing*> binds to a converted CThing*
// temporary ([ebp-30h]); the inline CGameEvent ctor (Type, Player = -1, EndPos 0, Valid 1, Replacement 0, as in the
// landed 0x005BC527) is followed by the player number byte from the hero (+0x90).
// Layout: CGameEvent (PDB, 40 bytes) Type +0, Player +4, Data[32] +5, EndPos +0x25, Valid +0x26, Replacement +0x27.
#pragma optimize("y", off)
#pragma optimize("s", on)

struct CMemoryBuffer {
};

struct CGameEvent : CMemoryBuffer {
    long          Type;                              // +0x00
    signed char   Player;                            // +0x04
    unsigned char Data[32];                          // +0x05
    unsigned char EndPos;                            // +0x25
    bool          Valid;                             // +0x26
    bool          Replacement;                       // +0x27
    CGameEvent(long type) : Type(type), Player(-1), EndPos(0), Valid(true), Replacement(false) { }
};

struct CMainGameComponent {
    void AddGameEvent(const CGameEvent& event);      // 0x0041649C
};
extern CMainGameComponent* g_pMainGameComponent;     // 0x013B86A0

struct CThing {
};

template <class T> bool __fastcall IS_THING_ALIVE(const T& thing);   // 0x0043DBD0 (CThing*)

struct CThingCreatureBase : CThing {
    unsigned char _pad_0x00[0x90];
    signed char   PlayerNumber;                      // +0x90
};

struct CPlayer {
    CThingCreatureBase* GetCharacterThing();         // 0x00487DC0
};

struct CPlayerManager {
    CPlayer* GetMainPlayer();                        // 0x00449970
};

enum { GAME_EVENT_CLOSE_IN_GAME_MENU = 0x1C };

struct CTCInGameMenu {
    void* __vftable;                                 // +0x00
    void* ParentThing;                               // +0x04 CTCBase

    CPlayerManager* GetPlayerManager() const;        // 0x00686D40 (CTCBase)
    void CloseInGameMenuViaGameEvent();
};

void CTCInGameMenu::CloseInGameMenuViaGameEvent()
{
    CThingCreatureBase* const& hero = GetPlayerManager()->GetMainPlayer()->GetCharacterThing();
    if (IS_THING_ALIVE<CThing*>(hero)) {
        CGameEvent event(GAME_EVENT_CLOSE_IN_GAME_MENU);
        event.Player = hero->PlayerNumber;
        g_pMainGameComponent->AddGameEvent(event);
    }
}