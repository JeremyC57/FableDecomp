// CEngineResourceManager::OnRestoreDefaultPoolResources() -- device restored: bring the GPU scratch-buffer texture manager back up
// Leaf: ?OnRestoreDefaultPoolResources@CEngineResourceManager@@EAE_NXZ
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x00B3AFF0, 11 bytes: `add ecx,3190h ; jmp 0x00F36D30` (tail call, the bool result passed through).
// Naming (NF172, 2026-11-20, q5133): ported off the generated CBaseIntelligentPointer / Method_00f36d30 stand-in onto the shared headers,
//   as its slot-11 sibling 0x00B3AFE0 (OnReleaseDefaultPoolResources -> BeginShutdown 0x00F37170). Identity (two sources): vtable
//   0x012A1B3C slot 12 is CEngineComponent::OnRestoreDefaultPoolResources (engine/CEngineComponent.h, virtual bool), and Ego_r.pdb
//   0x009D9640 / FableWin.pdb 0x02DD2A20 have the private override ?OnRestoreDefaultPoolResources@CEngineResourceManager@@EAE_NXZ
//   (was the stand-in ...@@UAEPAVCBaseObject@@XZ). Callee: Ego_r.pdb 0x00B3C1D0 / FableWin.pdb 0x0316CC20
//   ?EndShutdown@CGPUScratchBufferTextureManager@@QAE_NXZ on the member GPUScratchBufferTextureManager (+0x3190); the 0x00F36D30 body
//   (parked, read from Fable.exe) is bool with no arguments and re-allocates the scratch textures (operator new 0x00BFEA1A + CTexture::
//   Initialise 0x009FA280), the inverse of BeginShutdown's frees. NF172 opt-ins FABLE_RETAIL_CENGINERESOURCEMANAGER_ONRESTORE and
//   FABLE_RETAIL_CGPUSCRATCHBUFFERTEXTUREMANAGER_ENDSHUTDOWN.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CENGINERESOURCEMANAGER
#define FABLE_RETAIL_CENGINERESOURCEMANAGER_ONRESTORE
#define FABLE_RETAIL_CGPUSCRATCHBUFFERTEXTUREMANAGER_ENDSHUTDOWN
#include "fable_string.h"
#include "engine/CEngineResourceManager.h"
#include "engine/CGPUScratchBufferTextureManager.h"
bool CEngineResourceManager::OnRestoreDefaultPoolResources()
{
    return GetGPUScratchBufferTextureManager().EndShutdown();
}
