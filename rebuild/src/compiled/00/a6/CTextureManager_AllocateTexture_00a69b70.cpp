// CTextureManager::AllocateTexture(CManagedTexture& texture, unsigned long width, unsigned long height, long mip_levels,
// const CPixelFormat& format, CManagedTextureOwner* owner) -- pick the prealloc pool for the texture's byte size, take a managed
// texture data from it, create the D3D texture of the requested shape, record the owner and hand the data to the managed texture's
// counted pointer (adopted by Reset, then AddRef'd); a failed pool allocation or creation bumps that pool's failure count.
// Leaf: ?AllocateTexture@CTextureManager@@QAE_NAAVCManagedTexture@@KKJABVCPixelFormat@@PAVCManagedTextureOwner@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A69B70, 176 bytes, `ret 18h`.
// Identity: Ego_r.pdb / FableWin.pdb, the spelling above.
// Callees: CTextureManager::CalculateTextureSize 0x00A698F0 (static __fastcall), CTextureManager::GetPoolIndex 0x00A69730,
//   CPreallocTexturePool::Allocate 0x00A69830, CManagedTextureData::CreateTexture 0x00A697A0, the released data's virtual Delete
//   (CIVCountedPointeeBase slot 1); inline: CManagedTextureData::SetOwner, CIVCountedPointer<CManagedTextureData>::Reset / Release /
//   operator->, CIVCountedPointeeBase::AddRef.
// Layout: CTextureManager PreallocPools +0x4C (stride 0x54), FailedAllocations +0x58C; CManagedTexture Data +0x04;
//   CManagedTextureData Owner +0x30, IVRefCount +0x04.
// Shape: Reset is the adopting Ego_r form (Release, then store; chunk 850, FABLE_RETAIL_CIVCOUNTEDPOINTER_RESET_ADOPT), so the
//   caller AddRefs: `texture.Data->AddRef()` goes through operator->'s copy of the stored pointer (0x00A69BF6 `mov esi,ebx`, then
//   load / inc / store interleaved with the pops), where `texture_data->AddRef()` gives `inc [ebx+4]`. Retail expands Reset in place;
//   VC7.1's inline budget leaves it a call in this caller, so the overlay forces the same body inline
//   (FABLE_RETAIL_CIVCOUNTEDPOINTER_RESET_FORCEINLINE nested under _ADOPT). The texture size is a named local: with the call
//   nested in GetPoolIndex's argument the callee-saved registers come out permuted (this edi / pool ebx / data esi instead of
//   retail's esi / edi / ebx).
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_STATICBLOCKS_TEXTURE_HELPERS
#define FABLE_RETAIL_CIVCOUNTEDPOINTER
#define FABLE_RETAIL_CIVCOUNTEDPOINTER_RESET_ADOPT
#define FABLE_RETAIL_CIVCOUNTEDPOINTER_RESET_FORCEINLINE
#define FABLE_RETAIL_CTEXTUREMANAGER_ALLOCATETEXTURE
#include "engine/CPixelFormat.h"
#include "engine/CManagedTexture.h"
#include "engine/CManagedTextureData.h"
#include "engine/CTextureManager.h"

bool CTextureManager::AllocateTexture(CManagedTexture& texture, unsigned long width, unsigned long height, long mip_levels,
                                      const CPixelFormat& format, CManagedTextureOwner* owner)
{
    unsigned long size = CalculateTextureSize(width, height, true, format);
    long pool_index = GetPoolIndex(size);
    if (pool_index == -1)
        return false;
    CManagedTextureData* texture_data = PreallocPools[pool_index].Allocate();
    if (texture_data && texture_data->CreateTexture(width, height, mip_levels, format)) {
        texture_data->SetOwner(owner);
        texture.Data.Reset(texture_data);
        texture.Data->AddRef();
        return true;
    }
    ++FailedAllocations[pool_index];
    return false;
}