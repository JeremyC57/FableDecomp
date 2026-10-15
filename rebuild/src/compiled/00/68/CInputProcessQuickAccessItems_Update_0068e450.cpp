// CInputProcessQuickAccessItems::Update(CProcessedInput& processed) -- cancel a quick-access charge once its button is no longer held: the special-move button (slot -2, game action 0x4e), the slot's assignable button (slot >= 0), or with no charge pending while the player is still in charge mode 0x1c
// Leaf: Update
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0068E450, length 115 bytes.
// Callees: CInputProcessGameBase::IsGameActionEventInQueue(long, long, float*) @ 0x00687D80 (landed),
// IsAssignableSpecialMoveButtonInQueue(long, long) @ 0x00687CB0 (landed), CPlayer::HasMode @ 0x00634160 (MyPlayer +0x18),
// CInputProcessQuickAccessItems::CancelQuickAccessCharge @ 0x0068E3C0. Globals 0x0137EE78 (charging slot: -2 the special
// move, -1 none) and 0x0137EE7C (the slot's button).
// Model: as the landed CInputProcessQuickAccessItems::Cleanup 0x0068E4D0. Shape: the slot is read once into eax; the
// queue time is a zeroed float local; each Cancel call is tail-duplicated with its own return.
class CProcessedInput;
extern long g_QuickAccessChargeSlot;               // 0x0137EE78
extern long g_QuickAccessChargeButton;             // 0x0137EE7C
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
    bool IsAssignableSpecialMoveButtonInQueue(long button, long buttonEventType);    // 0x00687CB0
    bool IsGameActionEventInQueue(long action, long buttonEventType, float* pTime);  // 0x00687D80
};
class CInputProcessQuickAccessItems : public CInputProcessGameBase {
public:
    void CancelQuickAccessCharge();                // 0x0068E3C0
    virtual void Update(CProcessedInput& processed);
};

void CInputProcessQuickAccessItems::Update(CProcessedInput& processed)
{
    long slot = g_QuickAccessChargeSlot;
    if (slot == -2) {
        float time = 0.0f;
        if (!IsGameActionEventInQueue(0x4e, 2, &time))
            CancelQuickAccessCharge();
    } else if (slot != -1) {
        if (!IsAssignableSpecialMoveButtonInQueue(g_QuickAccessChargeButton, 2))
            CancelQuickAccessCharge();
    } else if (MyPlayer->HasMode(0x1c)) {
        CancelQuickAccessCharge();
    }
}