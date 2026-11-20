// CAIStateGroupBase::PeekThingSearchTools() const -- the world's thing search tools, reached through the brain's thing manager
// Leaf: ?PeekThingSearchTools@CAIStateGroupBase@@IBEABVCThingSearchTools@@XZ
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x008FD000, 13 bytes: `mov eax,[ecx+4] ; mov ecx,[eax+1Ch] ; mov edx,[ecx+24h] ; mov eax,[edx+20h] ; ret`
//   (*PBrain->ThingManager->World.PThingSearchTools).
// Identity (two sources): Ego_r.pdb 0x00847E50 + FableWin.pdb 0x0288D940 ?PeekThingSearchTools@CAIStateGroupBase@@IBEABVCThingSearchTools@@XZ
//   (protected const) and engine/CAIStateGroupBase.h _PEEK (chunk 756, the spelling the landed binders 0x009104D0 / 0x00969E70 / 0x00986210 ...
//   bind). Ego_r donor order PeekWorld 0x00847DF0, PeekDefinitionManager 0x00847E00, GetPWorldSeed 0x00847E20, GetDebugText 0x00847E30,
//   PeekThingSearchTools 0x00847E50, IsStopping 0x00847E60 = retail 0x008FCFA0 / 0x008FCFB0 / 0x008FCFD0 / 0x008FCFE0 / 0x008FD000 / 0x008FD010.
//   Every landed binder of 0x008FD000 binds this spelling after the NF137 (nf137) and NF141 hand-backs (55 REFS rows).
// NF141 (2026-11-20, q4994 / q5140): was the free stand-in ?CAIStateGroupBase_PeekThingSearchTools@@YIPAVCThingSearchTools@@PBUA@@@Z over
//   address-only local structs (A / B / C / D); now the out-of-line member on the shared headers. Body: the inline CThingManager::PeekWorld
//   (FableWin ?PeekWorld@CThingManager@@QBEABVCWorld@@XZ, as the landed 0x008FCFA0) and CWorld::PeekThingSearchTools (FableWin
//   ?PeekThingSearchTools@CWorld@@QBEABVCThingSearchTools@@XZ, inline: [world+0x20]).
// Header: engine/CAIStateGroupBase.h (FABLE_RETAIL_CAISTATEGROUPBASE, _MEMBERS, _PEEK), engine/CAIBrain.h (ThingManager +0x1c),
//   engine/CThingManager.h (_PEEKWORLD: World +0x24), engine/CWorld.h (PThingSearchTools +0x20).
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_CCLASSFACTORY
#define FABLE_RETAIL_CTHINGMANAGER
#define FABLE_RETAIL_CTHINGMANAGER_PEEKWORLD
#define FABLE_RETAIL_CAISTATEGROUPBASE
#define FABLE_RETAIL_CAISTATEGROUPBASE_MEMBERS
#define FABLE_RETAIL_CAISTATEGROUPBASE_PEEK
#define FABLE_RETAIL_CWORLD
#include "fable_string.h"
#include "engine/CAIStateGroupBase.h"
#include "engine/CAIBrain.h"
#include "engine/CThingManager.h"
#include "engine/CWorld.h"

const CThingSearchTools& CAIStateGroupBase::PeekThingSearchTools() const
{
    return PBrain->ThingManager->PeekWorld().PeekThingSearchTools();
}
