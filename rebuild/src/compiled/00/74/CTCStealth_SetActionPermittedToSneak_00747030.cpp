// CTCStealth::SetActionPermittedToSneak(const CCreatureActionBase*) -- remembers the id of the action the creature may sneak during
// Leaf: ?SetActionPermittedToSneak@CTCStealth@@QAEXPBVCCreatureActionBase@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00747030, length 19 bytes: the action's out-of-line GetActionID (0x00692620, `__thiscall`
//   on the argument) stored at +0x2C, `ret 4`.
// Name: Ego_r.pdb 0x00759460 + FableWin.pdb 0x025941C0 ?SetActionPermittedToSneak@CTCStealth@@QAEXPBVCCreatureActionBase@@@Z; Ego_r
//   CTCStealth ActionPermittedToSneakID +0x2C (long). Callee Ego_r / FableWin ?GetActionID@CCreatureActionBase@@QBEJXZ. Bound by
//   0x007EB6A0 CTCActionUseSearch::OnActionUse. NF33 (2026-11-16): renamed from the made-up CDrawEnvironmentUser_00747030::SetWorld
//   (whose CWorld::DrawGetEnvironment callee was the same 0x00692620) and moved onto engine/CTCStealth.h.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_CCREATUREACTIONBASE
#define FABLE_RETAIL_CCREATUREACTIONBASE_GETACTIONID
#include "engine/C3DVector.h"
#include "engine/CThing.h"
#include "engine/CCreatureActionBase.h"
#include "engine/CTCStealth.h"
void CTCStealth::SetActionPermittedToSneak(const CCreatureActionBase* action)
{
    ActionPermittedToSneakID = action->GetActionID();
}
