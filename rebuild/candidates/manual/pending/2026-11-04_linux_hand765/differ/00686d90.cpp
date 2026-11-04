// CTCBase::PeekThingSearchTools() const -- the current thing manager's thing search tools (const view), for thing components
// Grade: DIFFER(ours v retail) -- none in the bytes (graded RELOCATION_MATCH); parked only as a duplicate symbol: the landed rebuild/src/compiled/00/6a/CTCBase_PeekThingSearchTools_006afd40.cpp already claims ?PeekThingSearchTools@CTCBase@@IBEABVCThingSearchTools@@XZ for 0x006AFD40, which is really ?GetThingSearchTools@CTCPhysicsBase@@IBEABVCThingSearchTools@@XZ (NAME_FIXES.tsv); next idea: land after that respelling
// Leaf: ?PeekThingSearchTools@CTCBase@@IBEABVCThingSearchTools@@XZ
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x00686D90, 11 bytes: `mov ecx,[0x013B8A1C] ; jmp 0x0051E480` (this unused; tail call).
// Identity (two sources, as engine/CTCBase.h records): Ego_r.pdb 0x006B2180 / FableWin.pdb 0x021915D0
//   ?PeekThingSearchTools@CTCBase@@IBEABVCThingSearchTools@@XZ, right after GetThingSearchTools in both PDBs as retail 0x00686D90 follows
//   0x00686D80; it jumps to the const twin CThingManager::PeekThingSearchTools 0x0051E480; its 62 callers are thing components.
// Callees: ?PeekThingSearchTools@CThingManager@@QBEABVCThingSearchTools@@XZ 0x0051E480 (landed); the manager through the inline
//   CThing::PeekPCurrentThingManager().
// Header: engine/CTCBase.h (FABLE_RETAIL_CTCBASE), engine/CThing.h (FABLE_RETAIL_CTHING_MANAGERACCESS), engine/CThingManager.h
//   (FABLE_RETAIL_CTHINGMANAGER_ACCESSORS).
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTCBASE_SERIALISEVERSION
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_MANAGERACCESS
#define FABLE_RETAIL_CCLASSFACTORY
#define FABLE_RETAIL_CTHINGMANAGER
#define FABLE_RETAIL_CTHINGMANAGER_ACCESSORS
#include "engine/CTCBase.h"
#include "engine/CThing.h"
#include "engine/CThingManager.h"
const CThingSearchTools& CTCBase::PeekThingSearchTools() const
{
    return CThing::PeekPCurrentThingManager()->PeekThingSearchTools();
}
