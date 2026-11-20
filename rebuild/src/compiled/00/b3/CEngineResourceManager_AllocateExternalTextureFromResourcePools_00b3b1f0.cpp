// CEngineResourceManager::AllocateExternalTextureFromResourcePools(unsigned long id, const C2DExtentsI& size, long levels, const CPixelFormat& format) -- allocates a zeroed CTexture with new and initialises it (no usage flags, managed pool, no dynamic/render-target flags); `this` and `id` are unused on PC
// Leaf: ?AllocateExternalTextureFromResourcePools@CEngineResourceManager@@QAEPAVCTexture@@KABVC2DExtentsI@@JABVCPixelFormat@@@Z
// Retail: Fable.exe @ 0x00B3B1F0, 70 bytes (+ int3 padding).
// Name (NF152, 2026-11-20, q4595): Ego_r.pdb 0x009D9860 / FableWin.pdb 0x02DD6090
//   ?AllocateExternalTextureFromResourcePools@CEngineResourceManager@@QAEPAVCTexture@@KABVC2DExtentsI@@JABVCPixelFormat@@@Z, the same four
//   parameters as CEngine::AllocateTextureFromEngineResourcePools, whose body 0x00B23F40 (`mov ecx,[0x01436E98] ; jmp 0x00B3B1F0`) already
//   binds this spelling; `ret 10h`. Was the made-up CEngineResourceManager::AllocateTexture on a local CTexture model.
// Callees: operator new @ 0x00BFEA1A, CTexture::Initialise(const C2DExtentsI&, long, const CPixelFormat&, ulong, ESurfacePool, bool, bool)
//   @ 0x009FA280.
// Shape: `new CTexture` with the header's inline default constructor (null test, `mov esi,eax` / `xor esi,esi`), then the seven Initialise
//   arguments pushed right to left with the three references passed as their addresses.
#include "fable_compiler.h"
#define FABLE_RETAIL_CENGINECOMPONENT
#define FABLE_RETAIL_CENGINERESOURCEMANAGER
#define FABLE_RETAIL_CENGINERESOURCEMANAGER_ALLOCATEEXTERNALTEXTUREFROMRESOURCEPOOLS
#define FABLE_RETAIL_CTEXTURE
#include "engine/CTexture.h"
#include "engine/CEngineResourceManager.h"

CTexture* CEngineResourceManager::AllocateExternalTextureFromResourcePools(unsigned long id, const C2DExtentsI& size, long levels,
                                                                          const CPixelFormat& format)
{
    CTexture* texture = new CTexture;
    texture->Initialise(size, levels, format, 0, SURFACE_POOL_MANAGED, false, false);
    return texture;
}
