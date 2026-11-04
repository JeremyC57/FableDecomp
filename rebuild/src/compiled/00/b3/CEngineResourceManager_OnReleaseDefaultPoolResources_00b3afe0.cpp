// CEngineResourceManager::OnReleaseDefaultPoolResources() -- device lost: shut the GPU scratch-buffer texture manager down
// Leaf: ?OnReleaseDefaultPoolResources@CEngineResourceManager@@EAEXXZ
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x00B3AFE0, 11 bytes: `add ecx,3190h ; jmp 0x00F37170` (the bool result is discarded: tail call).
// Identity (two sources): (1) vtable 0x012A1B3C (RTTI CEngineResourceManager, primary CEngineComponent table) slot 11 is
//   CEngineComponent::OnReleaseDefaultPoolResources (engine/CEngineComponent.h order), and Ego_r.pdb has the private override
//   ?OnReleaseDefaultPoolResources@CEngineResourceManager@@EAEXXZ; (2) the target 0x00F37170 is the landed
//   ?BeginShutdown@CGPUScratchBufferTextureManager@@QAE_NXZ on the Ego_r member GPUScratchBufferTextureManager (retail +0x3190, the
//   accessor reads in 0x00B71880 / 0x00B6D630), and slot 12 (OnRestoreDefaultPoolResources 0x00B3AFF0, landed) forwards the same
//   member to 0x00F36D30 (the matching EndShutdown). The catalog "CEngineResourceManager::CEngineResourceManager::..." doubled the class.
// Callees: ?BeginShutdown@CGPUScratchBufferTextureManager@@QAE_NXZ 0x00F37170.
// Header: engine/CEngineResourceManager.h (FABLE_RETAIL_CENGINERESOURCEMANAGER + _ONRELEASE new),
//   engine/CGPUScratchBufferTextureManager.h (new).
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CENGINERESOURCEMANAGER
#define FABLE_RETAIL_CENGINERESOURCEMANAGER_ONRELEASE
#include "fable_string.h"
#include "engine/CEngineResourceManager.h"
#include "engine/CGPUScratchBufferTextureManager.h"
void CEngineResourceManager::OnReleaseDefaultPoolResources()
{
    GetGPUScratchBufferTextureManager().BeginShutdown();
}