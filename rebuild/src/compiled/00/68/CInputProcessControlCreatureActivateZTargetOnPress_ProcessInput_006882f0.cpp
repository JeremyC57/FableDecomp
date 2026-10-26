// CInputProcessControlCreatureActivateZTargetOnPress::ProcessInput(const CInputEvent& event, CProcessedInput& processed) -- a press of game action 1 queues game event 0x15 (activate Z-target)
// Retry 10: matches under the RTM 13.10.3077 compiler with /GS (the QFE shadowing of `processed` into ebx was the DDK compiler).
// Leaf: ProcessInput
// Flags: /GS
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006882F0, length 103 bytes.
// Callees: CInputProcessGameBase::IsEventGameAction @ 0x00687CF0, IsEventButtonPressed @ 0x00687A90 (event kind 0x13),
// CProcessedInput::AddGameEvent @ 0x00A0D340, __security_check_cookie @ 0x00BFE9F9.
// Model: CGameEvent as the landed CTCInventoryBase::AddCloseInventoryEvent 0x005BC527 (Ego_r.pdb, 40 bytes); its char
// array gives the /GS cookie frame. The input-process base as engine/CInputProcessGameBase.h.
class CInputEvent;
struct CMemoryBuffer {
};
class CGameEvent : public CMemoryBuffer { public:
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
    long GetCurrentMode() const;                   // 0x00633BE0
};
class CGamePlayerInterface {
public:
    bool IsHoldToActivate();                       // 0x00445B80
};
class CWorld; class CGameDefinitionManager; class CMainGameComponent;
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
class CInputProcessControlCreatureActivateZTargetOnPress : public CInputProcessGameBase {
public:
    virtual void ProcessInput(const CInputEvent& event, CProcessedInput& processed);
};

void CInputProcessControlCreatureActivateZTargetOnPress::ProcessInput(const CInputEvent& event, CProcessedInput& processed)
{
    if (IsEventGameAction(1, event) && IsEventButtonPressed(event))
        processed.AddGameEvent(0x15);
}