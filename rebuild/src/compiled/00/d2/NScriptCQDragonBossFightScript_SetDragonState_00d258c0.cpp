// NScript::CQ_DragonBossFightScript::SetDragonState(EDragonState state) -- record the dragon's health state and load the matching minion / summoner targets from the script def
// Leaf: ?SetDragonState@CQ_DragonBossFightScript@NScript@@QAEXW4EDragonState@12@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00D258C0, 212 bytes, `__fastcall`, one stack argument (`ret 4`).
//   FableWin symbol ?SetDragonState@CQ_DragonBossFightScript@NScript@@QAEXW4EDragonState@12@@Z. Caller: CDragon::Init
//   @ 0x00D25880 (the "Dragon" entity script, slot 2 of vtable 0x012CB770; its ParentClass +0x14), with state 0.
// Label correction: the catalog "CScriptBase_InitCheckThread" is descriptive; the caller's manifest label
//   "CBS_Teacher::Init" is propagated (it is CDragon::Init).
// Callees: CScriptBase::IsActiveThreadTerminating() @ 0x00CB7940.
// Data: pGlobalScriptDef @ 0x0143E90C: DBNumMinions{High,Medium,Low,VeryLow}Health +0xBC0..+0xBCC and
//   DBNumSummoners{...}Health +0xBD0..+0xBDC (Ego_r.pdb CScriptDef +0xC68 / +0xC78; retail = PDB - 0xA8 here, as
//   DBDragonMediumHealth PDB +0xC5C is read at +0xBB4 by CDragon::Init).
// Layout (retail = Ego_r.pdb CQ_DragonBossFightScript - 0x14, STLport vectors 12 bytes): DragonState +0x48,
//   MinionSpawnPoints +0x4C, SummonerSpawnPoints +0x58, NextSummonerSpawnPoint +0x64, TargetNumMinions +0x68,
//   TargetNumSummoners +0x6C.
// Shape: speed TU (no pragma). The state is stored first, then an if/else-if chain over the four states (`test`, then
//   `cmp 1/2/3`); each arm is the script translator's `if (IsActiveThreadTerminating()) return;` guard followed by the
//   two loads, the def pointer re-read for each (the member stores may alias it). The first arm reuses ecx = this.
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_DEF_CLASS_BASE
#include "engine/fable_script_def_global.h"          // pGlobalScriptDef 0x0143E90C (CDefPointer<const CScriptDef>; agentNF60: was a local CScriptDef model and `CScriptDef* pGlobalScriptDef`)

namespace NScript {
}   // namespace NScript (CScriptBase is global in retail)
// CScriptBase: the retail NScript model from the shared headers (FABLE_RETAIL_NSCRIPT: CScriptBase :
//   CBaseClassNonCopyable : CBaseClass, vtable 0x012C1648, 5 slots, 0x40).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CScriptBase.h"
namespace NScript {
class CQ_DragonBossFightScript : public CScriptBase {
public:
    enum EDragonState {
        DRAGON_HIGH_HEALTH,
        DRAGON_MEDIUM_HEALTH,
        DRAGON_LOW_HEALTH,
        DRAGON_VERY_LOW_HEALTH
    };
    void*         Game;                                 // +0x40
    void*         MasterScriptData;                     // +0x44
    EDragonState  DragonState;                          // +0x48
    unsigned char MinionSpawnPoints[12];                // +0x4c vector<CScriptThing>
    unsigned char SummonerSpawnPoints[12];              // +0x58 vector<CScriptThing>
    long          NextSummonerSpawnPoint;               // +0x64
    long          TargetNumMinions;                     // +0x68
    long          TargetNumSummoners;                   // +0x6c
    void SetDragonState(EDragonState state);
};
void CQ_DragonBossFightScript::SetDragonState(EDragonState state)
{
    DragonState = state;
    if (state == DRAGON_HIGH_HEALTH) {
        if (IsActiveThreadTerminating())
            return;
        TargetNumMinions = pGlobalScriptDef->DBNumMinionsHighHealth;
        TargetNumSummoners = pGlobalScriptDef->DBNumSummonersHighHealth;
    }
    else if (state == DRAGON_MEDIUM_HEALTH) {
        if (IsActiveThreadTerminating())
            return;
        TargetNumMinions = pGlobalScriptDef->DBNumMinionsMediumHealth;
        TargetNumSummoners = pGlobalScriptDef->DBNumSummonersMediumHealth;
    }
    else if (state == DRAGON_LOW_HEALTH) {
        if (IsActiveThreadTerminating())
            return;
        TargetNumMinions = pGlobalScriptDef->DBNumMinionsLowHealth;
        TargetNumSummoners = pGlobalScriptDef->DBNumSummonersLowHealth;
    }
    else if (state == DRAGON_VERY_LOW_HEALTH) {
        if (IsActiveThreadTerminating())
            return;
        TargetNumMinions = pGlobalScriptDef->DBNumMinionsVeryLowHealth;
        TargetNumSummoners = pGlobalScriptDef->DBNumSummonersVeryLowHealth;
    }
}
}