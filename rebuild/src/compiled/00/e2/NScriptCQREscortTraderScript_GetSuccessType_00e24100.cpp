// NScript::CQR_EscortTraderScript::GetSuccessType(const CScriptThing& trader) -- grade a finished escort: untouched trader at full health = 3, then two tiers by hit count and remaining health fraction from the script def, else 0
// Leaf: ?GetSuccessType@CQR_EscortTraderScript@NScript@@QAE?AW4EscortTraderSuccessType@12@ABVCScriptThing@@@Z
// Grade: MATCH (retry 9) -- the hit-limit test reads the global directly and the fraction test reads it through a block-local
//   `const CScriptDef* def = pGlobalScriptDef.Get();` declared inside the hit-limit `if`: the global load is CSE'd into the local, which is
//   copied to ebx before the GetHealth call (`mov ebx,eax ; fcomp [ebx+990h]`); with both reads on the global (or one local
//   covering both) VC7.1 folds the field address (`lea ebx,[eax+990h] ; fcomp [ebx]`) or colours the def/trader pair the other way
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00E24100, length 170 bytes, `ret 4`.
// Identity: Ego_r.pdb `?GetSuccessType@CQR_EscortTraderScript@NScript@@QAE?AW4EscortTraderSuccessType@12@ABVCScriptThing@@@Z`
// (an enum result in eax, the trader by const reference); single caller 0x00E23392 in the escort script.
// Callees: CGameScriptInterface slot 264 GetHealth(const CScriptThing&) (+0x420, float). Data: the script def pointer
// pGlobalScriptDef @ 0x0143E90C (the name landed callers use); its escort tier fields +0x988/+0x98C (long hit limits) and
// +0x990/+0x994 (float health fractions) carry descriptive names: the retail CScriptDef does not line up with
// Ego_r.pdb here.
// Layout: retail quest base 0x48 bytes (GSI +0x40); TraderHitCount +0x64 and TraderInitialHealth +0x6C (Ego_r.pdb
// +0x78/+0x80 less the 0x14 debug-container shift).
// Shape: three early returns in source order. `GetHealth(trader) == TraderInitialHealth` gives the call then
// `fld member ; fucompp ; test ah,44h ; jp`; each tier is a hit-limit `if` holding a block-local def pointer and the
// fraction test `limit <= health / initial` (`fcomp [ebx+N] ; test ah,1 ; jne`); the global is reloaded per tier.
class CScriptThing;

// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"

// agentNF60: the local CScriptDef model named these members descriptively; engine/CScriptDef.h (Ego_r / FableWin names at the
//   retail offsets) names them: EscortTraderGreatMaxHits -> EscortTraderHitCountHigh (+0x988), EscortTraderGoodMaxHits -> EscortTraderHitCountMedium (+0x98c), EscortTraderGreatMinHealthFraction -> EscortTraderHealthHigh (+0x990), EscortTraderGoodMinHealthFraction -> EscortTraderHealthMedium (+0x994).
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_DEF_CLASS_BASE
#include "engine/fable_script_def_global.h"          // pGlobalScriptDef 0x0143E90C (CDefPointer<const CScriptDef>; agentNF60: was a local CScriptDef model and `CScriptDef* pGlobalScriptDef`)

namespace NScript {
// CScriptBase: the retail NScript model from the shared headers (FABLE_RETAIL_NSCRIPT: CScriptBase :
//   CBaseClassNonCopyable : CBaseClass, vtable 0x012C1648, 5 slots, 0x40).
#include "engine/CScriptBase.h"

class CQR_EscortTraderScript : public CScriptBase { public:
    enum EscortTraderSuccessType {
        ESCORT_TRADER_SUCCESS_BASIC   = 0,
        ESCORT_TRADER_SUCCESS_GOOD    = 1,
        ESCORT_TRADER_SUCCESS_GREAT   = 2,
        ESCORT_TRADER_SUCCESS_PERFECT = 3
    };
    CGameScriptInterface* GSI;                          // +0x40
    unsigned char _pad044[0x20];                        // +0x44 .. +0x64 (owner, mission flags, bandit counts)
    long  TraderHitCount;                               // +0x64
    long  TraderHealthBarID;                            // +0x68
    float TraderInitialHealth;                          // +0x6c
    EscortTraderSuccessType GetSuccessType(const CScriptThing& trader);
};

CQR_EscortTraderScript::EscortTraderSuccessType CQR_EscortTraderScript::GetSuccessType(const CScriptThing& trader)
{
    if (TraderHitCount == 0 && GSI->GetHealth(trader) == TraderInitialHealth)
        return ESCORT_TRADER_SUCCESS_PERFECT;
    if (TraderHitCount <= pGlobalScriptDef->EscortTraderHitCountHigh) {
        const CScriptDef* def = pGlobalScriptDef.Get();
        if (def->EscortTraderHealthHigh <= GSI->GetHealth(trader) / TraderInitialHealth)
            return ESCORT_TRADER_SUCCESS_GREAT;
    }
    if (TraderHitCount <= pGlobalScriptDef->EscortTraderHitCountMedium) {
        const CScriptDef* def = pGlobalScriptDef.Get();
        if (def->EscortTraderHealthMedium <= GSI->GetHealth(trader) / TraderInitialHealth)
            return ESCORT_TRADER_SUCCESS_GOOD;
    }
    return ESCORT_TRADER_SUCCESS_BASIC;
}
}