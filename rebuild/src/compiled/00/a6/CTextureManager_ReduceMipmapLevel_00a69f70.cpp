// 00a69f70 CTextureManager::ReduceMipmapLevel
// Leaf: ?ReduceMipmapLevel@CTextureManager@@QAE_NAAVCGraphicFrame@@AAVCManagedTexture@@K@Z
// Flags: /O2 /Oy
#define FABLE_RETAIL_ASSERT_DEAD_ARM
#include "fable_assert.h"
#define FABLE_RETAIL_CIVCOUNTEDPOINTEEBASE_RELEASE_COUNT
#define FABLE_RETAIL_CIVCOUNTEDPOINTER_RELEASE_ISNULL
#define FABLE_RETAIL_TEXTURE_MIP_REDUCE_PASS29
#define FABLE_RETAIL_CTEXTUREMANAGER_CLASS
#define FABLE_RETAIL_CTEXTUREMANAGER_ALLOCATETEXTURE
#define FABLE_RETAIL_STATICBLOCKS_TEXTURE_HELPERS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CIVCOUNTEDPOINTER
#define FABLE_RETAIL_CIVCOUNTEDPOINTER_GET
#define FABLE_RETAIL_CIVCOUNTEDPOINTER_ASSIGN
#define FABLE_RETAIL_CIVCOUNTEDPOINTER_RESET_ADOPT
#define FABLE_RETAIL_CMANAGEDTEXTUREDATA_RELEASETEXTURE
#define FABLE_RETAIL_CMANAGEDTEXTURE_GETTEXTURE
#define FABLE_RETAIL_CGRAPHICFRAME
#define FABLE_RETAIL_CGRAPHICFRAME_GETTEXTURE
#define FABLE_RETAIL_CGRAPHICFRAMEDATA_TEXTUREQUADUVS
#define FABLE_RETAIL_CTEXTURE
#define FABLE_RETAIL_CTEXTURE_SIZED
#define FABLE_RETAIL_CTEXTURE_GETNOLEVELS
#define FABLE_RETAIL_CTEXTURE_FORMATQUERIES
#define FABLE_RETAIL_CSURFACE
#define FABLE_RETAIL_CSURFACE_COPYFROMMEMORY
#define FABLE_RETAIL_CSURFACE_COPYFROMSURFACE
#define FABLE_RETAIL_C2DBOXI
#define FABLE_RETAIL_C2DBOXI_CTORS
#include "engine/C2DBoxI.h"
#include "engine/CTexture.h"
#include "engine/CSurface.h"
#include "engine/CManagedTexture.h"

// Inline-budget: the member initializer expands in the caller. The closing brace selects an out-of-line destructor in the native deleting destructor.
inline CManagedTexture::CManagedTexture() {
#ifdef _MSC_VER
#pragma inline_depth(0)
#endif
}
#ifdef _MSC_VER
#pragma inline_depth()
#endif

inline bool CManagedTexture::Release()
{
    if (Data.IsNull()) return true;
    bool was_released = false;
    if (Data.Get() && Data.GetRefCount() == 2) {
        FABLE_RETAIL_ASSERT(Data->IsAllocated());
        Data->ReleaseTexture();
        was_released = true;
    }
    Data.Reset(0);
    return was_released;
}
inline CManagedTexture::~CManagedTexture()
{
    if (Data.GetRefCount() == 2)
        Release();
}
inline const CManagedTexture& CManagedTexture::operator=(const CManagedTexture& src)
{
    if (Data.Get() != src.Data.Get()) {
        Release();
        Data = src.Data;
    }
    return *this;
}

#include "engine/CGraphicFrame.h"
#include "engine/CTextureManager.h"
bool CTextureManager::ReduceMipmapLevel(CGraphicFrame& frame, CManagedTexture& managed_texture, unsigned long skip_levels)
{
    CTexture& src_texture = *frame.GetTexture();
    unsigned long old_levels = src_texture.GetNoLevels();
    unsigned long new_levels = old_levels - skip_levels;
    C2DExtentsI dimensions(frame.GetWidth(), frame.GetHeight());
    for (unsigned long c_skip=0; c_skip<skip_levels; ++c_skip) {
        dimensions.X /= 2;
        dimensions.Y /= 2;
    }
    managed_texture.Data->SetLocked(true);
    CManagedTexture new_managed_texture;
    if (!AllocateTexture(new_managed_texture, dimensions.X, dimensions.Y, new_levels, src_texture.GetPixelFormat(), managed_texture.Data->GetOwner())) {
        managed_texture.Data->SetLocked(false);
        return false;
    }
    CCountedPointer<CTexture> dest_texture = new_managed_texture.GetTexture();
    C2DBoxI surface_area(0,0,dimensions.X,dimensions.Y);
    for (unsigned long c_level=0; c_level<new_levels; ++c_level) {
        CSurface dest_surface = dest_texture->GetSurfaceLevel(c_level);
        CSurface src_surface = src_texture.GetSurfaceLevel(c_level+skip_levels);
        dest_surface.CopyFromSurface(&surface_area,src_surface,&surface_area,1);
        surface_area.BRX /= 2;
        surface_area.BRY /= 2;
    }
    CGraphicFrame old_frame(frame);
    frame.UnInitialise();
    frame.InitialiseAsSurface(dest_texture,dimensions.X,dimensions.Y,old_frame.IsAlpha(),old_frame.IsBooleanAlpha());
    managed_texture.Data->SetOwner(0);
    managed_texture.Data->SetLocked(false);
    managed_texture = new_managed_texture;
    return true;
}