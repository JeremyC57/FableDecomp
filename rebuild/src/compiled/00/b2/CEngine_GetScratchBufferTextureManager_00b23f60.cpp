// CEngine::GetScratchBufferTextureManager() -- the resource manager's GPU scratch-buffer texture manager
// Leaf: ?GetScratchBufferTextureManager@CEngine@@UAEPAVCGPUScratchBufferTextureManager@@XZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B23F60, 11 bytes: `mov eax,[0x01436E98] ; add eax,3190h ; ret` (`this` unused).
// NF46 (round 7): identity Ego_r.pdb 0x009AE350 / FableWin.pdb 0x02CA2030 ?GetScratchBufferTextureManager@CEngine@@UAEPAVCGPUScratchBufferTextureManager@@XZ,
//   the CIEngine slot-73 override (engine/CIEngine.h; CEngine.h overlay opt-in _GETSCRATCHBUFFERTEXTUREMANAGER); was landed as the free
//   function CEngine_GetScratchBufferTextureManager(CEngine*) reading `char* g_engineSingleton + 0x3190`. The 0x01436E98 global is
//   NEngineInternal::GPResourceManager (Ego_r.pdb / FableWin.pdb ?GPResourceManager@NEngineInternal@@3PAVCEngineResourceManager@@A) and +0x3190
//   its GPUScratchBufferTextureManager (FableWin.pdb inline GetGPUScratchBufferTextureManager, engine/CEngineResourceManager.h).
#define FABLE_RETAIL_CENGINE
#define FABLE_RETAIL_CENGINE_GETSCRATCHBUFFERTEXTUREMANAGER
#define FABLE_RETAIL_CENGINERESOURCEMANAGER
#include "engine/CEngine.h"
#include "engine/CEngineResourceManager.h"

namespace NEngineInternal { extern CEngineResourceManager* GPResourceManager; }   // 0x01436E98

CGPUScratchBufferTextureManager* CEngine::GetScratchBufferTextureManager()
{
    return &NEngineInternal::GPResourceManager->GetGPUScratchBufferTextureManager();
}
