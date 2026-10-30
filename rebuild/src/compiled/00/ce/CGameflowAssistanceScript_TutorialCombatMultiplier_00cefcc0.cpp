// CGameflowAssistanceScript::Tutorial_CombatMultiplier() -- while assistance runs and the player wields a weapon, have the guildmaster comment when the hero's combat multiplier reaches a new high (5 progress, 10 adept, 15 excellent, past the def's master level: master and stop) or stays unimproved too long (try)
// Leaf: ?Tutorial_CombatMultiplier@CGameflowAssistanceScript@@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00CEFCC0, length 752 bytes (+ int3 padding; plain `ret`). The catalog's
//   "Tutorial_LowHealth" label is wrong: the spawner at 0x00CEFAE0 builds this thread under the name
//   "Tutorial_CombatMultiplier" (0x012C5E58), and the body only touches the CombatMult* members.
// Callees: CScriptBase::IsActiveThreadTerminating 0x00CB7940, CCharString(const char*, long) 0x0099EBF0 / ~CCharString
//   0x0099EAE0; CGameScriptInterface (at +0x40) slots 7 NewScriptFrame (+0x1c), 149 GetPlayerCreatureCombatMultiplier
//   (+0x254), 195 IsPlayerWieldingWeapon (+0x30c), 274 HeroReceiveMessageFromGuildMaster (+0x448), 394
//   IsHeroControlledByPlayer (+0x628) (names from ghidra_out/cgsi_slots_aligned.tsv).
// Data: the script def 0x0143E90C (+0xfd8 frames-per-level allowance before the "try" nag, +0xfdc master level; no PDB
//   name found at those retail offsets), "" 0x0122D70E, "TEXT_QST_080_MULT_EXCELLENT" 0x012C5EDC, "..._ADEPT" 0x012C5EC4,
//   "..._PROGRESS" 0x012C5EA8, "..._TRY" 0x012C5E90, "..._MASTER" 0x012C5E74.
// Layout: retail = Ego_r.pdb CGameflowAssistanceScript - 0x14 (as the landed quest scripts): the interface +0x40, the
//   master data +0x44 (its stop flag at +0x67), CombatMultHighestLevel +0x50, CombatMultFramesSinceChange +0x54.
// Shape: the outer loop is NOT rotated (a top `jmp` into the flag test): write it `for (;;) { if (flag) break; ... }` with the
//   bottom as `if (!IsActiveThreadTerminating()) continue; return;` (a plain `while (!flag)` or a bottom `if (term) return;`
//   rotates it and duplicates the flag test). Every action is guarded by its own IsActiveThreadTerminating
//   check; the message temporaries are built right to left ("" first) from implicit `const char*` conversions.
class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);        // 0x0099EBF0
    ~CCharString();                                         // 0x0099EAE0
};
// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"

class CScriptDef { public:
    unsigned char _pad[0xfd8];
    long          CombatMultFramesPerLevel;                 // +0xfd8
    long          CombatMultMasterLevel;                    // +0xfdc
};
extern CScriptDef* pGlobalScriptDef;                            // 0x0143E90C (Ego_r.pdb `pGlobalScriptDef`, a def pointer: its first word)

struct CGameflowMasterData {
    unsigned char _pad[0x67];
    bool          StopAssistance;                           // +0x67
};
class CScriptBase { public:
    unsigned char _body[0x40];
    bool IsActiveThreadTerminating();                       // 0x00CB7940
};
struct CGameflowAssistanceScript : CScriptBase {
    CGameScriptInterface* GSI;                              // +0x40
    CGameflowMasterData*  PMasterData;                      // +0x44
    long                  FlourishesOffered;                // +0x48
    long                  FlourishesPerformed;              // +0x4c
    long                  CombatMultHighestLevel;           // +0x50
    long                  CombatMultFramesSinceChange;      // +0x54
    long                  HealthWarningsGiven;              // +0x58
    long                  WillEnergyWarningsGiven;          // +0x5c
    long                  SavedRenownLevel;                 // +0x60
    long                  SaveWarningsGiven;                // +0x64
    void Tutorial_CombatMultiplier();
};

void CGameflowAssistanceScript::Tutorial_CombatMultiplier()
{
    if (IsActiveThreadTerminating())
        return;
    for (;;) {
        if (PMasterData->StopAssistance)
            break;
        if (GSI->IsPlayerWieldingWeapon() && GSI->IsHeroControlledByPlayer()) {
            if (IsActiveThreadTerminating())
                return;
            long multiplier = GSI->GetPlayerCreatureCombatMultiplier();
            if (multiplier > CombatMultHighestLevel) {
                if (IsActiveThreadTerminating())
                    return;
                CombatMultHighestLevel = multiplier;
                CombatMultFramesSinceChange = 0;
                if (multiplier > pGlobalScriptDef->CombatMultMasterLevel) {
                    if (IsActiveThreadTerminating())
                        return;
                    GSI->HeroReceiveMessageFromGuildMaster("TEXT_QST_080_MULT_MASTER", "", true, false);
                    return;
                }
                if (IsActiveThreadTerminating())
                    return;
                if (CombatMultHighestLevel == 15) {
                    if (IsActiveThreadTerminating())
                        return;
                    GSI->HeroReceiveMessageFromGuildMaster("TEXT_QST_080_MULT_EXCELLENT", "", true, false);
                } else if (CombatMultHighestLevel == 10) {
                    if (IsActiveThreadTerminating())
                        return;
                    GSI->HeroReceiveMessageFromGuildMaster("TEXT_QST_080_MULT_ADEPT", "", true, false);
                } else if (CombatMultHighestLevel == 5) {
                    if (IsActiveThreadTerminating())
                        return;
                    GSI->HeroReceiveMessageFromGuildMaster("TEXT_QST_080_MULT_PROGRESS", "", true, false);
                }
            } else {
                if (IsActiveThreadTerminating())
                    return;
                ++CombatMultFramesSinceChange;
                if (CombatMultFramesSinceChange > pGlobalScriptDef->CombatMultFramesPerLevel * CombatMultHighestLevel) {
                    if (IsActiveThreadTerminating())
                        return;
                    GSI->HeroReceiveMessageFromGuildMaster("TEXT_QST_080_MULT_TRY", "", true, false);
                    CombatMultFramesSinceChange = 0;
                }
            }
        }
        GSI->NewScriptFrame();
        if (!IsActiveThreadTerminating())
            continue;
        return;
    }
    if (IsActiveThreadTerminating())
        return;
}