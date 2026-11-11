// CTCAIScratchpad::IsComponentSerialisable() const -- returns true
// Leaf: ?IsComponentSerialisable@CTCAIScratchpad@@MBE_NXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x007ACDD0, thiscall, `ret`.
// Identity: slot 23 of CTCAIScratchpad's vtable 0x0126D07C (defined here: the slot holds this body in 1 vtable(s), all of CTCAIScratchpad or derived); the Xbox 360 Test PDB virtual order (third source) aligned on 16 TLC-named slots of that vtable puts IsComponentSerialisable(void) at this slot; Ego_r/FableWin name set: ?IsComponentSerialisable@CTCAIScratchpad@@MBE_NXZ; `ret` matches the signature.
// Header: engine/CTCAIScratchpad.h (opt-ins as in the landed row CAIStateGroupScriptingBaseCAIStateGroupBase_Fired_009350c0.cpp).
#define FABLE_RETAIL_CTCAISCRATCHPAD_VSLOTS
#define FABLE_RETAIL_CAIBRAIN_GETCREATURE
#define FABLE_RETAIL_CAISTATEGROUPBASE_GETCREATURE
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGAICREATURE
#define FABLE_RETAIL_CAISTATEGROUPBASE
#define FABLE_RETAIL_CAISTATEGROUPBASE_MEMBERS
#include "fable_string.h"
#include "engine/CThing.h"
#include "engine/CTCBase.h"
#include "engine/CThingCreatureBase.h"
#include "engine/CThingAICreature.h"
#include "engine/CAIStateGroupBase.h"
#include "engine/CAIBrain.h"
#include "engine/CMainGameComponent.h"
#include "engine/CWorld.h"
#include "engine/CTCAIScratchpad.h"
#include "engine/CAIStateGroup_ScriptingBase.h"
bool CTCAIScratchpad::IsComponentSerialisable() const
{
    return true;
}
