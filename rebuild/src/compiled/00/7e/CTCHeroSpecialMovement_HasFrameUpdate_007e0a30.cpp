// CTCHeroSpecialMovement::HasFrameUpdate() const -- returns true
// Leaf: ?HasFrameUpdate@CTCHeroSpecialMovement@@UBE_NXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x007E0A30, thiscall, `ret`.
// Identity: slot 8 of CTCHeroSpecialMovement's vtable 0x0126FE3C (defined here: the slot holds this body in 1 vtable(s), all of CTCHeroSpecialMovement or derived); the Xbox 360 Test PDB virtual order (third source) aligned on 16 TLC-named slots of that vtable puts HasFrameUpdate(void) at this slot; Ego_r/FableWin name set: ?HasFrameUpdate@CTCHeroSpecialMovement@@UBE_NXZ; `ret` matches the signature.
// Header: engine/CTCHeroSpecialMovement.h (opt-ins as in the landed row CTCHeroSpecialMovement_OnCreate_007e0910.cpp).
#define FABLE_RETAIL_CTCHEROSPECIALMOVEMENT_VSLOTS
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
#define FABLE_RETAIL_CHEROSPECIALMOVEMENTDEF
#define FABLE_RETAIL_CGAMECOMPONENT
#define FABLE_RETAIL_CMAINGAMECOMPONENT
#define FABLE_RETAIL_SCOPED_PTR
#include "fable_string.h"
#include "engine/CThing.h"
#include "engine/CThingBaseDef.h"
#include "engine/CHeroSpecialMovementDef.h"
#include "engine/CGameDefinitionManager.h"
#include "engine/CMainGameComponent.h"
#include "engine/CTCHeroSpecialMovement.h"
bool CTCHeroSpecialMovement::HasFrameUpdate() const
{
    return true;
}
