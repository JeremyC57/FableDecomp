// CTCTrapBase::HasFrameUpdate() const -- returns true
// Leaf: ?HasFrameUpdate@CTCTrapBase@@UBE_NXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x007891EF, thiscall, `ret`.
// Identity: slot 8 of CTCProjectileTrap's vtable 0x0126B90C (defined here: the slot holds this body in 3 vtable(s), all of CTCTrapBase or derived); the Xbox 360 Test PDB virtual order (third source) aligned on 14 TLC-named slots of that vtable puts HasFrameUpdate(void) at this slot; Ego_r/FableWin name set: ?HasFrameUpdate@CTCTrapBase@@UBE_NXZ; `ret` matches the signature.
// Header: engine/CTCTrapBase.h (opt-ins as in the landed row CTCWeaponTrail_UpdateWeaponTrails_0076efa0.cpp).
#define FABLE_RETAIL_CENGINEPERSISTENTPRIMITIVEBASE
#define FABLE_RETAIL_CTCGRAPHICAPPEARANCE_FOCAL
#define FABLE_RETAIL_CTCWEAPONTRAIL_UPDATE
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_ORIGINAL_CHAIN
#define FABLE_RETAIL_STLP_LIST_CONSTRUCT_HELPER
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGCREATUREBASE_ANIMATION
#define FABLE_RETAIL_CTHINGCREATUREBASE_ACTIONQUERY
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CCREATUREACTIONBASE
#define FABLE_RETAIL_CCREATUREACTIONBASE_FULL_MEMBERS
#define FABLE_RETAIL_CTCTRAPBASE
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_CGAMEDEFINITIONMANAGER
#define FABLE_RETAIL_CGAMEDEFINITIONMANAGER_GLOBAL
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CTCGRAPHICAPPEARANCE_MOVEMENT
#define FABLE_RETAIL_STLP_LIST
#define FABLE_RETAIL_STLP_VECTOR
#define FABLE_RETAIL_IS_THING_ALIVE
#include "engine/CThingCreatureBase.h"
#include "engine/CTCWeaponTrail.h"
#include "engine/CTCTrapBase.h"
#include "engine/CTCCarryable.h"
#include "engine/NThingShot_CTCShotDefinitionBase.h"
#include "engine/CWeaponDef.h"
#include "engine/CThingBaseDef.h"
#include "engine/CDefClassBase_GetSubDef.h"
#include "engine/NGraphicAppearance_CTCGraphicAppearance.h"
#include "engine/CCreatureActionBase.h"
#include "engine/CWorld.h"
#include "engine/CMainGameComponent.h"
#include "engine/CGlobalDef.h"
#include "engine/CGameDefinitionManager.h"
#include "engine/ThingAlive.h"
bool CTCTrapBase::HasFrameUpdate() const
{
    return true;
}
