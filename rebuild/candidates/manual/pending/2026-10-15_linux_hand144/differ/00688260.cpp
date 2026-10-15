// CInputProcessControlCreatureActivateZTarget::ProcessInput(const CInputEvent& event, CProcessedInput& processed) -- unless the player is in mode 2, a press (or a held event when the game player interface's controller flag is set) of game action 1 queues game event 0x15 (activate Z-target)
// Grade: DIFFER(141v137) -- /GS lane gap: the DDK 13.10.4035 compiler shadows the pointer parameters into callee-saved registers at entry (`push ebp ; mov ebp,[esp+38h]` for `processed`, edi for `event`); retail (RTM) re-reads the argument slots late; the event zero bytes are immediates where retail shares `xor al,al` (the __forceinline AddGameEvent(long) helper of 006882f0 fixes that part)
// Leaf: ProcessInput
// Flags: /GS
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00688260, length 137 bytes.
// Callees: CPlayer::GetCurrentMode @ 0x00633BE0 (MyPlayer +0x18), the game player interface flag getter @ 0x00445B80
// (`mov eax,[ecx+7A0h] ; mov al,[eax+48h]`; manifest label CTCShotDefinitionBase::CanBeHitByMeleeStrike is propagated),
// CInputProcessGameBase::IsEventButtonHeld @ 0x00687BD0 (event kind 0x15) / IsEventButtonPressed @ 0x00687A90 (kind 0x13),
// IsEventGameAction @ 0x00687CF0, CProcessedInput::AddGameEvent @ 0x00A0D340, __security_check_cookie @ 0x00BFE9F9.
// Model: CGameEvent as the landed CTCInventoryBase::AddCloseInventoryEvent 0x005BC527 (Ego_r.pdb, 40 bytes); its char
// array gives the /GS cookie frame. The input-process base as engine/CInputProcessGameBase.h (MyPlayer +0x18,
// PGamePlayerInterface +0x24).
// Shape: the two button predicates are one conditional call (the shared `push edi` hoisted above the branch).
class CInputEvent;
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
};
class CPlayer {
public:
    long GetCurrentMode() const;                   // 0x00633BE0
};
class CGamePlayerInterface {
public:
    bool IsHoldToActivate();                       // 0x00445B80
};
struct CWorld; struct CGameDefinitionManager; struct CMainGameComponent;
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
class CInputProcessControlCreatureActivateZTarget : public CInputProcessGameBase {
public:
    virtual void ProcessInput(const CInputEvent& event, CProcessedInput& processed);
};

void CInputProcessControlCreatureActivateZTarget::ProcessInput(const CInputEvent& event, CProcessedInput& processed)
{
    if (MyPlayer->GetCurrentMode() != 2) {
        bool ok = PGamePlayerInterface->IsHoldToActivate() ? IsEventButtonHeld(event) : IsEventButtonPressed(event);
        if (ok && IsEventGameAction(1, event)) {
            CGameEvent gameEvent(0x15);
            processed.AddGameEvent(gameEvent);
        }
    }
}
