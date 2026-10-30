// CNOVI_Villager::GetVillagerSpeechIndex(long) -- pick a random villager speech index different from the last one used (waiting a frame on a repeat), record it in the quest script and return it; 0 once the script is done
// Retail: Fable.exe (TLC, VC7.1) @ 0x00DAEBF0, 107 bytes (+ int3 padding; `ret 4`).
// Callees: CEntityScriptBase::IsActiveThreadTerminating() @ 0x00F35B30 (manifest label "CCreatureAction_TrollWhackGroundBase::Initialise" is propagated),
//   rand @ 0x00BFEB16, the game script interface's slot 7 (+0x1c, the frame update / yield).
// Layout: CEntityScriptBase {vptr, Game +4, CScriptThing Me +8}, ParentClass +0x14 (CQ_NewOakValeIntroScript; its
//   lastVillagerSpeechIdx sits at retail +0x98, PDB +0xac: the retail quest-script base is 0x14 bytes smaller).
// Shape: quest-script style -- every statement after a possible yield re-tests IsActiveThreadTerminating(); the final
//   `IsActiveThreadTerminating() ? 0 : index` is the `neg al ; sbb eax,eax ; not eax ; and eax,edi` tail.
namespace NScript { class CPredicateBase; }
// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"
class CCPPointerInfo;
#include "engine/CCountedPointer.h"
// CScriptThing: the retail model from engine/CScriptThing.h (: CBaseClass, vtable 0x01238C8C, 79 slots, 0xc).
#include "engine/CScriptThing.h"
struct CQ_NewOakValeIntroScript {
    unsigned char _pad00[0x98];
    long          lastVillagerSpeechIdx;           // +0x98 (retail)
};
extern "C" int __cdecl rand();                     // 0x00BFEB16

// CEntityScriptBase: the retail model from engine/CEntityScriptBase.h (vtable 0x012C3224, 7 slots, 0x14: Game +0x04, Me +0x08).
#include "engine/CEntityScriptBase.h"

struct CNOVI_Villager : public CEntityScriptBase {
        CQ_NewOakValeIntroScript* ParentClass;         // +0x14
    void*                     PMasterData;         // +0x18
    bool                      HeroDidHitMe;        // +0x1c
    long GetVillagerSpeechIndex(long count);
};

long CNOVI_Villager::GetVillagerSpeechIndex(long count)
{
    while (!IsActiveThreadTerminating()) {
        long index = rand() % count;
        if (index != ParentClass->lastVillagerSpeechIdx) {
            if (IsActiveThreadTerminating())
                return 0;
            ParentClass->lastVillagerSpeechIdx = index;
            return IsActiveThreadTerminating() ? 0 : index;
        }
        Game->NewScriptFrame();
    }
    return 0;
}