// CCreatureActionBase::SetPItemRemoved(CIntelligentPointer<CThing>) -- PItemRemoved (+0x2C) = the by-value pointer's target; the argument is destroyed here
// Leaf: ?SetPItemRemoved@CCreatureActionBase@@QAEXV?$CIntelligentPointer@VCThing@@@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x0072D360, 34 bytes: push esi ; mov esi,ecx ; lea ecx,[esp+8] ; call GetPItem ; push eax ;
//   lea ecx,[esi+2Ch] ; call SetPItem ; lea ecx,[esp+8] ; call ~CBaseIntelligentPointer ; pop esi ; ret 8.
// Identity (agentNF19, from chunk H40i's claim): Ego_r.pdb 0x00525E80 / FableWin.pdb 0x01DD0230
//   ?SetPItemRemoved@CCreatureActionBase@@QAEXV?$CIntelligentPointer@VCThing@@@@@Z; the store goes to +0x2C, PItemRemoved in the
//   shared retail CCreatureActionBase model; caller CTCSpecialAbilities::SetupBerserk 0x0072CBA0 sets it on a new berserk cast.
//   Was the shape row CItemOwner_0072d360::SetItem_CItemOwner_0072d360 (catalog _unattributed; manifest label
//   CTCInventoryBase::GetItemDescription, propagated).
// Callees: CBaseIntelligentPointer::GetPItem 0x00A01B50, SetPItem 0x00A01B90, ~CBaseIntelligentPointer 0x00A01C10.
// Model: the shared retail CCreatureActionBase (FULL_MEMBERS) on the retail CThing / CThingCreatureBase stack, as in the landed
//   CCreatureAction_PerformExpression::FrameUpdate 0x00846BC0.
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CCREATUREACTIONBASE
#define FABLE_RETAIL_CCREATUREACTIONBASE_FULL_MEMBERS
#define FABLE_RETAIL_CCREATUREACTIONBASE_SETPITEMREMOVED
#include "engine/CThingCreatureBase.h"
#include "engine/CCreatureActionBase.h"

void CCreatureActionBase::SetPItemRemoved(CIntelligentPointer<CThing> item)
{
    PItemRemoved.SetPItem(item.GetPItem());
}
