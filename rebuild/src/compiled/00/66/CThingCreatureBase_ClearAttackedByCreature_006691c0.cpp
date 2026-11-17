// CThingCreatureBase::ClearAttackedByCreature() -- forget the creature that last attacked this one (`PLastAttackedByCreature = 0`)
// Leaf: ?ClearAttackedByCreature@CThingCreatureBase@@QAEXXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006691C0, 69 bytes.
// Identity: Ego_r.pdb 0x005454B0 + FableWin.pdb 0x01D8BB80 ?ClearAttackedByCreature@CThingCreatureBase@@QAEXXZ (engine/CThingCreatureBase.h
//   ATTACKEDBY, which documents 0x006691C0). Second source: the body assigns +0xF4 = the retail CThingCreatureBase::PLastAttackedByCreature
//   (Ego_r.pdb +0x110 less the narrower STLport containers, the offset UpdateLastAttackingCreature 0x006624A0 tests before it tail-jumps
//   here), and all three callers are CThingCreatureBase code (UpdateLastAttackingCreature, WorldUpdate 0x006666A0, and
//   CAIStateGroupBase::AIAction_FindAGuardAndYellForHelp 0x0097FEA0 on the creature, which binds this spelling).
// Callees: CBaseIntelligentPointer::CBaseIntelligentPointer() @ 0x00A01B10, SetPItem @ 0x00A01B90, GetPItem @ 0x00A01B50,
//   ~CBaseIntelligentPointer @ 0x00A01C10 -- all on the 8-byte frame temporary (CIntelligentPointer<CThingCreatureBase> vtable).
// Shape: `PLastAttackedByCreature = 0` goes through a converting CIntelligentPointer temporary (base constructor, vtable store, SetPItem(0))
//   and the inline assignment `SetPItem(other.GetPItem())`, then the temporary's destructor.
// Name fix (agentNF55, 2026-11-17): re-authored on the shared headers; it was the stand-in CAIStateGroupWithBuilding_006691c0::Cleanup
//   (the manifest label "CAIStateGroup_MinionPutOutFires::Cleanup" came from a byte-identical body).
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_ASSIGN
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGCREATUREBASE_ATTACKEDBY
#define FABLE_RETAIL_C3DVECTOR
#include "fable_string.h"
#include "engine/C3DVector.h"
#include "engine/CThing.h"
#include "engine/CThingCreatureBase.h"

void CThingCreatureBase::ClearAttackedByCreature()
{
    PLastAttackedByCreature = 0;
}
