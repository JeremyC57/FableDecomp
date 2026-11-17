// CInputProcessQuickAccessItems::Update(CProcessedInput& processed) -- cancel a quick-access charge once its button is no longer held: the special-move button (slot -2, game action 0x4e), the slot's assignable button (slot >= 0), or with no charge pending while the player is still in charge mode 0x1c
// Leaf: Update
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0068E450, length 115 bytes.
// Callees: CInputProcessGameBase::IsGameActionEventInQueue(EGameAction, EInputButtonEventType, float&) const @ 0x00687D80 (Ego_r.pdb +
// FableWin.pdb ?IsGameActionEventInQueue@CInputProcessGameBase@@IBE_NW4EGameAction@@W4EInputButtonEventType@@AAM@Z, protected),
// IsQuickAccessEventInQueue(long, EInputButtonEventType) const @ 0x00687CB0 (FAM13: Ego_r.pdb / FableWin.pdb
// ?IsQuickAccessEventInQueue@CInputProcessGameBase@@IBE_NJW4EInputButtonEventType@@@Z; the old spelling is in neither PDB), CPlayer::HasMode @ 0x00634160 (MyPlayer +0x18),
// CInputProcessQuickAccessItems::CancelQuickAccessCharge @ 0x0068E3C0. Globals 0x0137EE78 (charging slot: -2 the special
// move, -1 none) and 0x0137EE7C (the slot's button).
// Model (NF55, 2026-11-17): engine/CInputProcessGameBase.h retail branch (EVENTINQUEUE + NF55 RETAIL_QUICKACCESSQUEUE),
// engine/CInputProcessQuickAccessItems.h (NF55 UPDATE: the private CancelQuickAccessCharge, AAE) and engine/CPlayer.h; was a local
// CInputProcessGameBase model with the made-up IsGameActionEventInQueue(long, long, float*). Shape: the slot is read once into eax; the
// queue time is a zeroed float local; each Cancel call is tail-duplicated with its own return.
class CProcessedInput;
extern long g_QuickAccessChargeSlot;               // 0x0137EE78
extern long g_QuickAccessChargeButton;             // 0x0137EE7C
#define FABLE_RETAIL_PLAYER
#define FABLE_RETAIL_INPUTPROCESS
#define FABLE_RETAIL_INPUTPROCESS_EVENTINQUEUE
#define FABLE_RETAIL_CINPUTPROCESSGAMEBASE_ISQUICKACCESSEVENTINQUEUE
#define FABLE_RETAIL_CINPUTPROCESSQUICKACCESSITEMS_UPDATE
#include "fable_compiler.h"
#include "engine/EGameAction.h"
#include "engine/EPlayerMode.h"
#include "engine/CPlayer.h"
#include "engine/CInputProcessGameBase.h"
#include "engine/CInputProcessQuickAccessItems.h"

void CInputProcessQuickAccessItems::Update(CProcessedInput& processed)
{
    long slot = g_QuickAccessChargeSlot;
    if (slot == -2) {
        float time = 0.0f;
        if (!IsGameActionEventInQueue((EGameAction)0x4e, (EInputButtonEventType)2, time))
            CancelQuickAccessCharge();
    } else if (slot != -1) {
        if (!IsQuickAccessEventInQueue(g_QuickAccessChargeButton, (EInputButtonEventType)2))
            CancelQuickAccessCharge();
    } else if (MyPlayer->HasMode(PLAYER_MODE_CHARGE_QUICK_ACCESS)) {
        CancelQuickAccessCharge();
    }
}