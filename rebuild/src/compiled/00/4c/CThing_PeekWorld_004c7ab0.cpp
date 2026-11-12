// CThing::PeekWorld() const -- the current world, read from the current thing manager (PCurrentThingManager->World, +0x24)
// Leaf: ?PeekWorld@CThing@@IBEABVCWorld@@XZ
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x004C7AB0, 9 bytes: `mov eax,[0x013B8A1C] ; mov eax,[eax+24h] ; ret` (same bytes as its
//   non-const twin GetWorld 0x004C7AA0).
// Name-fix 2026-11-12 (was the stand-in free function GetDisplayEngine, catalog _global::GetGlobal_013b8a1c_deref0x24_004c7ab0):
//   Ego_r.pdb 004c9650 ?PeekWorld@CThing@@IBEABVCWorld@@XZ (protected const), directly after ?GetWorld@CThing@@IAEAAVCWorld@@XZ
//   004c9640, the donor order of retail 0x004C7AA0 / 0x004C7AB0; FableWin.exe.signatures 01cf6400; the landed
//   CCreatureActionBase::PeekWorld 0x00692950 tail-jumps here. Declared in engine/CThing.h; the thing manager's World reference
//   +0x24 through engine/CThingManager.h (FABLE_RETAIL_CTHINGMANAGER_PEEKWORLD, FableWin.pdb ?PeekWorld@CThingManager@@QBEABVCWorld@@XZ,
//   inline in retail).
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_MANAGERACCESS
#define FABLE_RETAIL_CCLASSFACTORY
#define FABLE_RETAIL_CTHINGMANAGER
#define FABLE_RETAIL_CTHINGMANAGER_PEEKWORLD
#include "engine/CThing.h"
#include "engine/CThingManager.h"

const CWorld& CThing::PeekWorld() const
{
    return PeekPCurrentThingManager()->PeekWorld();
}
