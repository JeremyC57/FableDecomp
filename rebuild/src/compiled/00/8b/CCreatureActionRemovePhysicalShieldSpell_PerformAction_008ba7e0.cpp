// CCreatureAction_RemovePhysicalShieldSpell::PerformAction() -- the remove-shield action's perform step: it only reads its creature
// Leaf: ?PerformAction@CCreatureAction_RemovePhysicalShieldSpell@@UAEXXZ
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x008BA7E0, 8 bytes: `add ecx,8 ; jmp 0x00A01B50` (PCreature.GetPItem(), result discarded, tail call).
// Identity (two sources): (1) slot 3 of the CCreatureAction_RemovePhysicalShieldSpell vtable 0x0127C1E4 (RTTI : CCombatActionBase :
//   CCreatureActionBase : CBaseObject), the CCreatureActionBase PerformAction slot (CBaseObject: ~ 0, Validate 1; CCreatureActionBase:
//   EditUpdate 2, PerformAction 3, as the landed 0x00854720 CCreatureAction_SpiritReturnToHero::PerformAction); (2) Ego_r.pdb
//   ?PerformAction@CCreatureAction_RemovePhysicalShieldSpell@@UAEXXZ (void), and +8 is the Ego_r CCreatureActionBase member PCreature.
// Shape: the release body is a lone out-of-line CBaseIntelligentPointer::GetPItem on PCreature whose result is unused (the same shape
//   as landed 0x008492B0); whatever else the source did there compiled to nothing in the release build, so the statement form is
//   unproven (any use that VC7.1 deletes gives the same bytes).
// Callees: ?GetPItem@CBaseIntelligentPointer@@QBEPAVCBaseObject@@XZ 0x00A01B50 (through CIntelligentPointer<CThingCreatureBase>::Get()).
// Header: engine/CCreatureActionBase.h (FABLE_RETAIL_CCREATUREACTIONBASE, _PCREATURE), engine/CIntelligentPointer.h. The generated
//   engine/CCreatureAction_RemovePhysicalShieldSpell.h is the flat Ego_r layout (included under a donor name only for the record).
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCREATUREACTIONBASE
#define FABLE_RETAIL_CCREATUREACTIONBASE_PCREATURE
#include "fable_string.h"
#include "engine/CThing.h"
#include "engine/CThingCreatureBase.h"
#include "engine/CIntelligentPointer.h"
#include "engine/CCreatureActionBase.h"
#define CCreatureAction_RemovePhysicalShieldSpell DonorCCreatureAction_RemovePhysicalShieldSpell
#include "engine/CCreatureAction_RemovePhysicalShieldSpell.h"
#undef CCreatureAction_RemovePhysicalShieldSpell
class CCreatureAction_RemovePhysicalShieldSpell : public CCreatureActionBase {   // retail base CCombatActionBase (no storage used here)
public:
    virtual void PerformAction();                       // slot 3
};
void CCreatureAction_RemovePhysicalShieldSpell::PerformAction()
{
    PCreature.Get();
}