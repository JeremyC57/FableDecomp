// NScript::CQR_EscortTraderScript::GetSuccessType(const CScriptThing& trader) -- grade a finished escort: untouched trader at full health = 3, then two tiers by hit count and remaining health fraction from the script def, else 0
// Leaf: ?GetSuccessType@CQR_EscortTraderScript@NScript@@
// Grade: MATCH (retry 9) -- the hit-limit test reads the global directly and the fraction test reads it through a block-local
//   `CScriptDef* def = pGlobalScriptDef;` declared inside the hit-limit `if`: the global load is CSE'd into the local, which is
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

class CScriptDef { public:
    unsigned char _pad000[0x988];
    long  EscortTraderGreatMaxHits;                     // +0x988
    long  EscortTraderGoodMaxHits;                      // +0x98c
    float EscortTraderGreatMinHealthFraction;           // +0x990
    float EscortTraderGoodMinHealthFraction;            // +0x994
};
extern CScriptDef* pGlobalScriptDef;                        // 0x0143E90C (Ego_r.pdb `pGlobalScriptDef`, a def pointer: its first word)

namespace NScript {
struct CScriptBase {                                    // retail 0x40 bytes (PDB 0x54: debug containers)
    virtual void v00();
    unsigned char _body[0x3c];                          // +0x04 .. +0x40
};

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
    if (TraderHitCount <= pGlobalScriptDef->EscortTraderGreatMaxHits) {
        CScriptDef* def = pGlobalScriptDef;
        if (def->EscortTraderGreatMinHealthFraction <= GSI->GetHealth(trader) / TraderInitialHealth)
            return ESCORT_TRADER_SUCCESS_GREAT;
    }
    if (TraderHitCount <= pGlobalScriptDef->EscortTraderGoodMaxHits) {
        CScriptDef* def = pGlobalScriptDef;
        if (def->EscortTraderGoodMinHealthFraction <= GSI->GetHealth(trader) / TraderInitialHealth)
            return ESCORT_TRADER_SUCCESS_GOOD;
    }
    return ESCORT_TRADER_SUCCESS_BASIC;
}
}