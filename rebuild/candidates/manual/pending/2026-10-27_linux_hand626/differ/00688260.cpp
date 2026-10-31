// CInputProcessControlCreatureActivateZTarget::ProcessInput(const CInputEvent& event, CProcessedInput& processed) -- unless the player is in mode 2, a held (toggle Z-target option) or pressed event of game action 1 queues game event 0x15 (activate Z-target)
// Grade: DIFFER(137v137) -- the inlined CGameEvent(long) zero bytes: retail stores EndPos/Replacement from a shared "xor al,al" before the push; ours stores two immediates after it
// Leaf: ?ProcessInput@CInputProcessControlCreatureActivateZTarget@@UAEXABVCInputEvent@@AAVCProcessedInput@@@Z
// Flags: /GS
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00688260, 137 bytes, ret 8, /GS cookie (CGameEvent's char array).
// Callees: CPlayer::GetCurrentMode() const 0x00633BE0, CGamePlayerInterface::IsToggleZTarget() const 0x00445B80 (Ego_r: the five
//   adjacent byte getters +0x48/+0x49/+0x4A/+0x4C/+0x4D at 0x00445B80.. follow the donor run IsToggleZTarget, IsToggleSpells,
//   IsToggleSneak, IsToggleExpressionShift, IsRequireAttackButtonHeldForFlourish; reloc_bindings has IsInputEnabled),
//   CInputProcessGameBase::IsEventButtonHeld 0x00687BD0 / IsEventButtonPressed 0x00687A90 (Ego_r IBE), IsEventGameAction
//   0x00687CF0 (Ego_r QBE, EGameAction), CProcessedInput::AddGameEvent(const CGameEvent&) 0x00A0D340, __security_check_cookie.
// Research: the retail out-of-line CGameEvent(long) 0x004159B0 shares "xor cl,cl" too, and the plain init-list ctor used here
//   MATCHES it (also the all-body form); only the inlined copy here keeps immediates.
// Tried (this session): the landed 0x006882F0 __forceinline AddGameEvent(long) helper, early return + PDB names (this file).
//   Earlier: if/else ok, ctor-body assignment order.
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
    __forceinline void AddGameEvent(long type) { CGameEvent gameEvent(type); AddGameEvent(gameEvent); }
};
enum EPlayerMode { PLAYER_MODE_2 = 2 };
enum EGameAction { GAME_ACTION_1 = 1 };
class CPlayer {
public:
    EPlayerMode GetCurrentMode() const;            // 0x00633BE0
};
class CGamePlayerInterface {
public:
    bool IsToggleZTarget() const;                  // 0x00445B80
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
    bool IsEventGameAction(EGameAction action, const CInputEvent& event) const;   // 0x00687CF0
protected:
    bool IsEventButtonPressed(const CInputEvent& event) const;        // 0x00687A90
    bool IsEventButtonHeld(const CInputEvent& event) const;           // 0x00687BD0
};
class CInputProcessControlCreatureActivateZTarget : public CInputProcessGameBase {
public:
    virtual void ProcessInput(const CInputEvent& event, CProcessedInput& processed);
};

void CInputProcessControlCreatureActivateZTarget::ProcessInput(const CInputEvent& event, CProcessedInput& processed)
{
    if (MyPlayer->GetCurrentMode() == PLAYER_MODE_2)
        return;
    if (PGamePlayerInterface->IsToggleZTarget() ? IsEventButtonHeld(event) : IsEventButtonPressed(event)) {
        if (IsEventGameAction(GAME_ACTION_1, event))
            processed.AddGameEvent(0x15);
    }
}
