// CTCCombat::HasFrameUpdate() const -- returns true
// Leaf: ?HasFrameUpdate@CTCCombat@@EBE_NXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00739D10, thiscall, `ret`.
// Identity: slot 8 of CTCCombat's vtable 0x0126724C (defined here: the slot holds this body in 1 vtable(s), all of CTCCombat or derived); the Xbox 360 Test PDB virtual order (third source) aligned on 15 TLC-named slots of that vtable puts HasFrameUpdate(void) at this slot; Ego_r/FableWin name set: ?HasFrameUpdate@CTCCombat@@EBE_NXZ; `ret` matches the signature.
// Header: engine/CTCCombat.h (opt-ins as in the landed row CTCCombat_AddMeleeFlourishAbility_00736640.cpp).
#define FABLE_RETAIL_CTCCOMBAT_VSLOTS
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_STLP_LIST
#define FABLE_RETAIL_STLP_MAP
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CDEFINITIONMANAGER_GETDEF_GLOBAL_BODY
#define FABLE_RETAIL_CGAMEDEFINITIONMANAGER
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CRESOURCE
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CCOMBATABILITY
#define FABLE_RETAIL_CCOMBATABILITYDATA
#define FABLE_RETAIL_CTCCOMBAT
#define FABLE_RETAIL_CTCCOMBAT_FLOURISH
#define FABLE_RETAIL_CTCCOMBAT_ABILITYLIST
#include "fable_string.h"
#include "engine/CThing.h"
#include "engine/CDefClassBase.h"
#include "engine/CGameDefinitionManager.h"
#include "engine/CTCCombat.h"
bool CTCCombat::HasFrameUpdate() const
{
    return true;
}
