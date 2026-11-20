// CEngineResourceManager::FreeExternalTextureFromResourcePools(CTexture* texture) -- deletes a texture made by AllocateExternalTextureFromResourcePools (the inline destructor runs CTexture::Uninitialise); `this` is unused on PC
// Leaf: ?FreeExternalTextureFromResourcePools@CEngineResourceManager@@QAEXPAVCTexture@@@Z
// Retail: Fable.exe @ 0x00B3B5B0, 29 bytes (+ int3 padding).
// Name (NF152, 2026-11-20, q4596): Ego_r.pdb 0x009D9C10 / FableWin.pdb 0x02DD6150
//   ?FreeExternalTextureFromResourcePools@CEngineResourceManager@@QAEXPAVCTexture@@@Z, the target of CEngine::FreeTextureFromEngineResourcePools
//   0x00B23F50 (`mov ecx,[0x01436E98] ; jmp 0x00B3B5B0`, already binding this spelling); `ret 4`. Was the made-up
//   CEngineResourceManager::FreeTexture on a local CTexture model.
// Callees: CTexture::Uninitialise @ 0x009F9F70 (through the header's inline ~CTexture), operator delete @ 0x00BFE9BC.
// Shape: `delete texture` of a non-polymorphic class whose destructor is inline: null test, Uninitialise, operator delete.
#include "fable_compiler.h"
#define FABLE_RETAIL_CENGINECOMPONENT
#define FABLE_RETAIL_CENGINERESOURCEMANAGER
#define FABLE_RETAIL_CENGINERESOURCEMANAGER_FREEEXTERNALTEXTUREFROMRESOURCEPOOLS
#define FABLE_RETAIL_CTEXTURE
#include "engine/CTexture.h"
#include "engine/CEngineResourceManager.h"

void CEngineResourceManager::FreeExternalTextureFromResourcePools(CTexture* texture)
{
    delete texture;
}
