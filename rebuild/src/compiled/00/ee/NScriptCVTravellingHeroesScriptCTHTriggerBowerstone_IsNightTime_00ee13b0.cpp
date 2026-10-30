// NScript::CV_TravellingHeroesScript::CTH_Trigger_Bowerstone::IsNightTime() -- night is after 18:00 or before 07:00 (HHMM game time); never while the script thread is terminating
// Leaf: ?IsNightTime@CTH_Trigger_Bowerstone@CV_TravellingHeroesScript@NScript@@AAE_NXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00EE13B0, length 65 bytes (+ int3 padding).
// Callees: CGameScriptInterface::GetTimeOfDay() const, slot 647 (+0xa1c) of the quest interface at +4 (twice; Ego_r
// ?GetTimeOfDay@CGameScriptInterface@@UBEJXZ via ghidra_out/cgsi_slots_aligned.tsv), the script abort test @ 0x00F35B30
// (`GetScriptBase()->IsActiveThreadTerminating()`; manifest "CCreatureAction_TrollWhackGroundBase::Initialise" is
// propagated -- named IsAborted as in the landed CStatueMasterChest::Main 0x00ED4830). Called by 0x00EE0DE7 / 0x00EE0FFA /
// 0x00EE10D7. Ego_r.pdb: ?IsNightTime@CTH_Trigger_Bowerstone@CV_TravellingHeroesScript@NScript@@AAE_NXZ.
// Shape: the abort test runs in both time outcomes; in the daytime arm its result is discarded (`call ; xor al,al`),
// in the night arm `if (!IsActiveThreadTerminating()) return true;` jumps (`jne`) back into the shared `xor al,al` tail, else `mov al,1`.
// Graded forms: a bool local `night` then `if (IsActiveThreadTerminating()) return false; return night;` if-converts (`neg ; sbb ; not ;
// and`); a result local cleared by the abort test parks it in bl (65v65); `if (IsActiveThreadTerminating()) return false; return true;`
// in the night arm becomes `neg ; sbb ; inc` (65v65); this form matches.
namespace NScript { class CPredicateBase; }
class CEntityScriptBase
{
public:
    bool IsActiveThreadTerminating() const;   // 0x00F35B30
};

namespace NScript {

// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"

class CV_TravellingHeroesScript { public:
    struct CTH_Trigger_Bowerstone : public CEntityScriptBase {
        void*                 __vftable;            // +0x00
        CGameScriptInterface* PQuest;               // +0x04
    
private:
    bool IsNightTime();
};
};

bool CV_TravellingHeroesScript::CTH_Trigger_Bowerstone::IsNightTime()
{
    if (PQuest->GetTimeOfDay() > 1800 || PQuest->GetTimeOfDay() < 700) {
        if (!IsActiveThreadTerminating())
            return true;
    } else {
        IsActiveThreadTerminating();
    }
    return false;
}

}