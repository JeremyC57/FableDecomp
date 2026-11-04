// CThing::CheckUID() const -- debug UID check; the release body only queries whether the manager is serialising game state
// Leaf: ?CheckUID@CThing@@IBEXXZ
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x004C7BC0, 11 bytes: `mov ecx,[0x013B8A1C] ; jmp 0x0051E2E0` (this unused; result discarded).
// Identity (two sources): Ego_r.pdb / FableWin.pdb ?CheckUID@CThing@@IBEXXZ (protected const, void) sits in Ego_r between
//   IsToBePlacedInMap 0x004C97D0 and IsForceNoSerialise 0x004C9860 (with the inline GetTypeName / IsLocal), exactly where retail puts
//   0x004C7BC0 (IsToBePlacedInMap 0x004C7B80, IsForceNoSerialise 0x004C7BD0); the sole caller, the UID verification loop
//   CThingManager 0x0051E630, calls it per thing and ignores eax before reading the thing's UniqueID (+0x18/+0x1C): a void call.
//   The catalog's "_Cons_val<..CParticleMorphs..>" was a propagated label. Descriptive body: the release build keeps only the
//   manager query (an assert-style test whose condition still calls out of line); the debug checks are not in retail.
// Callees: ?IsSerialisingGameState@CThingManager@@QBE_NXZ 0x0051E2E0 (landed under wrong spellings; NAME_FIXES.tsv).
// Header: engine/CThing.h (FABLE_RETAIL_CTHING_MANAGERACCESS) and engine/CThingManager.h (FABLE_RETAIL_CTHINGMANAGER_ACCESSORS).
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_MANAGERACCESS
#define FABLE_RETAIL_CCLASSFACTORY
#define FABLE_RETAIL_CTHINGMANAGER
#define FABLE_RETAIL_CTHINGMANAGER_ACCESSORS
#include "engine/CThing.h"
#include "engine/CThingManager.h"
void CThing::CheckUID() const
{
    PeekPCurrentThingManager()->IsSerialisingGameState();
}