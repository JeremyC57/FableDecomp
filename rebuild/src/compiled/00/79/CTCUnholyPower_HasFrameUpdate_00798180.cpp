// CTCUnholyPower::HasFrameUpdate() const -- returns true
// Leaf: ?HasFrameUpdate@CTCUnholyPower@@UBE_NXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00798180, thiscall, `ret`.
// Identity: slot 8 of CTCUnholyPower's vtable 0x0126BFDC (defined here: the slot holds this body in 1 vtable(s), all of CTCUnholyPower or derived); the Xbox 360 Test PDB virtual order (third source) aligned on 14 TLC-named slots of that vtable puts HasFrameUpdate(void) at this slot; Ego_r/FableWin name set: ?HasFrameUpdate@CTCUnholyPower@@UBE_NXZ; `ret` matches the signature.
// Header: engine/CTCUnholyPower.h (opt-ins as in the landed row CTCUnholyPower_CancelForCutscene_00794e60.cpp).
#define FABLE_RETAIL_CTCUNHOLYPOWER_CANCELFORCUTSCENE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_MOREACCESS
#define FABLE_RETAIL_CTHING_STATEACCESS
#define FABLE_RETAIL_CTHING_ADDANDCOPYTC
#define FABLE_RETAIL_CTHING_GETTC_ORIGINAL_CHAIN
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGCREATUREBASE_FINISHACTION
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COPY
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COPY_ORIGINAL_CHAIN
#define FABLE_RETAIL_CINTELLIGENTPOINTER_GET_ORIGINAL_CHAIN
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COMPARE_NULL_INT
#define FABLE_RETAIL_IS_THING_ALIVE
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCSPECIALABILITIES_FULL
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_CWORLD_ENVIRONMENT
#define FABLE_RETAIL_CENVIRONMENT
#define FABLE_RETAIL_CENVIRONMENT_DEFAULTTHEME
#define FABLE_RETAIL_CTCSOUNDPLAYER
#include "engine/CThingCreatureBase.h"
#include "engine/CTCUnholyPower.h"
#include "engine/CTCSpecialAbilities.h"
#include "engine/CWorld.h"
#include "engine/CEnvironment.h"
#include "engine/ThingAlive.h"
bool CTCUnholyPower::HasFrameUpdate() const
{
    return true;
}
