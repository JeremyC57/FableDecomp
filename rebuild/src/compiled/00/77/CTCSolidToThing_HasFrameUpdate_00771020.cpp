// CTCSolidToThing::HasFrameUpdate() const -- returns true
// Leaf: ?HasFrameUpdate@CTCSolidToThing@@UBE_NXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00771020, thiscall, `ret`.
// Identity: slot 8 of CTCSolidToThing's vtable 0x0126A62C (defined here: the slot holds this body in 1 vtable(s), all of CTCSolidToThing or derived); the Xbox 360 Test PDB virtual order (third source) aligned on 16 TLC-named slots of that vtable puts HasFrameUpdate(void) at this slot; Ego_r/FableWin name set: ?HasFrameUpdate@CTCSolidToThing@@UBE_NXZ; `ret` matches the signature.
// Header: engine/CTCSolidToThing.h (opt-ins as in the landed row CTCCarrying_RemoveThingInCarrySlot_006a16a0.cpp).
#define FABLE_RETAIL_CTCSOLIDTOTHING_VSLOTS
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CTHING_STATEACCESS
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_ASSIGN
#define FABLE_RETAIL_STLP_LIST
#define FABLE_RETAIL_STLP_LIST_REMOVE_DECL
#define FABLE_RETAIL_CTCCARRYING
#define FABLE_RETAIL_CTCCARRYING_MEMBERS
#define FABLE_RETAIL_CTCCARRYING_SLOTACCESS
#define FABLE_RETAIL_CTCCARRYABLE_SETASCARRIEDBY
#define FABLE_RETAIL_CTCSOLIDTOTHING
#define FABLE_RETAIL_CTCCREATUREMODEMANAGER
#define FABLE_RETAIL_CTCPHYSICSBASE
#define FABLE_RETAIL_CRIGHTHANDEDSET
#define FABLE_RETAIL_CRIGHTHANDEDSET_ROTATE
#include <math.h>
#include "fable_string.h"
#include "engine/ETCInterfaceType.h"
#include "engine/CThing.h"
#include "engine/CIntelligentPointer.h"
#include "engine/CVectorMap.h"
#include "stlport/fable_containers.h"
#include "engine/CTCCarrying.h"
#include "engine/CTCCarryable.h"
#include "engine/CTCSolidToThing.h"
#include "engine/CTCCreatureModeManager.h"
#include "engine/CTCPhysicsBase.h"
#include "engine/CRightHandedSet.h"
#include "engine/NCreatureMode.h"
bool CTCSolidToThing::HasFrameUpdate() const
{
    return true;
}
