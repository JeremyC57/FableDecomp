// CTCGhostSword::OnKill() -- does nothing
// Leaf: ?OnKill@CTCGhostSword@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x007C5460, thiscall, `ret`.
// Identity: slot 14 of CTCGhostSword's vtable 0x0126E494 (defined here: the slot holds this body in 1 vtable(s), all of CTCGhostSword or derived); the Xbox 360 Test PDB virtual order (third source) aligned on 14 TLC-named slots of that vtable puts OnKill(void) at this slot; Ego_r/FableWin name set: ?OnKill@CTCGhostSword@@UAEXXZ; `ret` matches the signature.
// Header: engine/CTCGhostSword.h (opt-ins as in the landed row CTCGhostSword_FrameUpdate_007c6460.cpp).
#define FABLE_RETAIL_CTCGHOSTSWORD_VSLOTS
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_STATEACCESS
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COMPARE_NULL_INT
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGAICREATURE
#define FABLE_RETAIL_IS_THING_ALIVE
#define FABLE_RETAIL_CTCGRAPHICAPPEARANCE_SETALPHA
#define FABLE_RETAIL_CTCGHOSTSWORD
#define FABLE_RETAIL_CTCGHOSTSWORD_FRAME
#include "fable_string.h"
#include "engine/CThingAICreature.h"
#include "engine/ThingAlive.h"
#include "engine/NGraphicAppearance_CTCGraphicAppearance.h"
#include "engine/CTCGhostSword.h"
void CTCGhostSword::OnKill()
{
}
