// CThingTrackNode::IsEntitySerialisable() const -- returns true
// Leaf: ?IsEntitySerialisable@CThingTrackNode@@UBE_NXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x008384E0, thiscall, `ret`.
// Identity: slot 22 of CThingTrackNode's vtable 0x01272CE4 (defined here: the slot holds this body in 1 vtable(s), all of CThingTrackNode or derived); the Xbox 360 Test PDB virtual order (third source) aligned on 27 TLC-named slots of that vtable puts IsEntitySerialisable(void) at this slot; Ego_r/FableWin name set: ?IsEntitySerialisable@CThingTrackNode@@UBE_NXZ; `ret` matches the signature.
// Header: engine/CThingTrackNode.h (opt-ins as in the landed row CThingTrackNode_FlipTrackDirection_00836de0.cpp).
#define FABLE_RETAIL_CTHINGTRACKNODE_VSLOTS
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGTRACKNODE
#define FABLE_RETAIL_CTHINGTRACKNODE_FLIP
#include "fable_string.h"
#include "engine/CThingTrackNode.h"
bool CThingTrackNode::IsEntitySerialisable() const
{
    return true;
}
