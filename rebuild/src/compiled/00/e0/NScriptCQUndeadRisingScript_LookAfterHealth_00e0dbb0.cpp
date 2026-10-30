// CQ_UndeadRisingScript::LookAfterHealth(CScriptThing undead) -- thread: while the risen undead lives, keeps it fully engageable, targetable, damageable, opaque and pushable while it is within 6 units of the UndeadRisingCentre marker, and evasive, untouchable and translucent (alpha 0.6) while it is outside
// Leaf: LookAfterHealth
// NScript (sweep 3): the script classes live in namespace NScript (Ego_r/FableWin name sets, e.g.
//   ?IsHeroDressedAsLady@CV_BordelloScript@NScript@@QAE_NXZ; every CV_/CQ_ script class is spelled ...@NScript@@ and
//   class-keyed); this row's class is now declared there (struct->class; already NScript). Same bytes.
class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);    // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
};

#include "engine/CCountedPointer.h"


#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0 (slot 0), implicit copy


// CScriptThing: the retail model from engine/CScriptThing.h (: CBaseClass, vtable 0x01238C8C, 79 slots, 0xc).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CScriptThing.h"
// Retail expands these at this row's sites (the local model had them in class): defined inline after the header.
inline bool CScriptThing::IsAlive() const { if (!PImp.Data) return false; return PImp.Data->IsAlive(); }
inline CScriptThing::CScriptThing(const CScriptThing& other) : CBaseClass(other), PImp(other.PImp) { }
inline CScriptThing& CScriptThing::operator=(const CScriptThing& other) { PImp = other.PImp; return *this; }

bool __fastcall IsDistanceBetweenThingsUnder(const CScriptThing& a, const CScriptThing& b, float distance);   // 0x00CBE2FF

// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"

namespace NScript {
}   // namespace NScript (CScriptBase is global in retail)
// CScriptBase: the retail NScript model from the shared headers (FABLE_RETAIL_NSCRIPT: CScriptBase :
//   CBaseClassNonCopyable : CBaseClass, vtable 0x012C1648, 5 slots, 0x40).
#include "engine/CScriptBase.h"
namespace NScript {

class CQ_UndeadRisingScript : public CScriptBase {
public:
    CGameScriptInterface* GSI;                          // +0x40
    void LookAfterHealth(CScriptThing undead);
};

void CQ_UndeadRisingScript::LookAfterHealth(CScriptThing undead)
{
    GSI->GetHealth(undead);
    CScriptThing centre = GSI->GetThingWithScriptName("UndeadRisingCentre");
    while (undead.IsAlive()) {
        if (IsActiveThreadTerminating())
            return;
        if (IsDistanceBetweenThingsUnder(undead, centre, 6.0f)) {
            if (IsActiveThreadTerminating())
                return;
            GSI->EntitySetAbleToBeEngagedInCombat(undead, true);
            GSI->EntitySetTargetable(undead, true);
            GSI->EntitySetAsDamageable(undead, true);
            GSI->EntitySetAsKillable(undead, true, true);
            GSI->EntitySetAlpha(undead, 1.0f, true);
            GSI->EntitySetEvadeAllHits(undead, false);
            GSI->SetIsPushableByHero(undead, true);
            GSI->EntitySetAsCollidableToThings(undead, true);
            GSI->SetIsThingForcePushable(undead, true);
            GSI->SetIsThingEpicSpellable(undead, true);
        } else {
            if (IsActiveThreadTerminating())
                return;
            GSI->EntitySetAbleToBeEngagedInCombat(undead, false);
            GSI->EntitySetTargetable(undead, false);
            GSI->EntitySetAsDamageable(undead, false);
            GSI->EntitySetAsKillable(undead, false, true);
            GSI->EntitySetAlpha(undead, 0.6f, true);
            GSI->EntitySetEvadeAllHits(undead, true);
            GSI->SetIsPushableByHero(undead, false);
            GSI->EntitySetAsCollidableToThings(undead, false);
            GSI->SetIsThingForcePushable(undead, false);
            GSI->SetIsThingEpicSpellable(undead, false);
        }
        GSI->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
}
}