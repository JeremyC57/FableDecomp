// CTCTalk::IsComponentSerialisable() const -- returns true
// Leaf: ?IsComponentSerialisable@CTCTalk@@UBE_NXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x006E4100, thiscall, `ret`.
// Identity: slot 23 of CTCTalk's vtable 0x0125FFEC (defined here: the slot holds this body in 1 vtable(s), all of CTCTalk or derived); the Xbox 360 Test PDB virtual order (third source) aligned on 15 TLC-named slots of that vtable puts IsComponentSerialisable(void) at this slot; Ego_r/FableWin name set: ?IsComponentSerialisable@CTCTalk@@UBE_NXZ; `ret` matches the signature.
// Header: engine/CTCTalk.h (opt-ins as in the landed row CombatDialogueManager_CanSaySpeech_008af6e0.cpp).
#define FABLE_RETAIL_CTCTALK_VSLOTS
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTCTALK
#define FABLE_RETAIL_CMAINGAMECOMPONENT
#define FABLE_RETAIL_CMAINGAMECOMPONENT_WORLDFRAME
#define FABLE_RETAIL_COMBATDIALOGUEMANAGER
#include "fable_string.h"
#include "engine/CThing.h"
#include "engine/CTCBase.h"
#include "engine/CThingCreatureBase.h"
#include "engine/CTCTalk.h"
#include "engine/CMainGameComponent.h"
#include "engine/CombatDialogueManager.h"
bool CTCTalk::IsComponentSerialisable() const
{
    return true;
}
