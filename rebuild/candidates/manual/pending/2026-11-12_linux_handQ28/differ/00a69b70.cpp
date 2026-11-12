// CTextureManager::AllocateTexture(CManagedTexture& texture, unsigned long width, unsigned long height, long mip_levels,
// Grade: DIFFER(170v176) -- callee-saved colouring (retail this esi / pool index edi / texture ebx, ours edi / ebx / esi) and the add-ref `inc [esi+4]` where retail copies the texture to esi and does load/inc/store; with Reset not expanded the colouring is retail's, so retail's expansion keeps the add-ref on a separate copy; tried nested ifs, pool / data reference locals, CastToBase in Reset, RELEASE_INNER
// const CPixelFormat& format, CManagedTextureOwner* owner) -- pick the prealloc pool for the texture's byte size, take a managed
// texture from it, create the D3D texture of the requested shape, record the owner and hand the texture back through the managed
// texture's counted pointer; a failed pool allocation or creation bumps that pool's failure count.
// Leaf: ?AllocateTexture@CTextureManager@@QAE_NAAVCManagedTexture@@KKJABVCPixelFormat@@PAVCManagedTextureOwner@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A69B70, 176 bytes, `ret 18h`.
// Identity: Ego_r.pdb / FableWin.pdb, the spelling above.
// Callees: CTextureManager::CalculateTextureSize 0x00A698F0 (static __fastcall), CTextureManager::GetPoolIndex 0x00A69730,
//   CPreallocTexturePool::Allocate 0x00A69830, CManagedTextureData::CreateTexture 0x00A697A0, the released texture's virtual Delete
//   (CIVCountedPointeeBase slot 1, through the inline CIVCountedPointer<CManagedTextureData>::Reset); SetOwner inline.
// Layout: CTextureManager PreallocPools +0x4C (stride 0x54), FailedAllocations +0x58C; CManagedTexture Data +0x04;
//   CManagedTextureData Owner +0x30, IVRefCount +0x04.
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_STATICBLOCKS_TEXTURE_HELPERS
#define FABLE_RETAIL_CIVCOUNTEDPOINTER
#define FABLE_RETAIL_CIVCOUNTEDPOINTER_RELEASE_MEMBER
#define FABLE_RETAIL_CIVCOUNTEDPOINTER_RESET_FORCEINLINE
#define FABLE_RETAIL_CIVCOUNTEDPOINTER_CASTTOBASE
#define FABLE_RETAIL_CIVCOUNTEDPOINTEEBASE_RELEASE_DELETES
#define FABLE_RETAIL_CTEXTUREMANAGER_ALLOCATETEXTURE
#include "engine/CPixelFormat.h"
#include "engine/CManagedTexture.h"
#include "engine/CManagedTextureData.h"
#include "engine/CTextureManager.h"

bool CTextureManager::AllocateTexture(CManagedTexture& texture, unsigned long width, unsigned long height, long mip_levels,
                                      const CPixelFormat& format, CManagedTextureOwner* owner)
{
    long pool_index = GetPoolIndex(CalculateTextureSize(width, height, true, format));
    if (pool_index == -1)
        return false;
    CManagedTextureData* texture_data = PreallocPools[pool_index].Allocate();
    if (texture_data && texture_data->CreateTexture(width, height, mip_levels, format)) {
        texture_data->SetOwner(owner);
        texture.Data.Reset(texture_data);
        return true;
    }
    ++FailedAllocations[pool_index];
    return false;
}
