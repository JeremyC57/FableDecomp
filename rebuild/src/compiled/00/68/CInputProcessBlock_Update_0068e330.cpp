// CInputProcessBlock::Update(CProcessedInput& processed) -- every frame queue a block game event: "keep blocking" (type 0xd) while game action 7 still has a held-button event (kind 2) in the queue, else "stop blocking" (type 0x3d)
// Grade: DIFFER(111v103) -- /GS lane gap: the lane caches `processed` in esi at entry (`push esi ; mov esi,[esp+34h]`); without /GS the body is the retail one minus the cookie
// Leaf: Update
// Flags: /GS
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0068E330, length 103 bytes.
// Callees: CInputProcessGameBase::IsGameActionEventInQueue(long, long) @ 0x00687D50, CProcessedInput::AddGameEvent @ 0x00A0D340.
// Shape: one event per arm sharing the frame slot (VC hoists the common Player/Valid stores above the branch and
// tail-merges the AddGameEvent call); the ternary-argument form becomes `neg/sbb/and/add` arithmetic instead.
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
class CInputProcessBlock : public CInputProcessGameBase {
public:
    virtual void ProcessInput(const CInputEvent& event, CProcessedInput& processed);
    virtual void Update(CProcessedInput& processed);
};

void CInputProcessBlock::Update(CProcessedInput& processed)
{
    if (IsGameActionEventInQueue(7, 2)) {
        CGameEvent gameEvent(0xd);
        processed.AddGameEvent(gameEvent);
    } else {
        CGameEvent gameEvent(0x3d);
        processed.AddGameEvent(gameEvent);
    }
}