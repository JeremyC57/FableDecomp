// CInputProcessClickPastText::ProcessInput(const CInputEvent& event, CProcessedInput& processed) -- on a button press that is game action 0x14 or 6, mark the input handled and queue a "click past text" game event (type 0x19)
// Retry 10: matches under the RTM 13.10.3077 compiler with /GS (the QFE shadowing of `processed` into ebx was the DDK compiler).
// Leaf: ProcessInput
// Flags: /GS
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0068C6E0, length 126 bytes.
// Callees: CInputProcessGameBase::IsEventButtonPressed @ 0x00687A90, IsEventGameAction @ 0x00687CF0 (twice),
// CProcessedInput::SetHandled @ 0x00A0D390 (manifest "CWomanToAttract::Init" propagated), AddGameEvent @ 0x00A0D340.
// Shape: the event is built before SetHandled (`CGameEvent e(0x19); processed.SetHandled(); processed.AddGameEvent(e);`).
// Model: CGameEvent as the landed CTCInventoryBase::AddCloseInventoryEvent 0x005BC527 (Ego_r.pdb, 40 bytes:
// Type +0, Player +4, Data[32] +5, EndPos +0x25, Valid +0x26, Replacement +0x27, over an empty CMemoryBuffer
// whose Copy(length, data) @ 0x009F1760 appends to Data); the input-process base as engine/CInputProcessGameBase.h
// (MyPlayer +0x18) with the helpers of the landed CInputProcessFirstPersonTargeting 0x00690390.
// /GS: the event's char array gets the cookie frame (cookie @ 0x0139C8A8, __security_check_cookie @ 0x00BFE9F9).
class CInputEvent;
class CProcessedInput;
struct CMemoryBuffer {
    void Copy(unsigned long length, const void* data);   // 0x009F1760
};
struct CGameEvent : CMemoryBuffer {
    long          Type;                            // +0x00
    signed char   Player;                          // +0x04
    unsigned char Data[32];                        // +0x05
    unsigned char EndPos;                          // +0x25
    bool          Valid;                           // +0x26
    bool          Replacement;                     // +0x27
    CGameEvent(long type) : Type(type), Player(-1), EndPos(0), Valid(true), Replacement(false) { }
    void AddLong(long value) { Copy(sizeof(value), &value); }
};
class CProcessedInput {
public:
    void AddGameEvent(const CGameEvent& event);    // 0x00A0D340
    void SetHandled();                             // 0x00A0D390
};
class CPlayer {
public:
    long GetCurrentMode() const;                   // 0x00633BE0
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
    bool IsEventButtonPressed(const CInputEvent& event);              // 0x00687A90 (kind 0x13; manifest "IsEventButtonHeld")
    bool IsEventButtonHeld(const CInputEvent& event);                 // 0x00687BD0 (kind 0x15)
    bool IsEventGameAction(long action, const CInputEvent& event);    // 0x00687CF0
    bool IsGameActionEventInQueue(long action, long buttonEventType); // 0x00687D50
};
class CInputProcessClickPastText : public CInputProcessGameBase {
public:
    virtual void ProcessInput(const CInputEvent& event, CProcessedInput& processed);
};

void CInputProcessClickPastText::ProcessInput(const CInputEvent& event, CProcessedInput& processed)
{
    if (IsEventButtonPressed(event)) {
        if (IsEventGameAction(0x14, event) || IsEventGameAction(6, event)) {
            CGameEvent gameEvent(0x19);
            processed.SetHandled();
            processed.AddGameEvent(gameEvent);
        }
    }
}