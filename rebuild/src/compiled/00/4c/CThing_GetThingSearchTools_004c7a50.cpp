// CThing::GetThingSearchTools() -- the current thing manager's thing search tools
// Leaf: ?GetThingSearchTools@CThing@@IAEAAVCThingSearchTools@@XZ
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x004C7A50, 11 bytes: `mov ecx,[0x013B8A1C] ; jmp 0x0051E490` (this unused; tail call).
// Identity (two sources): Ego_r.pdb / FableWin.pdb ?GetThingSearchTools@CThing@@IAEAAVCThingSearchTools@@XZ, and Ego_r's donor order
//   PeekPlayer / GetPlayer / GetThingSearchTools / PeekThingSearchTools / PeekComponent (0x004C95B0..0x004C9610) is retail's
//   0x004C7A10 / 0x004C7A30 / 0x004C7A50 / 0x004C7A60 / 0x004C7A70 (landed PeekPlayer, GetPlayer, PeekComponent); the sole caller
//   CThingPlayerCreature::ResetThingReactions 0x006AC16F is a CThing-derived member. The catalog's "GetSearchTools" was a stem.
// Callees: ?GetThingSearchTools@CThingManager@@QAEAAVCThingSearchTools@@XZ 0x0051E490 (landed); the manager is the private static
//   ?PCurrentThingManager@CThing@@0PAVCThingManager@@A 0x013B8A1C through the inline GetPCurrentThingManager().
// Header: engine/CThing.h (FABLE_RETAIL_CTHING_MANAGERACCESS) and engine/CThingManager.h (FABLE_RETAIL_CTHINGMANAGER_ACCESSORS).
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_MANAGERACCESS
#define FABLE_RETAIL_CCLASSFACTORY
#define FABLE_RETAIL_CTHINGMANAGER
#define FABLE_RETAIL_CTHINGMANAGER_ACCESSORS
#include "engine/CThing.h"
#include "engine/CThingManager.h"
CThingSearchTools& CThing::GetThingSearchTools()
{
    return GetPCurrentThingManager()->GetThingSearchTools();
}