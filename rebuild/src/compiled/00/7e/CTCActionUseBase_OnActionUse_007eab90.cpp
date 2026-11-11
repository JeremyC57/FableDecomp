// CTCActionUseBase::OnActionUse(CThing&) -- does nothing
// Leaf: ?OnActionUse@CTCActionUseBase@@UAEXAAVCThing@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x007EAB90, thiscall, `ret 4`.
// Identity: slot 28 of CTCCarriedActionUseBase's vtable 0x0123F894 (defined here: the slot holds this body in 2 vtable(s), all of CTCActionUseBase or derived); the Xbox 360 Test PDB virtual order (third source) aligned on 13 TLC-named slots of that vtable puts OnActionUse(CThing &) at this slot; Ego_r/FableWin name set: ?OnActionUse@CTCActionUseBase@@UAEXAAVCThing@@@Z; `ret` matches the signature.
// Header: engine/CTCActionUseBase.h (opt-ins as in the landed row CTCActionUseBase_OnCreate_007eac10.cpp).
class CThing;
#define FABLE_RETAIL_CTCACTIONUSEBASE_VSLOTS
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_STLP_LIST
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CDEFINITIONMANAGER_GETDEF_GLOBAL_BODY
#define FABLE_RETAIL_CGAMEDEFINITIONMANAGER
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CRESOURCE
#define FABLE_RETAIL_CTCACTIONUSEBASE_ONCREATE
#include "fable_string.h"
#include "engine/CThing.h"
#include "engine/CThingBaseDef.h"
#include "engine/CGameDefinitionManager.h"
#include "engine/CTCActionUseBase.h"
void CTCActionUseBase::OnActionUse(CThing&)
{
}
