// CInputProcessSetRangedWeaponThirdPersonMode::ProcessInput(const CInputEvent& event, CProcessedInput& processed) -- unless the player's flag +0x218 is already set, a press of game action 9 or 10 queues game event 0x3a (enter ranged-weapon third-person mode)
// Leaf: ProcessInput
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0068EA60, length 123 bytes.
// Callees: CInputProcessGameBase::IsEventButtonPressed @ 0x00687A90, IsEventGameAction @ 0x00687CF0,
// CProcessedInput::AddGameEvent @ 0x00A0D340.
// Model: CGameEvent as the landed CTCInventoryBase::AddCloseInventoryEvent 0x005BC527 (Ego_r.pdb, 40 bytes). This TU has
// no /GS (no cookie although the event holds a char array). The player flag at +0x218 is modelled descriptively.
// Shape: the two action arms each queue their own event; VC cross-jumps them after the `push` of the event address.
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
    unsigned char _pad000[0x218];
    bool          m_inRangedWeaponMode;            // +0x218
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
class CInputProcessSetRangedWeaponThirdPersonMode : public CInputProcessGameBase {
public:
    virtual void ProcessInput(const CInputEvent& event, CProcessedInput& processed);
};

void CInputProcessSetRangedWeaponThirdPersonMode::ProcessInput(const CInputEvent& event, CProcessedInput& processed)
{
    if (MyPlayer->m_inRangedWeaponMode == false && IsEventButtonPressed(event)) {
        if (IsEventGameAction(9, event)) {
            CGameEvent gameEvent(0x3a);
            processed.AddGameEvent(gameEvent);
        } else if (IsEventGameAction(10, event)) {
            CGameEvent gameEvent(0x3a);
            processed.AddGameEvent(gameEvent);
        }
    }
}
