// CTCSinglePersonOccupiable::SetOccupier(CThingCreatureBase* occupier) -- stores the occupier through a temporary intelligent pointer
// Leaf: ?SetOccupier@CTCSinglePersonOccupiable@@QAEXPAVCThingCreatureBase@@@Z
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x0087C390, 71 bytes: a stack CIntelligentPointer<CThingCreatureBase> (base ctor 0x00A01B10, vtable
//   0x0123E7D8, SetPItem 0x00A01B90 with the argument), GetPItem 0x00A01B50 on it, SetPItem into POccupier (+0x0C), the temporary
//   destroyed (0x00A01C10); `ret 4`.
// Identity (two sources): Ego_r.pdb ?SetOccupier@CTCSinglePersonOccupiable@@QAEXPAVCThingCreatureBase@@@Z and FableWin.pdb (same
//   spelling); Ego_r donor order SetOccupier / GetOccupier / IsOccupied / IsOfType = retail 0x0087C390 / 0x0087C3E0 / 0x0087C3F0 /
//   0x0087C420, POccupier the Ego_r member at +0x0C (engine/CTCSinglePersonOccupiable.h). Its callers fetch the interface-172
//   (TCI_SINGLE_PERSON_OCCUPIABLE) component first (CAIStateGroup_GoToBed::OnReplace 0x0092C740, CAIStateGroup_SleepOnFloor::OnReplace
//   0x00969AE0). The old catalog name "_unattributed::AssignTarget_0087c390" was generated for a repeated body shape.
// Shape: `POccupier = CIntelligentPointer<CThingCreatureBase>(occupier);` (the inline (T*) ctor and the inline ASSIGN operator=).
// Header: engine/CTCSinglePersonOccupiable.h, engine/CThingCreatureBase.h (the retail chain, so the pointer conversions are no-ops);
//   preamble as the landed GetOccupier 0x0087C3E0.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_ASSIGN
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#include "fable_string.h"
#include "engine/CThing.h"
#include "engine/CThingCreatureBase.h"
#include "engine/CTCSinglePersonOccupiable.h"
void CTCSinglePersonOccupiable::SetOccupier(CThingCreatureBase* occupier)
{
    POccupier = CIntelligentPointer<CThingCreatureBase>(occupier);
}
