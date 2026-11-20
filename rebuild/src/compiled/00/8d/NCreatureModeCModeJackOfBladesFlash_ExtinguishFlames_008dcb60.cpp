// NCreatureMode::CModeJackOfBladesFlash::ExtinguishFlames() -- put out the flames (TCI_BURNT_EFFECT) on the live target when the creature perceives it, then on every live AI creature other than the creature itself
// Leaf: ?ExtinguishFlames@CModeJackOfBladesFlash@NCreatureMode@@AAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x008DCB60, 279 bytes (`ret`); called by CModeJackOfBladesFlash::OnDeactivate 0x008DF310.
// Callees: CBaseIntelligentPointer::GetPItem 0x00A01B50 (IS_THING_ALIVE on PTarget, then the compare and the RemoveTC target),
//   CVectorMap LowerBound 0x0040F020 (GetTC of TCI_PERCEIVE_THING 0x34 on PCreature), CTCPerceiveThing::GetPerceivedThing 0x007446A0 (twice),
//   CThing::RemoveTC 0x004C9840 (TCI_BURNT_EFFECT 0x89), CModeBase::GetWorld 0x0073A680, CThingSearchTools::GetAllThings<CThingAICreature,
//   vector<CThingAICreature*>, CThingFilter_IsNot> 0x008E3BB0 (CWorld +0x20), the vector's free 0x00BFEA14.
// Layout: Ego_r.pdb CModeJackOfBladesFlash : CModeStandBase (0x68): Timer +0x68, BurnCounter +0x6c, Filter +0x70, PCreature +0x7c,
//   PTarget +0x80, PEmitter +0x88, PSpotLight +0x90.
// Naming (NF135, 2026-11-20; q6453 / q6455): moved off the local CThing / CThingCreatureBase / CBaseIntelligentPointer / CIntelligentPointer /
//   CTCPerception / vector / CThingFilter_NearCreature / CThingSearchTools / CWorld models, the local std::pair / CVectorMap specialisation and the
//   same-TU GetAllThings stand-in body onto engine/CModeStandBase.h (FABLE_RETAIL_CMODESTANDBASE) over the shared nested
//   CTCCreatureModeManager::CModeBase (VIRTUALS: GetWorld), engine/CThing.h (GETTC_EXPANDED, STATEACCESS RemoveTC), engine/CThingAICreature.h,
//   engine/CIntelligentPointer.h + engine/ThingAlive.h, engine/CTCPerceiveThing.h (FABLE_RETAIL_CTCPERCEIVETHING: the stand-in CTCPerception is
//   CTCPerceiveThing, TCI_PERCEIVE_THING 0x34), engine/CThingSearchTools.h (ALLTHINGSBODY: the header body of GetAllThings, same-TU noinline as
//   HEAD needed), engine/CThingFilter_IsNot.h (the filter built from PCreature; HEAD: the descriptive CThingFilter_NearCreature), STLport
//   std::vector<CThingAICreature*> and engine/CWorld.h. RELOCATION_MATCH in one form.
//   Self symbol ?ExtinguishFlames@CModeJackOfBladesFlash@@QAEXXZ -> ?ExtinguishFlames@CModeJackOfBladesFlash@NCreatureMode@@AAEXXZ (namespace +
//   private: Ego_r.pdb 0x0081E350, FableWin.pdb 0x0284B620); its one landed referencer 0x008DF310 moves from ...@NCreatureMode@@QAEXXZ to the same
//   AAE spelling in this hand-back (a 2-row family; the address had two spellings, now one). Callee 0x008E3BB0: the stand-in
//   ??$GetAllThings@UCThingFilter_NearCreature@@... becomes the PDB instance ??$GetAllThings@VCThingAICreature@@V?$vector@PAVCThingAICreature@@...
//   (Ego_r.pdb 0x00825CE0, FableWin.pdb 0x0286CD30; the owner row 0x008E3BB0 documents the same template arguments). The class is the
//   row-local model HEAD had, now in NCreatureMode (RTTI .?AVCModeJackOfBladesFlash@NCreatureMode@@ : CModeStandBase) over the shared base;
//   no shared header for it anywhere under rebuild/include.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CTHING_STATEACCESS
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COMPARE_NULL_INT
#define FABLE_RETAIL_CINTELLIGENTPOINTER_DEREFERENCE
#define FABLE_RETAIL_IS_THING_ALIVE
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGAICREATURE
#define FABLE_RETAIL_CTHINGSEARCHTOOLS
#define FABLE_RETAIL_CTHINGSEARCHTOOLS_ALLTHINGSBODY
#define FABLE_RETAIL_CTCPERCEIVETHING
#define FABLE_RETAIL_CMODEBASE_VIRTUALS
#define FABLE_RETAIL_CMODESTANDBASE
#define FABLE_RETAIL_CWORLD
#include "stlport/fable_containers.h"
#include "fable_string.h"
#include "engine/C3DVector.h"
#include "engine/ETCInterfaceType.h"
#include "engine/CThing.h"
#include "engine/CThingCreatureBase.h"
#include "engine/CThingAICreature.h"
#include "engine/ThingAlive.h"
#include "engine/CTCPerceiveThing.h"
#include "engine/CThingSearchTools.h"
#include "engine/CThingFilter_IsNot.h"
#include "engine/CTCCreatureModeManager.h"
#include "engine/CModeStandBase.h"
#include "engine/CAnimFilterOfGroup.h"
#include "engine/CWorld.h"

namespace NCreatureMode {
// Ego_r.pdb / FableWin.pdb ?ExtinguishFlames@CModeJackOfBladesFlash@NCreatureMode@@AAEXXZ (private); RTTI CModeJackOfBladesFlash : CModeStandBase.
// Row-local (no shared header anywhere under rebuild/include; HEAD already defined the class here, as the landed sibling 0x008DF310 does, over
// the shared CModeStandBase); only the member this row defines is declared.
class CModeJackOfBladesFlash : public CModeStandBase {
private:
    void ExtinguishFlames();
    long                                    Timer;          // +0x68
    long                                    BurnCounter;    // +0x6c
    CAnimFilterOfGroup                      Filter;         // +0x70
    CThingCreatureBase*                     PCreature;      // +0x7c
    CIntelligentPointer<CThingCreatureBase> PTarget;        // +0x80
    CIntelligentPointer<CThing>             PEmitter;       // +0x88
    CIntelligentPointer<CThing>             PSpotLight;     // +0x90
};

void CModeJackOfBladesFlash::ExtinguishFlames()
{
    CTCPerceiveThing* perception;
    if (IS_THING_ALIVE(PTarget) && PCreature->GetTC(TCI_PERCEIVE_THING, &perception)) {
        if (IS_THING_ALIVE(perception->GetPerceivedThing())) {
            const CThing* target = perception->GetPerceivedThing();
            if (PTarget.Get() == target)
                PTarget->RemoveTC(TCI_BURNT_EFFECT);
        }
    }
    {
        CThingFilter_IsNot filter(*PCreature);
        std::vector<CThingAICreature*> things;
        GetWorld().GetThingSearchTools().GetAllThings<CThingAICreature>(filter, things);
        long count = things.size();
        for (long i = 0; i < count; ++i) {
            CThingAICreature* thing = things[i];
            if (IS_THING_ALIVE(thing))
                thing->RemoveTC(TCI_BURNT_EFFECT);
        }
    }
}

}
