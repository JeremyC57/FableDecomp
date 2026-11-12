// CTCHaste::HasFrameUpdate() const -- returns true
// Leaf: ?HasFrameUpdate@CTCHaste@@UBE_NXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x004E0CA4, thiscall, `ret`.
// Identity: slot 8 of CTCHaste's vtable 0x012423CC (defined here: the slot holds this body in 1 vtable(s), all of CTCHaste or derived); the Xbox 360 Test PDB virtual order (third source) aligned on 16 TLC-named slots of that vtable puts HasFrameUpdate(void) at this slot; Ego_r/FableWin name set: ?HasFrameUpdate@CTCHaste@@UBE_NXZ; `ret` matches the signature.
// Header: engine/CTCHaste.h (opt-ins as in the landed row CCreatureActionTrollGroundPound_Initialise_008c7280.cpp).
#define FABLE_RETAIL_CTCHASTE_VSLOTS
#define FABLE_RETAIL_STLP_MAP
#define FABLE_RETAIL_STLP_MAP_ORIGINAL_INSERT_BODY
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CCREATUREACTIONBASE
#define FABLE_RETAIL_CCREATUREACTIONBASE_PCREATURE
#define FABLE_RETAIL_CCOMBATACTIONBASE_MELEE
#define FABLE_RETAIL_CCOMBATACTIONBASE_INITIALISE
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGPHYSICAL_HEALTH_PERCENTAGE
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTCHASTE
#define FABLE_RETAIL_CTCHASTE_MAKE_PAIR
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CRESOURCELIST
#include "fable_string.h"
#include "engine/CThingCreatureBase.h"
#include "engine/CThingBaseDef.h"
#include "engine/CDefinitionManager.h"
#include "engine/CTrollBattleDef.h"
#include "engine/CTCHaste.h"
#include "engine/CCreatureAction_TrollGroundPound.h"
bool CTCHaste::HasFrameUpdate() const
{
    return true;
}
