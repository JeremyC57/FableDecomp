// CTCBase::GetSerialiseVersion() const -- the thing manager's serialise version number, for thing components
// Leaf: ?GetSerialiseVersion@CTCBase@@IBEJXZ
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x00686A90, 11 bytes: `mov ecx,[0x013B8A1C] ; jmp 0x0051E2D0` (this unused; tail call).
// Identity (two sources): Ego_r.pdb / FableWin.pdb ?GetSerialiseVersion@CTCBase@@IBEJXZ at Ego_r 0x006B1E80, right after PeekWorld
//   0x006B1E70, as retail 0x00686A90 follows the landed CTCBase::PeekWorld 0x00686A80; the jump target 0x0051E2D0 is
//   CThingManager::GetSerialiseVersion (reads +0xD8 SerialiseVersionNumber); the sole caller is CTCPhysicsBase::OnSerialise 0x006B09F8.
//   The catalog's "_Cons_val<..CParticleMorphs..>" was a propagated label.
// Callees: ?GetSerialiseVersion@CThingManager@@QBEJXZ 0x0051E2D0 (landed under a wrong spelling; NAME_FIXES.tsv).
// Header: engine/CTCBase.h (FABLE_RETAIL_CTCBASE_SERIALISEVERSION), engine/CThing.h, engine/CThingManager.h.
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
long CTCBase::GetSerialiseVersion() const
{
    return CThing::PeekPCurrentThingManager()->GetSerialiseVersion();
}