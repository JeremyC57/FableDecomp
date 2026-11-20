// CThingGameObject::ConstructFromLevelScript(const char* script) -- the CThingGameObject override only forwards to its base,
//   CThingPhysical::ConstructFromLevelScript (a 5-byte tail jump)
// Leaf: ?ConstructFromLevelScript@CThingGameObject@@UAE_NPBD@Z
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x00838930, 5 bytes: `jmp 0x006A5D90`.
// Identity (two sources): Ego_r.pdb 0x008135B0 + FableWin.pdb 0x027EBDD0 ?ConstructFromLevelScript@CThingGameObject@@UAE_NPBD@Z (public
//   virtual), CThingGameObject vtable 0x01244F7C slot 4 = 0x00838930 (engine/CThingGameObject.h LEVELSCRIPT), and every landed referencing
//   row (the subclass overrides 0x00662930 CThingCreatureBase, 0x00703110 CThingObject, 0x00837E40 CThingTrackNode, 0x0083BBA0, 0x0083D990,
//   HEAD compile) calls it by name under this spelling.
// Callee: ?ConstructFromLevelScript@CThingPhysical@@UAE_NPBD@Z 0x006A5D90 (Ego_r.pdb 0x00685130 + FableWin.pdb 0x01F1F560; the landed self
//   row), called non-virtually by qualified name.
// NF171 (2026-11-20, q5207 / q6211): re-authored on the shared headers (the 0x00837E40 macro set); the landed row was a header-less
//   `extern "C" void Forward()` forwarder to `_ForwardTarget_006a5d90`, with no Leaf line (old-style row).
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_LEVELSCRIPT
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGPHYSICAL_LEVELSCRIPT
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGGAMEOBJECT_LEVELSCRIPT
#include "fable_string.h"
#include "engine/CThing.h"
#include "engine/CThingPhysical.h"
#include "engine/CThingGameObject.h"

bool CThingGameObject::ConstructFromLevelScript(const char* script)
{
    return CThingPhysical::ConstructFromLevelScript(script);
}
