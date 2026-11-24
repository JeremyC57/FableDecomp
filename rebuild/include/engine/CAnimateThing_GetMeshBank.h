#ifndef FABLE_ENGINE_CANIMATETHING_GETMESHBANK_H
#define FABLE_ENGINE_CANIMATETHING_GETMESHBANK_H
// B24C8/B24C9 (row 0x0070D940): body of the FABLE_RETAIL_CANIMATETHING_GETMESHBANK member of engine/CAnimateThing.h.
// ?GetMeshBank@CAnimateThing@@QBEABVCMeshDataBank@@XZ: Ego_r.pdb release animate_thing.obj 12 bytes = retail's out-of-line copy 0x0070C0D0
//   (`mov eax,[PMainGameComponent]; mov ecx,[eax+24h]; mov eax,[ecx+3Ch]; ret`), the main game component's world's main mesh bank.
#if !defined(FABLE_RETAIL_CANIMATETHING_GETMESHBANK) || !defined(FABLE_RETAIL_CWORLD_MESH_BANK) || !defined(FABLE_RETAIL_CMAINGAMECOMPONENT_GETWORLD)
#error engine/CAnimateThing_GetMeshBank.h needs FABLE_RETAIL_CANIMATETHING_GETMESHBANK, _CWORLD_MESH_BANK, _CMAINGAMECOMPONENT_GETWORLD
#endif
#include "engine/CAnimateThing.h"
#include "engine/CMeshDataBank.h"
#include "engine/CWorld.h"
#include "engine/CMainGameComponent.h"
inline const CMeshDataBank& CAnimateThing::GetMeshBank() const
{
    return *CMainGameComponent::Get()->GetWorld().GetMeshBank();
}
#endif
