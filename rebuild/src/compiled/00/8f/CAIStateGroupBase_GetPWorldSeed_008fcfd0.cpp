// CAIStateGroupBase::GetPWorldSeed() const -- the world random seed, inside the brain's main game component
// Leaf: ?GetPWorldSeed@CAIStateGroupBase@@IBEPAKXZ
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x008FCFD0, 12 bytes: `mov eax,[ecx+4] ; mov eax,[eax+14h] ; add eax,1613Ch ; ret`
//   (&PBrain->Component->WorldSeed).
// Identity (two sources): Ego_r.pdb 0x00847E20 + FableWin.pdb 0x0288DA10 ?GetPWorldSeed@CAIStateGroupBase@@IBEPAKXZ (protected const; Ego_r donor
//   order PeekDefinitionManager 0x00847E00 then GetPWorldSeed 0x00847E20 = retail 0x008FCFB0 / 0x008FCFD0, the landed 0x008FCFB0 form);
//   engine/CAIStateGroupBase.h declares it. Every landed caller binds this spelling after the nf127 / NF131 hand-backs (99 REFS rows).
// NF131 (2026-11-20, q5141): was the free stand-in ?CAIStateGroupBase_GetPWorldSeed@@YIPAKPBVCAIStateGroupBase@@@Z over a local CAIBrain model;
//   now the out-of-line member on the shared headers. Body: the inline CMainGameComponent::GetWorldSeed (Ego_r.pdb ?GetWorldSeed@CMainGameComponent@@QBEPAKXZ).
// Header: engine/CAIStateGroupBase.h (FABLE_RETAIL_CAISTATEGROUPBASE, _MEMBERS), engine/CAIBrain.h (Component +0x14), engine/CMainGameComponent.h
//   (FABLE_RETAIL_CMAINGAMECOMPONENT, _SAMPLEBANK_WORLDSEED: WorldSeed +0x1613C).
#define FABLE_RETAIL_CAISTATEGROUPBASE
#define FABLE_RETAIL_CAISTATEGROUPBASE_MEMBERS
#define FABLE_RETAIL_CMAINGAMECOMPONENT
#define FABLE_RETAIL_CMAINGAMECOMPONENT_SAMPLEBANK_WORLDSEED
#include "engine/CAIStateGroupBase.h"
#include "engine/CAIBrain.h"
#include "engine/CMainGameComponent.h"
unsigned long* CAIStateGroupBase::GetPWorldSeed() const
{
    return PBrain->Component->GetWorldSeed();
}
