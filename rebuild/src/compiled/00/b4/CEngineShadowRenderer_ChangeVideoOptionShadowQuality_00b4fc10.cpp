// CEngineShadowRenderer::ChangeVideoOptionShadowQuality(long shadow_buffer_size, float shadow_distance_scale) -- store the user's shadow video options (+0x338 buffer size, +0x33C distance scale), release the default-pool resources and rebuild the shadow textures
// Leaf: ?ChangeVideoOptionShadowQuality@CEngineShadowRenderer@@QAE_NJM@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B4FC10, length 39 bytes (+ int3 padding), `ret 8`, the bool result in al.
// Name (NF152, 2026-11-20, q5011): Ego_r.pdb 0x009EED40 / FableWin.pdb 0x02D73600 ?ChangeVideoOptionShadowQuality@CEngineShadowRenderer@@QAE_NJM@Z
//   (bool (long, float)); the body stores the long into UserVideoOptionShadowBufferSize +0x338 and the float into
//   UserVideoOptionShadowDistanceScale +0x33C (Ego_r.pdb CEngineShadowRenderer members) and returns InitialiseTextures' bool. The caller
//   0x00B24500 already binds this spelling. Parameter names are descriptive (no Ego_d record). Was the label SetShadowMapSize(long, long), void.
// Callees: CEngineShadowRenderer::OnReleaseDefaultPoolResources @ 0x00B4B830 (the virtual override, called directly: both PDBs
//   ?OnReleaseDefaultPoolResources@CEngineShadowRenderer@@UAEXXZ), CEngineShadowRenderer::InitialiseTextures @ 0x00B4F8E0 (private,
//   ?InitialiseTextures@CEngineShadowRenderer@@AAE_NXZ).
#define FABLE_RETAIL_CENGINESHADOWRENDERER
#define FABLE_RETAIL_CENGINESHADOWRENDERER_MEMBERS
#define FABLE_RETAIL_CENGINESHADOWRENDERER_INITIALISETEXTURES
#define FABLE_RETAIL_CENGINESHADOWRENDERER_ONRELEASEDEFAULTPOOLRESOURCES
#define FABLE_RETAIL_CENGINESHADOWRENDERER_CHANGEVIDEOOPTIONSHADOWQUALITY
#include "engine/CEngineShadowRenderer.h"

bool CEngineShadowRenderer::ChangeVideoOptionShadowQuality(long shadow_buffer_size, float shadow_distance_scale)
{
    UserVideoOptionShadowBufferSize = shadow_buffer_size;
    UserVideoOptionShadowDistanceScale = shadow_distance_scale;
    CEngineShadowRenderer::OnReleaseDefaultPoolResources();
    return InitialiseTextures();
}
