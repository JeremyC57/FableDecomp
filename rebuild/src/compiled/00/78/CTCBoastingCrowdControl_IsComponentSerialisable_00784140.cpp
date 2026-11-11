// CTCBoastingCrowdControl::IsComponentSerialisable() const -- returns true
// Leaf: ?IsComponentSerialisable@CTCBoastingCrowdControl@@UBE_NXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00784140, thiscall, `ret`.
// Identity: slot 23 of CTCBoastingCrowdControl's vtable 0x0126B3AC (defined here: the slot holds this body in 1 vtable(s), all of CTCBoastingCrowdControl or derived); the Xbox 360 Test PDB virtual order (third source) aligned on 13 TLC-named slots of that vtable puts IsComponentSerialisable(void) at this slot; Ego_r/FableWin name set: ?IsComponentSerialisable@CTCBoastingCrowdControl@@UBE_NXZ; `ret` matches the signature.
// Header: engine/CTCBoastingCrowdControl.h (opt-ins as in the landed row CTCBoastingCrowdControl_GetCrowd_00783a30.cpp).
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CGAMEDEFINITIONMANAGER
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CRESOURCE
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBOASTINGCROWDCONTROL
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_CQUESTMANAGER
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_GET_ORIGINAL_CHAIN
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COPY
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COPY_ORIGINAL_CHAIN
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_IS_THING_ALIVE
#define FABLE_RETAIL_STLP_TREE_ERASE
#define FABLE_RETAIL_STLP_TREE_FIND_MEMBER_TEMPLATE
#include "engine/CThing.h"
#include "engine/CWorld.h"
#include "engine/CGameDefinitionManager.h"
#include "engine/CTCBoastingCrowdControl.h"
#include "engine/CTCCreatureGroupBoastingCrowd.h"
#include "engine/CTCDInternalMarker.h"
#include "engine/ThingAlive.h"
bool CTCBoastingCrowdControl::IsComponentSerialisable() const
{
    return true;
}
