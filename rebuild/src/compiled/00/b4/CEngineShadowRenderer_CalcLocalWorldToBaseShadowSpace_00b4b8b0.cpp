// CEngineShadowRenderer::CalcLocalWorldToBaseShadowSpace(const C3DVector& light_vector, float shadow_box_width, float shadow_box_length) -- build the light-space basis of a shadow direction (helper axis pvec = Y or X, whichever is further from the light; shadow_world_x = pvec ^ light, shadow_world_y = shadow_world_x ^ light, both normalised) and return it as a matrix whose columns are scaled by 2/width, 2/width and 2/length
// Leaf: ?CalcLocalWorldToBaseShadowSpace@CEngineShadowRenderer@@AAE?AVCMatrix3x4@@ABVC3DVector@@MM@Z
// Retail: Fable.exe (TLC, VC7.1) 0x00B4B8B0, 421 bytes, `ret 10h` (hidden result pointer, 12-dword copy of the local matrix).
// Names: Ego_r.pdb / FableWin.pdb ?CalcLocalWorldToBaseShadowSpace@CEngineShadowRenderer@@AAE?AVCMatrix3x4@@ABVC3DVector@@MM@Z (private);
//   parameters and locals light_vector / shadow_box_width / shadow_box_length / pvec / local_world_to_shadow / shadow_light_vector /
//   shadow_world_x / shadow_world_y from Ego_d.pdb (Engine_Shadow_Renderer2.obj); our /Od frame puts every one of them at Ego_d's offset.
//   C3DVector::AccurateNormalise 0x00A14440 (bound spelling, reloc_bindings.tsv). The 12-float CMatrix3x4 constructor and
//   PostScale(const C3DVector&) are FableWin-size SAME (fwsize: 123b / 223b).
// FITTED (chosen by grading; FableWin / Ego_d /Od size 376b, this text 282b): pvec.Initialise(...) in each arm (keeps pvec's X / Y in x87 registers and gives retail's `fld st(2) ; fmul [L.Z]`,
//   0x00B4B922); FITTED: the shared grouped cross-product text (existing opt-in FABLE_RETAIL_C3DVECTOR_CROSS_XYZ_GROUP) moves the
//   scheduling cut so the result-copy set-up (0x00B4BA12..0x00B4BA2B) interleaves with the last column stores as retail does.
#include <math.h>
#pragma intrinsic(fabs)
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_C3DVECTOR_CROSS_XYZ_GROUP
#define FABLE_RETAIL_CENGINESHADOWRENDERER
#define FABLE_RETAIL_CENGINESHADOWRENDERER_MEMBERS
#define FABLE_RETAIL_CENGINESHADOWRENDERER_CALCLOCALWORLD
#define FABLE_RETAIL_CMATRIX3X4_POSTSCALE_XYZ
#define FABLE_RETAIL_CMATRIX3X4_PREMULTIPLY
#include "engine/CEngineShadowRenderer.h"

CMatrix3x4 CEngineShadowRenderer::CalcLocalWorldToBaseShadowSpace(const C3DVector& light_vector, float shadow_box_width, float shadow_box_length)
{
    C3DVector shadow_light_vector = light_vector;
    C3DVector pvec;
    if (fabs(shadow_light_vector.X) > fabs(shadow_light_vector.Y))
        pvec.Initialise(0.0f, 1.0f, 0.0f);
    else
        pvec.Initialise(1.0f, 0.0f, 0.0f);
    C3DVector shadow_world_x = pvec ^ shadow_light_vector;
    C3DVector shadow_world_y = shadow_world_x ^ shadow_light_vector;
    shadow_world_y.AccurateNormalise();
    shadow_world_x.AccurateNormalise();
    CMatrix3x4 local_world_to_shadow(shadow_world_y.X, shadow_world_x.X, shadow_light_vector.X,
                                     shadow_world_y.Y, shadow_world_x.Y, shadow_light_vector.Y,
                                     shadow_world_y.Z, shadow_world_x.Z, shadow_light_vector.Z,
                                     0.0f, 0.0f, 0.0f);
    local_world_to_shadow.PostScale(C3DVector(2.0f / shadow_box_width, 2.0f / shadow_box_width, 2.0f / shadow_box_length));
    return local_world_to_shadow;
}