// CInputProcessJumpingAndRolling::ProcessJumping(const CInputEvent& event, CProcessedInput& processed) -- while jumping is enabled (global flag), a press of game action 0x1d queues game event 0x32 (jump)
// Grade: DIFFER(116v112) -- /GS lane gap: both reference parameters shadowed into ebp/edi at entry; retail loads the event into edi after the flag test and re-reads `processed` late
// Leaf: ProcessJumping
// Flags: /GS
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006A4040, length 112 bytes.
// Callees: CInputProcessGameBase::IsEventButtonPressed @ 0x00687A90, IsEventGameAction @ 0x00687CF0,
// CProcessedInput::AddGameEvent @ 0x00A0D340, __security_check_cookie @ 0x00BFE9F9; global byte 0x013B86E5 (jumping enabled).
// Model: CGameEvent as the landed CTCInventoryBase::AddCloseInventoryEvent 0x005BC527 (Ego_r.pdb, 40 bytes); its char
// array gives the /GS cookie frame. The input-process base as engine/CInputProcessGameBase.h (the
// engine/CInputProcessJumpingAndRolling.h header flattens it).
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
class CPlayer;
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
extern bool g_JumpingEnabled;                      // 0x013B86E5
class CInputProcessJumpingAndRolling : public CInputProcessGameBase {
public:
    void ProcessJumping(const CInputEvent& event, CProcessedInput& processed);
};

void CInputProcessJumpingAndRolling::ProcessJumping(const CInputEvent& event, CProcessedInput& processed)
{
    if (g_JumpingEnabled && IsEventButtonPressed(event) && IsEventGameAction(0x1d, event)) {
        CGameEvent gameEvent(0x32);
        processed.AddGameEvent(gameEvent);
    }
}