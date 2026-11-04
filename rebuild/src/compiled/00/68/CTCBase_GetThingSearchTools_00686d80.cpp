// CTCBase::GetThingSearchTools() -- the current thing manager's thing search tools, for thing components
// Leaf: ?GetThingSearchTools@CTCBase@@IAEAAVCThingSearchTools@@XZ
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x00686D80, 11 bytes: `mov ecx,[0x013B8A1C] ; jmp 0x0051E490` (this unused; tail call).
// Identity (two sources, as engine/CTCBase.h records): Ego_r.pdb 0x006B2170 / FableWin.pdb 0x021915B0
//   ?GetThingSearchTools@CTCBase@@IAEAAVCThingSearchTools@@XZ; Ego_r's donor run GetWorld .. PeekWorldMap .. GetThingSearchTools /
//   PeekThingSearchTools matches retail 0x00686A70 .. 0x00686D20 .. 0x00686D80 / 0x00686D90, and its 18 callers are thing components
//   (CTCWife::GlobalUpdate, CTCPerceiveThing::UpdateHearing, CTCExplodingObject::FrameUpdate ...). The catalog's
//   "_Cons_val<..CParticleMorphs..>" was a propagated label.
// Callees: ?GetThingSearchTools@CThingManager@@QAEAAVCThingSearchTools@@XZ 0x0051E490 (landed); the manager through the inline
//   CThing::GetPCurrentThingManager() (private static 0x013B8A1C).
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
CThingSearchTools& CTCBase::GetThingSearchTools()
{
    return CThing::GetPCurrentThingManager()->GetThingSearchTools();
}