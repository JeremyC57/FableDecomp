// CQR_EscortTrader_ManagerScript::Main() -- the repeatable trader-escort manager: on first run reset the counter and phase; then each frame run the phase machine: activate an escort (phases 0/2/4/6), or wait for the running escort to finish and for the gating quest (Q_TraderEscort / Q_BanditCamp / Q_Arena (+ activate the Orchard Farm barricade quest) / Q_OpeningGraveyardSecretPassage) to be completed before moving on; in the last phase keep activating an escort whenever the previous one has ended, the hero is away from LookoutPoint, the day is a quest day and the start time has passed
// Leaf: ?Main@CQR_EscortTrader_ManagerScript@NScript@@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00E24E10, length 1972 bytes.
// Callees: CCharString(const char*, long) 0x0099EBF0 / ~CCharString 0x0099EAE0, CScriptBase::IsActiveThreadTerminating
//   0x00CB7940, CQR_EscortTrader_ManagerScript::Activate 0x00E255D0 (landed); pGlobalScriptDef 0x0143E90C; the game script
//   interface (+0x40) slots 7, 12, 276, 284, 286, 647, 651.
// Layout (retail = Ego_r.pdb - 0x14): OrchardFarmTraderEscortCounter +0x48, DebugCounter +0x4c, Activated +0x50,
//   OrchardFarmTraderEscortPhase +0x54; CScriptDef (retail = PDB - 0xa0 here): EscortTraderQuestDayInterval +0x9d4,
//   EscortTraderQuestStartTime +0x9d8.
// Shape: the day/time tests go through inline helpers that take the CScriptDef pointer, so pGlobalScriptDef is loaded into
//   callee-saved edi before each interface call; that makes edi a saved register, which VC then also uses as the zero
//   register (xor edi,edi) for the flag word, the first-run stores and the phase-0 test.
class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);    // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
};
// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"
class CScriptDef { public:
    unsigned char _pad0[0x9d4];
    long EscortTraderQuestDayInterval;                  // +0x9d4
    long EscortTraderQuestStartTime;                    // +0x9d8
};
extern CScriptDef* pGlobalScriptDef;                        // 0x0143E90C (Ego_r.pdb `pGlobalScriptDef`, a def pointer: its first word)
enum EscortTraderManagerPhase {
    PHASE_ACTIVATE_FIRST, PHASE_WAIT_TRADER_ESCORT, PHASE_ACTIVATE_SECOND, PHASE_WAIT_BANDIT_CAMP, PHASE_ACTIVATE_THIRD,
    PHASE_WAIT_ARENA, PHASE_ACTIVATE_FOURTH, PHASE_WAIT_GRAVEYARD, PHASE_REPEAT
};
static __forceinline long DayModInterval(long day, const CScriptDef* def) { return day % def->EscortTraderQuestDayInterval; }
static __forceinline bool NotPastStartTime(long time, const CScriptDef* def) { return time <= def->EscortTraderQuestStartTime; }
namespace NScript {
}   // namespace NScript (CScriptBase is global in retail)
// CScriptBase: the retail NScript model from the shared headers (FABLE_RETAIL_NSCRIPT: CScriptBase :
//   CBaseClassNonCopyable : CBaseClass, vtable 0x012C1648, 5 slots, 0x40).
#include "engine/CScriptBase.h"
namespace NScript {
class CQR_EscortTrader_ManagerScript : public CScriptBase { public:
    CGameScriptInterface*    Game;                      // +0x40
    void*                    Data;                      // +0x44
    long                     OrchardFarmTraderEscortCounter;   // +0x48
    long                     DebugCounter;              // +0x4c
    bool                     Activated;                 // +0x50
    EscortTraderManagerPhase OrchardFarmTraderEscortPhase;     // +0x54
    void Main();
    void Activate();                                    // 0x00E255D0
};
void CQR_EscortTrader_ManagerScript::Main()
{
    if (!Activated) {
        if (IsActiveThreadTerminating())
            return;
        OrchardFarmTraderEscortCounter = 0;
        OrchardFarmTraderEscortPhase = PHASE_ACTIVATE_FIRST;
        Activated = true;
    }
    while (!IsActiveThreadTerminating()) {
        if (OrchardFarmTraderEscortPhase == PHASE_ACTIVATE_FIRST) {
            if (IsActiveThreadTerminating())
                return;
            Activate();
            OrchardFarmTraderEscortPhase = PHASE_WAIT_TRADER_ESCORT;
        } else if (OrchardFarmTraderEscortPhase == PHASE_WAIT_TRADER_ESCORT) {
            if (IsActiveThreadTerminating())
                return;
            while (Game->IsQuestActive("QR_EscortTrader") != false) {
                Game->NewScriptFrame();
                if (IsActiveThreadTerminating())
                    return;
            }
            if (IsActiveThreadTerminating())
                return;
            while (!Game->IsQuestCompleted("Q_TraderEscort")) {
                Game->NewScriptFrame();
                if (IsActiveThreadTerminating())
                    return;
            }
            if (IsActiveThreadTerminating())
                return;
            OrchardFarmTraderEscortPhase = PHASE_ACTIVATE_SECOND;
        } else if (OrchardFarmTraderEscortPhase == PHASE_ACTIVATE_SECOND) {
            if (IsActiveThreadTerminating())
                return;
            Activate();
            OrchardFarmTraderEscortPhase = PHASE_WAIT_BANDIT_CAMP;
        } else if (OrchardFarmTraderEscortPhase == PHASE_WAIT_BANDIT_CAMP) {
            if (IsActiveThreadTerminating())
                return;
            while (Game->IsQuestActive("QR_EscortTrader") != false) {
                Game->NewScriptFrame();
                if (IsActiveThreadTerminating())
                    return;
            }
            if (IsActiveThreadTerminating())
                return;
            while (!Game->IsQuestCompleted("Q_BanditCamp")) {
                Game->NewScriptFrame();
                if (IsActiveThreadTerminating())
                    return;
            }
            if (IsActiveThreadTerminating())
                return;
            OrchardFarmTraderEscortPhase = PHASE_ACTIVATE_THIRD;
        } else if (OrchardFarmTraderEscortPhase == PHASE_ACTIVATE_THIRD) {
            if (IsActiveThreadTerminating())
                return;
            Activate();
            OrchardFarmTraderEscortPhase = PHASE_WAIT_ARENA;
        } else if (OrchardFarmTraderEscortPhase == PHASE_WAIT_ARENA) {
            if (IsActiveThreadTerminating())
                return;
            while (Game->IsQuestActive("QR_EscortTrader") != false) {
                Game->NewScriptFrame();
                if (IsActiveThreadTerminating())
                    return;
            }
            if (IsActiveThreadTerminating())
                return;
            while (!Game->IsQuestCompleted("Q_Arena")) {
                Game->NewScriptFrame();
                if (IsActiveThreadTerminating())
                    return;
            }
            if (IsActiveThreadTerminating())
                return;
            Game->ActivateQuest("Q_OrchardFarm_Barricade");
            OrchardFarmTraderEscortPhase = PHASE_ACTIVATE_FOURTH;
        } else if (OrchardFarmTraderEscortPhase == PHASE_ACTIVATE_FOURTH) {
            if (IsActiveThreadTerminating())
                return;
            Activate();
            OrchardFarmTraderEscortPhase = PHASE_WAIT_GRAVEYARD;
        } else if (OrchardFarmTraderEscortPhase == PHASE_WAIT_GRAVEYARD) {
            if (IsActiveThreadTerminating())
                return;
            while (Game->IsQuestActive("QR_EscortTrader") != false) {
                Game->NewScriptFrame();
                if (IsActiveThreadTerminating())
                    return;
            }
            if (IsActiveThreadTerminating())
                return;
            while (!Game->IsQuestCompleted("Q_OpeningGraveyardSecretPassage")) {
                Game->NewScriptFrame();
                if (IsActiveThreadTerminating())
                    return;
            }
            if (IsActiveThreadTerminating())
                return;
            OrchardFarmTraderEscortPhase = PHASE_REPEAT;
        } else if (OrchardFarmTraderEscortPhase == PHASE_REPEAT) {
            if (IsActiveThreadTerminating())
                return;
            Activate();
            if (IsActiveThreadTerminating())
                return;
            for (;;) {
                while (Game->IsQuestActive("QR_EscortTrader") != false) {
                    Game->NewScriptFrame();
                    if (IsActiveThreadTerminating())
                        return;
                }
                if (IsActiveThreadTerminating())
                    return;
                while (Game->IsRegionLoaded("LookoutPoint")
                       || DayModInterval(Game->GetDayOfWeek(), pGlobalScriptDef) != 0
                       || NotPastStartTime(Game->GetTimeOfDay(), pGlobalScriptDef)) {
                    Game->NewScriptFrame();
                    if (IsActiveThreadTerminating())
                        return;
                }
                if (IsActiveThreadTerminating())
                    return;
                Activate();
                Game->NewScriptFrame();
                if (IsActiveThreadTerminating())
                    return;
            }
        }
        Game->NewScriptFrame();
    }
}
}