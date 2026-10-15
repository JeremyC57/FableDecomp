// CInputProcessToggleViewHeroMode::ProcessInput(const CInputEvent& event, CProcessedInput& processed) -- on a held button that is game action 0x70, or (while the player is in mode 0x2f) any held button from controller 1, queue game event 0x55 (toggle view-hero mode)
// Grade: DIFFER(133v129) -- /GS lane gap: `processed` shadowed into ebx (and the event into esi) at entry; retail keeps the event in edi and re-reads `processed` from its slot before AddGameEvent
// Leaf: ProcessInput
// Flags: /GS
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0068C7F0, length 129 bytes.
// Callees: CInputProcessGameBase::IsEventButtonHeld @ 0x00687BD0, IsEventGameAction @ 0x00687CF0, CPlayer::HasMode
// @ 0x00634160 (MyPlayer +0x18), the event's controller-index getter (+0) @ 0x00A03B70, CProcessedInput::AddGameEvent @ 0x00A0D340,
// __security_check_cookie @ 0x00BFE9F9.
// Model: CGameEvent as the landed CTCInventoryBase::AddCloseInventoryEvent 0x005BC527 (Ego_r.pdb, 40 bytes); the
// `__forceinline AddGameEvent(long type)` helper reproduces retail's shared `xor al,al` zero stores.
class CInputEvent {
public:
    long GetControllerIndex() const;               // 0x00A03B70 (`mov eax,[ecx]`)
};
struct CMemoryBuffer {
};
struct CGameEvent : CMemoryBuffer {
    long          Type;                            // +0x00
    signed char   Player;                          // +0x04
    unsigned char Data[32];                        // +0x05
    unsigned char EndPos;                          // +0x25
    bool          Valid;                           // +0x26
    bool          Replacement;                     // +0x27
    CGameEvent(long type) : Type(type), Player(-1), EndPos(0), Valid(true), Replacement(false) { }
};
class CProcessedInput {
public:
    void AddGameEvent(const CGameEvent& event);    // 0x00A0D340
    __forceinline void AddGameEvent(long type) { CGameEvent gameEvent(type); AddGameEvent(gameEvent); }
};
class CPlayer {
public:
    bool HasMode(long mode);                       // 0x00634160
};
struct CWorld; struct CGameDefinitionManager; struct CMainGameComponent; struct CGamePlayerInterface;
class CInputProcessGameBase {                      // engine/CInputProcessGameBase.h layout
public:
    virtual void v00();
    unsigned char           _pad04[0x10];          // +0x04 CAInputProcess
    CWorld*                 World;                 // +0x14
    CPlayer*                MyPlayer;              // +0x18
    CGameDefinitionManager* DefinitionManager;     // +0x1c
    CMainGameComponent*     Component;             // +0x20
    CGamePlayerInterface*   PGamePlayerInterface;  // +0x24
    bool IsEventButtonPressed(const CInputEvent& event);              // 0x00687A90 (kind 0x13)
    bool IsEventButtonHeld(const CInputEvent& event);                 // 0x00687BD0 (kind 0x15)
    bool IsEventGameAction(long action, const CInputEvent& event);    // 0x00687CF0
};
class CInputProcessToggleViewHeroMode : public CInputProcessGameBase {
public:
    virtual void ProcessInput(const CInputEvent& event, CProcessedInput& processed);
};

void CInputProcessToggleViewHeroMode::ProcessInput(const CInputEvent& event, CProcessedInput& processed)
{
    if (IsEventButtonHeld(event)
        && (IsEventGameAction(0x70, event) || (MyPlayer->HasMode(0x2f) && event.GetControllerIndex() == 1)))
        processed.AddGameEvent(0x55);
}
