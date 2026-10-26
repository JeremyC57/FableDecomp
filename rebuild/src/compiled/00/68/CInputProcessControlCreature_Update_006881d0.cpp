// CInputProcessControlCreature::Update(CProcessedInput& processed) -- drop the player out of spell mode / expression-shift mode once the holding button (game actions 0x56 / 0x57, held kind 2) is no longer in the event queue, unless the game player interface forces that mode on
// Leaf: Update
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006881D0, length 132 bytes.
// Callees: the game player interface's mode-forced getters @ 0x00445B90 / 0x00445BB0 (`[this+7A0h]` +0x49 / +0x4a; manifest
// "CTCShotDefinitionBase::CanBeHitByMeleeStrike" propagated), CPlayer::GetPlayerNumber @ 0x00487510,
// CGamePlayerInterface::IsGameActionEventInQueue(EGameAction, EInputButtonEventType, long) @ 0x00446B10.
// Layout: Ego_r.pdb CPlayer with the retail 4-byte PlayerModes list: SpellMode +0x218, ExpressionShiftMode +0x219
// (PDB +0x220/+0x221); the input-process base as engine/CInputProcessGameBase.h (MyPlayer +0x18, PGamePlayerInterface +0x24).
// Shape: the interface is read into a local before the player-number call (edi); the first clear stores the call's zero `al` (the result kept in a local and assigned), the second an immediate.
// Layout (supersedes any local-model note above): engine/CPlayer.h now has the retail STLport 4.6 offsets, so the data
//   members come from the shared header; the class below keeps only the declarations this body needs.
#define CPlayer CPlayerDonor
#include "engine/CPlayer.h"
#undef CPlayer
class CProcessedInput;
class CPlayer : public CPlayerDonor {
public:
    long GetPlayerNumber() const;                  // 0x00487510
};
class CGamePlayerInterface {
public:
    bool IsSpellModeForced() const;                // 0x00445B90
    bool IsExpressionShiftModeForced() const;      // 0x00445BB0
    bool IsGameActionEventInQueue(long action, long buttonEventType, long player);   // 0x00446B10
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
};
class CInputProcessControlCreature : public CInputProcessGameBase {
public:
    virtual void Update(CProcessedInput& processed);
};

void CInputProcessControlCreature::Update(CProcessedInput& processed)
{
    if (MyPlayer->SpellMode && !PGamePlayerInterface->IsSpellModeForced()) {
        CGamePlayerInterface* gpi = PGamePlayerInterface;
        bool held = gpi->IsGameActionEventInQueue(0x56, 2, MyPlayer->GetPlayerNumber());
        if (!held)
            MyPlayer->SpellMode = held;
    }
    if (MyPlayer->ExpressionShiftMode && !PGamePlayerInterface->IsExpressionShiftModeForced()) {
        CGamePlayerInterface* gpi = PGamePlayerInterface;
        if (!gpi->IsGameActionEventInQueue(0x57, 2, MyPlayer->GetPlayerNumber()))
            MyPlayer->ExpressionShiftMode = false;
    }
}