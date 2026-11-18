// CAIStateGroup_SleepOnFloor::OnActivate() -- reserve the sleep point for the creature (SetOccupier) and clear the asleep flag
// Leaf: ?OnActivate@CAIStateGroup_SleepOnFloor@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00969A80, length 92 bytes (+ int3 padding); Ego_r.pdb ?OnActivate@CAIStateGroup_SleepOnFloor@@UAEXXZ.
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (PSleepPoint +0x14), CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound
//   @ 0x0040F020 (the thing's interface map +0x44), CTCSinglePersonOccupiable::SetOccupier(CThingCreatureBase*) @ 0x0087C390 (Ego_r.pdb /
//   FableWin.pdb; engine/CTCSinglePersonOccupiable.h; the component is interface 0xAC = TCI_SINGLE_PERSON_OCCUPIABLE).
// Shape: the inlined GetTC lookup (presence bitset +0x20, dword 5 bit 12 -> `test ch,10h`) with its bool result IGNORED, so when the
//   bit is clear the uninitialised component slot is used (retail bug reproduced). Then the brain's creature is set as the occupier
//   and Asleep (+0x1c) is cleared.
// Layout: Ego_r.pdb CAIStateGroup_SleepOnFloor (32 bytes: PSleepPoint CIntelligentPointer<CThing> +0x14, Asleep +0x1c) over the
//   shared engine/CAIStateGroupBase.h; the class has no shared header, so its two members are declared here.
// NF79 (round 7, q10956): ported onto the shared headers (macro block of the landed GoToBed rows 0x0092D6F0 / 0x0092C740); was a
//   local CThing / CBaseIntelligentPointer / CAIBrain / CAIStateGroupBase model calling 0x0087C390 as the stand-in
//   CVillageTaskBase::AssignCreatureToTask(CThing*) (a propagated manifest label); member names now Ego_r's (were PFloor / Sleeping).
#define FABLE_RETAIL_CAISTATEGROUPBASE
#define FABLE_RETAIL_CAISTATEGROUPBASE_MEMBERS
#define FABLE_RETAIL_CAISTATEGROUPBASE_GETCREATURE
#define FABLE_RETAIL_CAIBRAIN_GETCREATURE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGAICREATURE
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_C3DVECTOR
#include "engine/CThingAICreature.h"
#include "engine/CAIStateGroupBase.h"
#include "engine/CAIBrain.h"
#include "engine/CTCSinglePersonOccupiable.h"

class CAIStateGroup_SleepOnFloor : public CAIStateGroupBase {
public:
    virtual void OnActivate();
protected:
    CIntelligentPointer<CThing> PSleepPoint;                             // +0x14
    bool Asleep;                                                         // +0x1c
};

void CAIStateGroup_SleepOnFloor::OnActivate()
{
    CTCSinglePersonOccupiable* ptc_sleep_point;
    PSleepPoint->GetTC(TCI_SINGLE_PERSON_OCCUPIABLE, &ptc_sleep_point);
    ptc_sleep_point->SetOccupier(&GetCreature());
    Asleep = false;
}
