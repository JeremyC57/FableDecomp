// CQR_EscortTraderScript::ForceDisableFollow(const CScriptThing& trader) -- stop the trader following the hero: clear the follow expression, restart the expression-detect timer, drop the trader's minimap marker, stop it following, hide its emote icon and make it region-follow the hero no more
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00E24090, length 112 bytes.
// Callees: SetTimer (slot 89, +0x164) through the game script interface global @ 0x0143E8F8, and through the quest's
// interface (+0x40): slot 352 MiniMapRemoveMarker (+0x580), slot 230 EntityStopFollowing (+0x398), slot 530
// EntitySetAsDisplayingEmoteIcon (+0x848), slot 70 GetHero (+0x118), slot 480 SetEntityAsRegionFollowing (+0x780)
// (names from ghidra_out/cgsi_slots_aligned.tsv). The quest-globals record @ 0x0143E90C holds the period at +0x9a4.
// Layout: Ego_r.pdb CQR_EscortTraderScript - 0x14: ExpressionDetectedFollow +0x70, TraderExpressionDetectTimer +0x7c.
// Shape: straight-line; the last call's first argument is `GSI->GetHero()`, evaluated after the pushes of the
// other two, both calls sharing one vtable load (ebx).
class CScriptThing;
#define FABLE_RETAIL_NSCRIPT
// CTimer: the retail model from engine/CTimer.h (Ego_r.pdb CTimer {long TimerIndex}, 0x4; the setter is the retail
//   operator=(long), Ego_r 0x00CCE720). Bodies the row expands inline follow the include.
#include "engine/CTimer.h"
// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"
extern CGameScriptInterface* g_pGameScriptInterface;       // 0x0143E8F8

class CScriptDef { public:
    unsigned char _pad[0x9a4];
    long          EscortTraderExpressionDetectPeriod;       // +0x9a4
};
extern CScriptDef* pGlobalScriptDef;                      // 0x0143E90C

// CScriptBase: the retail NScript model from the shared headers (FABLE_RETAIL_NSCRIPT: CScriptBase :
//   CBaseClassNonCopyable : CBaseClass, vtable 0x012C1648, 5 slots, 0x40).
#include "engine/CScriptBase.h"

struct CQuestScriptBase : CScriptBase {                     // retail 0x48 bytes
    CGameScriptInterface* GSI;                              // +0x40
    void*                 Owner;                            // +0x44
};

struct CQR_EscortTraderScript : CQuestScriptBase {
    unsigned char _pad48[0x70 - 0x48];                      // +0x48 mission flags, counters, health bar
    bool          ExpressionDetectedFollow;                 // +0x70
    bool          ExpressionDetectedWait;                   // +0x71
    bool          AddedBonusObject;                         // +0x72
    void*         BonusObject;                              // +0x74 CCharString
    CTimer        TraderComplainTimer;                      // +0x78
    CTimer        TraderExpressionDetectTimer;              // +0x7c
    void ForceDisableFollow(const CScriptThing& trader);
};

void CQR_EscortTraderScript::ForceDisableFollow(const CScriptThing& trader)
{
    ExpressionDetectedFollow = false;
    g_pGameScriptInterface->SetTimer(TraderExpressionDetectTimer.TimerIndex, pGlobalScriptDef->EscortTraderExpressionDetectPeriod);
    GSI->MiniMapRemoveMarker(trader);
    GSI->EntityStopFollowing(trader);
    GSI->EntitySetAsDisplayingEmoteIcon(trader, false);
    GSI->SetEntityAsRegionFollowing(GSI->GetHero(), trader, false);
}