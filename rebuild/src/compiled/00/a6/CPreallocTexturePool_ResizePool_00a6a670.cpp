// CPreallocTexturePool::ResizePool(unsigned long).
// Leaf: ?ResizePool@CPreallocTexturePool@@QAE_NK@Z
// Flags:
// Inline-budget: the STLport fastcall helpers keep the same retail copy, fill and overflow call boundaries.
#define FABLE_RETAIL_TEXTUREPOOL_RESIZE_PASS30
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CRESOURCELIST_REMOVERESOURCE
#define FABLE_RETAIL_STATICBLOCKS_TEXTURE_HELPERS
#define FABLE_RETAIL_CTEXTUREMANAGER_ALLOCATETEXTURE
#define FABLE_RETAIL_TEXTURE_MIP_REDUCE_PASS29
#define FABLE_RETAIL_CTEXTURE
#define FABLE_RETAIL_TEXTURE_STREAM_LOAD_PASS30
#define FABLE_RETAIL_CCOUNTEDPOINTER_DELETEDATA_DECL_ONLY
#define FABLE_RETAIL_CIVCOUNTEDPOINTER
#define FABLE_RETAIL_CIVCOUNTEDPOINTER_ASSIGN
#define FABLE_RETAIL_CIVCOUNTEDPOINTER_RESET_ADOPT
#define FABLE_RETAIL_CIVCOUNTEDPOINTER_RELEASE_ISNULL
#define FABLE_RETAIL_CIVCOUNTEDPOINTEEBASE_RELEASE_ORIGINAL_SCOPES_PASS22
#define FABLE_RETAIL_CRENDERMANAGERCORE
#define FABLE_RETAIL_CRENDERMANAGERCORE_WAITWHILEGPUISBUSY
#define FABLE_RETAIL_CDISPLAYMANAGER
#define FABLE_RETAIL_CSYSTEMMANAGER
#define FABLE_RETAIL_SCOPED_PTR
#define FABLE_RETAIL_CARRAY
#define FABLE_RETAIL_STLP_VECTOR
#define FABLE_RETAIL_STLP_VECTOR_C
#define FABLE_RETAIL_STLP_COPY_NOINLINE

#define FABLE_RETAIL_CCOUNTEDPOINTER_RELEASEDATA
#define FABLE_RETAIL_STLP_FILL_NOINLINE
#define FABLE_RETAIL_STLP_UNINIT_COPY_FALSE_FASTCALL_PASS26
#define FABLE_RETAIL_STLP_DESTROY_AUX_FASTCALL
#define FABLE_RETAIL_STLP_RANDOM_ACCESS_COPY_BACKWARD
#define FABLE_RETAIL_STLP_COPY_BACKWARD_NOINLINE
#define FABLE_RETAIL_STLP_UNINITIALIZED_FILL_N_NOINLINE

#include "fable_compiler.h"
#include "fable_string.h"
#include "engine/CSystemManager.h"
#include "engine/Direct3D9Types.h"
#include "engine/CDisplayManager.h"
#include "engine/CRenderManager.h"
#include "engine/CManagedTextureOwner.h"
#include "engine/CPreallocTexturePool.h"

bool CPreallocTexturePool::ResizePool(unsigned long new_size)
{
    if (new_size == Count)
        return true;

    bool success = false;
    CRenderManager& render_manager = GFGetSystemManager()->GetDisplayManager()->GetRenderManager();
    render_manager.WaitWhileGPUIsBusy();
    for (long c_stage = 0; c_stage < 4; ++c_stage)
        render_manager.AttachTextureToStage(c_stage, 0);

    for (unsigned long c1 = 0; c1 < Count; ++c1)
        RemoveResource(*Textures[c1]);

    unsigned long alloc_size = GetTexturesAllocated();
    long dest_index = 0;
    for (unsigned long c1 = 0; c1 < Count; ++c1) {
        CManagedTextureData& managed_texture = GetTextureData(c1);
        if (managed_texture.IsAllocated() && new_size < alloc_size) {
            managed_texture.GetOwner()->ReleaseTexture();
            alloc_size = GetTexturesAllocated();
        }
        if (managed_texture.IsAllocated()) {
            if (static_cast<unsigned long>(dest_index) < new_size &&
                static_cast<unsigned long>(dest_index) != c1) {
                Textures[dest_index] = Textures[c1];
                Textures[c1].Reset(new CManagedTextureData());
            }
            ++dest_index;
        }
    }

    if (static_cast<unsigned long>(dest_index) <= new_size) {
        Count = new_size;
        Textures.resize(Count);
        for (unsigned long c1 = dest_index; c1 < Count; ++c1) {
            Textures[c1].Reset(new CManagedTextureData());
            Textures[c1]->Initialise(MaxSize, PoolIndex);
        }
        success = true;
    } else {
        Count = dest_index;
        Textures.resize(Count);
        success = true;
    }

    for (unsigned long c1 = 0; c1 < Count; ++c1)
        AddResource(*Textures[c1]);
    return success;
}