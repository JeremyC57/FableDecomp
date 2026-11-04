// CThing::PeekThingSearchTools() const -- the current thing manager's thing search tools (const view)
// Leaf: ?PeekThingSearchTools@CThing@@IBEABVCThingSearchTools@@XZ
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x004C7A60, 11 bytes: `mov ecx,[0x013B8A1C] ; jmp 0x0051E480` (this unused; tail call).
// Identity (two sources): Ego_r.pdb / FableWin.pdb ?PeekThingSearchTools@CThing@@IBEABVCThingSearchTools@@XZ at Ego_r 0x004C9600, right
//   after GetThingSearchTools 0x004C95F0, as retail 0x004C7A60 follows 0x004C7A50; it jumps to the const twin
//   CThingManager::PeekThingSearchTools 0x0051E480; callers are CThing-derived members (CThingCreatureBase::GetHeightZBelowAt
//   0x00666E84, CThingAICreature::WorldUpdate 0x00834778).
// Callees: ?PeekThingSearchTools@CThingManager@@QBEABVCThingSearchTools@@XZ 0x0051E480 (landed); the manager through the inline
//   PeekPCurrentThingManager() (?PeekPCurrentThingManager@CThing@@SIPBVCThingManager@@XZ).
// Header: engine/CThing.h (FABLE_RETAIL_CTHING_MANAGERACCESS) and engine/CThingManager.h (FABLE_RETAIL_CTHINGMANAGER_ACCESSORS).
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_MANAGERACCESS
#define FABLE_RETAIL_CCLASSFACTORY
#define FABLE_RETAIL_CTHINGMANAGER
#define FABLE_RETAIL_CTHINGMANAGER_ACCESSORS
#include "engine/CThing.h"
#include "engine/CThingManager.h"
const CThingSearchTools& CThing::PeekThingSearchTools() const
{
    return PeekPCurrentThingManager()->PeekThingSearchTools();
}