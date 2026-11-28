// CEngineLandscapeRenderer::LoadForegroundTextures @ 0x00B68070
// Leaf: ?LoadForegroundTextures@CEngineLandscapeRenderer@@AAE_N_N@Z
// Flags: /Gr /d2inlT51
// Inline-budget: T51 retains 005E97E0 while 00464931 and 0046496F use their size-optimized STLport bodies.
// The native graphics accessors and the 0041C090 nonvirtual delete expand under the shared header controls.
// Retail-asm-helper: GFFloatToLongFloor
// Retail-asm: 0x00B681BB and 0x00B681F8 fistp dword with the original x87 tie correction.
#define FABLE_RETAIL_CENGINELANDSCAPERENDERER
#define FABLE_RETAIL_CENGINELANDSCAPERENDERER_LOADFOREGROUND_PASS30
#define FABLE_RETAIL_CENGINELANDSCAPERENDERER_GETSETTINGS
#define FABLE_RETAIL_CENGINELANDSCAPERENDERER_GPLANDSCAPERENDERER
#define FABLE_RETAIL_CENGINECAMERA_POSITION
#define FABLE_RETAIL_CENGINECAMERA_ZSPRITE_ACCESSORS
#define FABLE_RETAIL_RADIAL_WORLD_TRACK_PASS29
#define FABLE_RETAIL_CENGINECAMERA_PEEKWINDOWHEIGHT_PASS30
#define FABLE_RETAIL_CENGINERESOURCEMANAGER
#define FABLE_RETAIL_CENGINERESOURCEMANAGER_TERRAIN_ACCESSORS_PASS30
#define FABLE_RETAIL_CGRAPHICINFOFRAME_ISLOADING_PASS30
#define FABLE_RETAIL_STLP_VECTOR
#define FABLE_RETAIL_LANDSCAPE_INLINE_COPIES_PASS30
#define FABLE_RETAIL_STLP_VECTOR_ULONG_ERASE_COPY_PASS27
#define FABLE_RETAIL_STLP_VECTOR_ORIGINAL_RESIZE_INSERT_CHAIN_PASS22
#define FABLE_RETAIL_CIVCOUNTEDPOINTER_RESET_FORCEINLINE
#define FABLE_RETAIL_STLP_VECTOR_FILL_INSERT_DECL_ONLY
#define FABLE_RETAIL_MESH_MIPMAP_PASS30
#define FABLE_RETAIL_CARRAY
#define FABLE_RETAIL_CIVCOUNTEDPOINTER_RELEASE_ISNULL
#define FABLE_RETAIL_CIVCOUNTEDPOINTEEBASE_RELEASE_COUNT
#define FABLE_RETAIL_CGRAPHICFRAME_RELEASEDATA
#define FABLE_RETAIL_CGRAPHICINFOFRAME_TOUCHFRAME
#define FABLE_RETAIL_CGRAPHICINFOFRAME_GETBANK_DIRECT
#define FABLE_RETAIL_CRESOURCEBANK_CURRENTFRAME_DIRECT
#define FABLE_RETAIL_CFRAMEDATA_ISLOADING
#define FABLE_RETAIL_CFRAMEDATA_ISLOADING_ISNULL
#define FABLE_RETAIL_CIVCOUNTEDPOINTER_GET
#define FABLE_RETAIL_CMANAGEDTEXTURE_TOUCHTEXTURE_GET
#define FABLE_RETAIL_C2DVECTOR
#define FABLE_RETAIL_C4DVECTOR
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_STATICBLOCKS_TEXTURE_HELPERS
#define FABLE_RETAIL_CGRAPHICINFO_SPRITE
#define FABLE_RETAIL_CGRAPHICINFO_PASS22_SPRITE
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CIVCOUNTEDPOINTER
#define FABLE_RETAIL_CIVCOUNTEDPOINTER_RESET_ADOPT
#define FABLE_RETAIL_CMOVABLERESOURCE
#define FABLE_RETAIL_CRESOURCE_TOUCHRESOURCE
#define FABLE_RETAIL_CGRAPHICFRAME
#define FABLE_RETAIL_CGRAPHICFRAME_CTOR
#define FABLE_RETAIL_CGRAPHICFRAME_GETTEXTURE
#define FABLE_RETAIL_CGRAPHICFRAME_DTOR_FORCEINLINE
#define FABLE_RETAIL_CGRAPHICINFOFRAME_GETFRAME
#define FABLE_RETAIL_CGRAPHICINFOFRAME_ORIGINAL_GETFRAME_POINTER_GUARD_PASS22
#define FABLE_RETAIL_CGRAPHICINFOFRAME_ORIGINAL_FORCED_SUCCESS_SCOPE_PASS22
#define FABLE_RETAIL_CGRAPHICINFOFRAME_BEGINLOADING
#define FABLE_RETAIL_CGRAPHICDATABANK_PEEKGRAPHIC
#define FABLE_RETAIL_CGRAPHICDATABANK_PEEKGRAPHICINFOFRAME
#define FABLE_RETAIL_DEF_CLASS_BASE

#include <math.h>
#include "engine/CEngineLandscapeRenderer.h"
#include "engine/CEngineCamera.h"
#include "engine/CEngineResourceManager.h"
#include "engine/GFFloatToLongFloor.h"
#include "engine/GFGetTopBitInByte.h"
#include "fable_assert.h"

namespace NEngineInternal {
extern CEngineCamera* GPCamera;
extern CEngineResourceManager* GPResourceManager;
}

bool CEngineLandscapeRenderer::LoadForegroundTextures(bool loading_new_scene)
{
    using namespace NEngineInternal;
    float max_pixel_to_texel_ratio = GPLandscapeRenderer->GetSettings().ForegroundMaxPixelsPerTexel;
    float base_texture_lod_factor = max_pixel_to_texel_ratio / 8.0f
        / GPCamera->PeekNonHomogenousViewToViewScaleX() / GPCamera->PeekWindowHeight();
    bool loading = false;
    unsigned long get_flags = loading_new_scene ? 16 : 0;
    for (long n = 0; n < FastForegroundTextureLoadInfo.TexturesUsedThisFrame.size(); ++n) {
        long texture = FastForegroundTextureLoadInfo.TexturesUsedThisFrame[n];
        float distance = sqrt(FastForegroundTextureLoadInfo.TextureIDToMinPatchDistanceSquared[texture]);
        FABLE_RETAIL_ASSERT(distance >= 0);
        float texture_lod_factor = 0;
        if (distance > GPCamera->PeekNearZ())
            texture_lod_factor = base_texture_lod_factor * distance;
        const CGraphicInfoFrame* frame = GPResourceManager->GetTerrainTexture(texture);
        FABLE_RETAIL_ASSERT(frame != NULL);
        long texture_size = frame->GetWidth() > frame->GetHeight() ? frame->GetWidth() : frame->GetHeight();
        long ratio = GFFloatToLongFloor(texture_size * texture_lod_factor) > 255
            ? 255 : GFFloatToLongFloor(texture_size * texture_lod_factor);
        long lod = ratio > 0 ? GFGetTopBitInByte((unsigned char)ratio) : 0;
        CGraphicFrame graphic_frame;
        frame->GetFrame(graphic_frame, lod, get_flags);
        if (loading_new_scene && frame->IsLoading())
            loading = true;
        FastForegroundTextureLoadInfo.CurrentTextureLODLevels[texture] = lod < 0 ? 0 : lod;
        FastForegroundTextureLoadInfo.TextureIDToMinPatchDistanceSquared[texture] = -1.0f;
    }
    FastForegroundTextureLoadInfo.TexturesUsedThisFrame.resize(0);
    return loading;
}