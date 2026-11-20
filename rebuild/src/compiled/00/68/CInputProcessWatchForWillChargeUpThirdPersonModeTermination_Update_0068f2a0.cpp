// CInputProcessWatchForWillChargeUpThirdPersonModeTermination::Update(CProcessedInput& processed) -- while the live hero's will-charge component (kind 0xa7) still runs, hold the quit timer at 5; once it has ended, count the timer down and leave player mode 0x23 when it runs out
// Leaf: Update
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0068F2A0, length 120 bytes.
// Callees: CPlayer::GetPControlledCreature @ 0x00487DC0 (MyPlayer +0x18), CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound (const)
// @ 0x004365B0, CTCSpecialAbilities::IsChargingUpAbility @ 0x0072A860 (AbilityMutexLock +0x0C), CPlayer::RemoveMode @ 0x00634190.
// Model: engine/CInputProcessWatchForWillChargeUpThirdPersonModeTermination.h (QuitModeTimer +0x28) over the
// CInputProcessGameBase members; the thing's presence bitset (+0x20: kind 0xa7 is bit 7 of the byte at +0x34, tested as
// `test dl,dl ; jns`), component map +0x44 and dead bit +0x91 as the landed CGameScriptInterface rows.
// Shape: the component is a null-initialised local assigned only when present (the zero preloaded in ecx; a ternary
// materialises it in eax) and handed to the test unchecked; the
// timer decrement is `--QuitModeTimer < 0` (`dec [edi+28h] ; jns`).
// Name fix (agentNF5 2026-11-10): CPlayer::RemoveMode 0x00634190 takes EPlayerMode (?RemoveMode@CPlayer@@QAEXW4EPlayerMode@@@Z, Ego_r.pdb / FableWin.pdb), was long.
// NF118 (PRIORITY q6369 / q6388): kind 0xa7 is TCI_SPECIAL_ABILITIES (167): the component is the retail engine/CTCSpecialAbilities.h
//   and its test is Ego_r.pdb / FableWin.pdb ?IsChargingUpAbility@CTCSpecialAbilities@@QBE_NXZ 0x0072A860 (the will-charge-up watcher
//   holds the mode while the hero is charging up); was the CASuspendableProcess::CheckTermination stand-in. The local CThing_Retail /
//   CVectorMap / pair model gave way to the shared engine/CThing.h (FABLE_RETAIL_CTHING_GETTC_EXPANDED PeekTC(id, T**): HasTC on the
//   UsingComponents bitset, the const LowerBound 0x004365B0, then the out-pointer store only when present, as retail's null-preloaded local).
enum EPlayerMode;                                  // MS forward enum (CPlayer::RemoveMode 0x00634190 takes it)
#include "fable_compiler.h"
class CProcessedInput;
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#include "engine/CThing.h"
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTCSPECIALABILITIES_FULL
#define FABLE_RETAIL_CTCSPECIALABILITIES_ISCHARGINGUPABILITY
#include "engine/CTCSpecialAbilities.h"
class CThingPlayerCreature : public CThing { };
class CPlayer {
public:
    CThingPlayerCreature* GetPControlledCreature() const;              // 0x00487DC0
    void RemoveMode(EPlayerMode mode);                    // 0x00634190
};
class CWorld; class CGameDefinitionManager; class CMainGameComponent; class CGamePlayerInterface;
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
class CInputProcessWatchForWillChargeUpThirdPersonModeTermination : public CInputProcessGameBase {
public:
    long QuitModeTimer;                            // +0x28
    virtual void Update(CProcessedInput& processed);
};

void CInputProcessWatchForWillChargeUpThirdPersonModeTermination::Update(CProcessedInput& processed)
{
    CThing* hero = MyPlayer->GetPControlledCreature();
    if (hero && !hero->IsDead()) {
        const CTCSpecialAbilities* charge = 0;
        hero->PeekTC(TCI_SPECIAL_ABILITIES, &charge);
        if (charge->IsChargingUpAbility()) {
            QuitModeTimer = 5;
            return;
        }
        if (--QuitModeTimer < 0)
            MyPlayer->RemoveMode((EPlayerMode)(0x23));
    }
}