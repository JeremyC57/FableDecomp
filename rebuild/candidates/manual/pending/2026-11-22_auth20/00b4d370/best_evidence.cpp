#include "fable_compiler.h"
#define FABLE_RETAIL_ASSERT_DEAD_ARM
#include "fable_assert.h"
#define FABLE_RETAIL_STLP_VECTOR_SWAP
#define FABLE_RETAIL_GFLIMIT_UPPER_LOWER
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_CENGINESHADOWRENDERER
#define FABLE_RETAIL_CENGINESHADOWRENDERER_MEMBERS
#define FABLE_RETAIL_CENGINESHADOWRENDERER_UPDATESHADOWSPACE
#define FABLE_RETAIL_CENGINECOMPONENT
#define FABLE_RETAIL_CMATRIX4X4_FROMMATRIX3X4
#define FABLE_RETAIL_CMATRIX4X4_SHADOWSPACE
#define FABLE_RETAIL_CENGINELIGHTINGMANAGER
#define FABLE_RETAIL_CENGINELIGHTINGMANAGER_MEMBERS
#define FABLE_RETAIL_CENGINELIGHTINGMANAGER_PEEKDIFFUSEVECTOR
#define FABLE_RETAIL_C4DVECTOR
#define FABLE_RETAIL_CMATRIX3X4
#define FABLE_RETAIL_CMATRIX3X4_SETTRANSLATEONLY
#define FABLE_RETAIL_CMATRIX3X4_SETTRANSLATEONLY_FLOATS
#define FABLE_RETAIL_CMATRIX3X4_TRANSFORMPOINT_LTR
#define FABLE_RETAIL_CMATRIX3X4_INVERSE_NO_TRANSLATE_PASS23
#define FABLE_RETAIL_CMATRIX3X4_SETINVERSE_DONOR_PASS26
#define FABLE_RETAIL_C2DVECTOR
#define FABLE_RETAIL_C2DVECTOR_ROTATE
#define FABLE_RETAIL_C2DVECTOR_NORMALISE
#define FABLE_RETAIL_C2DVECTOR_FROM3D
#define FABLE_RETAIL_C2DVECTOR_ASSIGN3D
#define FABLE_RETAIL_C3DVECTOR_SCALE_NAMED_RESULT
#define FABLE_RETAIL_C3DVECTOR_MAGNITUDE_SQUARED_LOCAL
#include <math.h>
#include "engine/C2DVector.h"
#include "engine/CMatrix4x4.h"
#include "engine/CRenderStateCaptureBlock.h"
#include "engine/CRenderTarget.h"
#include "engine/CShadowClipVolume.h"
#include "engine/CShadowRendererSceneData.h"
#include "engine/CEngineLightingManager.h"
#include "engine/CEngineShadowRenderer.h"
#include "engine/GFLimit.h"
#include "stlport/fable_containers.h"

namespace NEngineInternal { extern CEngineLightingManager* GPLightingManager; }   // 0x01436E9C

void CEngineShadowRenderer::UpdateShadowSpace(const CEngineCamera& camera)
{
    C3DVector shadow_light_vector = NEngineInternal::GPLightingManager->PeekDiffuseVector();
    shadow_light_vector.AccurateNormalise();
    SceneData.ShadowLightVector = shadow_light_vector;

    CMatrix3x4 local_world_to_base_shadow_space = CalcLocalWorldToBaseShadowSpace(shadow_light_vector, MaxShadowDistance * 2.0f, 512.0f);

    CMatrix3x4 view_mat = CameraViewMatrix;
    view_mat.SetTranslateOnly(0.0f, 0.0f, 0.0f);
    CMatrix3x4 view_to_world;
    view_to_world.SetInverse(view_mat);

    const int sub_divs = 4;
    const int point_count = (sub_divs + 1) * (sub_divs + 1) + 1;

    C3DVector frustrum_points[5] = {
        C3DVector(0.0f, 0.0f, 0.0f),
        C3DVector(-MaxShadowDistance, -MaxShadowDistance, MaxShadowDistance),
        C3DVector(MaxShadowDistance, -MaxShadowDistance, MaxShadowDistance),
        C3DVector(-MaxShadowDistance, MaxShadowDistance, MaxShadowDistance),
        C3DVector(MaxShadowDistance, MaxShadowDistance, MaxShadowDistance)
    };
    for (long pt0 = 0; pt0 < 5; pt0++)
    {
        view_to_world.TransformPoint(frustrum_points[pt0]);
    }

    C3DVector world00 = frustrum_points[1];
    C3DVector world10 = frustrum_points[2];
    C3DVector world01 = frustrum_points[3];
    C3DVector world11 = frustrum_points[4];

    C3DVector world_space_control_points[point_count];
    world_space_control_points[0] = frustrum_points[0];
    long new_point_id = 1;
    for (long y = 0; y <= sub_divs; y++)
    {
        for (long x = 0; x <= sub_divs; x++)
        {
            float weightx0 = (float)x / sub_divs;
            float weighty0 = (float)y / sub_divs;

            float weightx1 = 1.0f - weightx0;
            float weighty1 = 1.0f - weighty0;

            float weight00 = weightx0 * weighty0;
            float weight10 = weightx1 * weighty0;
            float weight01 = weightx0 * weighty1;
            float weight11 = weightx1 * weighty1;

            float total_weight = weight00 + weight10 + weight01 + weight11;

            weight00 /= total_weight;
            weight10 /= total_weight;
            weight01 /= total_weight;
            weight11 /= total_weight;

            C3DVector new_pos = world00 * weight00 + world10 * weight10 + world01 * weight01 + world11 * weight11;
            new_pos.AccurateNormalise();
            new_pos *= MaxShadowDistance;
            world_space_control_points[new_point_id] = new_pos;
            new_point_id++;
        }
    }

    C3DVector shadow_space_control_points[point_count];
    for (long pt0 = 0; pt0 < point_count; pt0++)
    {
        C3DVector pos = world_space_control_points[pt0];
        local_world_to_base_shadow_space.TransformPoint(pos);
        shadow_space_control_points[pt0] = pos;
    }

    C3DVector shadow_space_corner_control_points[4] = {
        shadow_space_control_points[1],
        shadow_space_control_points[5],
        shadow_space_control_points[21],
        shadow_space_control_points[25]
    };

    C2DVector new_shadow_space_y = shadow_space_control_points[13] - shadow_space_control_points[0];
    new_shadow_space_y.AccurateNormalise();
    C2DVector new_shadow_space_x;
    new_shadow_space_x = C2DVector(new_shadow_space_y.Y, -new_shadow_space_y.X);

    CMatrix4x4 new_shadow_transform;
    new_shadow_transform.Initialise(new_shadow_space_x.X, new_shadow_space_y.X, 0.0f, 0.0f,
                                    new_shadow_space_x.Y, new_shadow_space_y.Y, 0.0f, 0.0f,
                                    0.0f, 0.0f, 1.0f, 0.0f,
                                    0.0f, 0.0f, 0.0f, 1.0f);

    float min_y = 1e30f;
    float max_y = -1e30f;
    for (long n = 0; n < 4; n++)
    {
        C3DVector corner_point = shadow_space_corner_control_points[n];
        new_shadow_transform.PerspectiveTransformPoint(corner_point);
        GFLimitUpper(min_y, corner_point.Y);
        GFLimitLower(max_y, corner_point.Y);
    }

    if (min_y < 0.0f)
    {
        if (max_y > 0.0f)
        {
            min_y = 0.0f;
            max_y = 0.0f;
        }
        else
        {
            new_shadow_transform.PostScale(C3DVector(-1.0f, -1.0f, 1.0f));
            std::swap(min_y, max_y);
            min_y = -min_y;
            max_y = -max_y;
        }
    }

    bool transform_setup_for_perspective_shadows = false;
    if (PerspectiveShadowsEnabled)
    {
        float perspective_interp_range_start = PerspectiveInterpStart;
        float perspective_interp_range_end = PerspectiveInterpEnd;
        float perspective_interp = (min_y - perspective_interp_range_start) / (perspective_interp_range_end - perspective_interp_range_start);
        GFLimit(perspective_interp, 0, 1);

        if (perspective_interp > 0.0f)
        {
            float frustrum_forground_width = ForegroundBufferSpan / MaxShadowDistance;
            float frustrum_background_width = 1.0f;
            float max_frustrum_background_width = (1.0f - perspective_interp) * frustrum_forground_width + perspective_interp * frustrum_background_width;
            float perspective_shift = frustrum_forground_width / (max_frustrum_background_width - frustrum_forground_width);
            float max_projected_range_x = frustrum_forground_width / perspective_shift;
            float scale = 1.0f;
            for (long n = 0; n < 4; n++)
            {
                C3DVector corner_point = shadow_space_corner_control_points[n];
                new_shadow_transform.PerspectiveTransformPoint(corner_point);
                C3DVector projected_corner_point = corner_point / (perspective_shift + corner_point.Y);
                if (fabs(projected_corner_point.X) > max_projected_range_x)
                {
                    float clipped_projected_y = projected_corner_point.Y * max_projected_range_x / fabs(projected_corner_point.X);
                    float clipped_uniform_y = perspective_shift * clipped_projected_y / (1.0f - clipped_projected_y);
                    float frustrum_ratio = clipped_uniform_y / corner_point.Y;
                    GFLimitUpper(scale, frustrum_ratio);
                }
            }
            for (long n = 0; n < point_count; n++)
            {
                shadow_space_control_points[n] *= scale;
            }
            SceneData.ShadowFadeEnd = scale * MaxShadowDistance;

            float frustrum_reduction_factor = 1.0f + perspective_shift;
            CMatrix4x4 perspective_transform;
            perspective_transform.Initialise(frustrum_reduction_factor, 0.0f, 0.0f, 0.0f,
                                             0.0f, frustrum_reduction_factor, 0.0f, 1.0f,
                                             0.0f, 0.0f, frustrum_reduction_factor, 0.0f,
                                             0.0f, 0.0f, 0.0f, perspective_shift);
            new_shadow_transform.PostMultiply(perspective_transform);
            transform_setup_for_perspective_shadows = true;
        }
    }

    if (!transform_setup_for_perspective_shadows)
    {
        new_shadow_space_x = shadow_space_control_points[15] - shadow_space_control_points[11];
        new_shadow_space_y = shadow_space_control_points[23] - shadow_space_control_points[3];
        if (new_shadow_space_x.GetSquaredMagnitude() > new_shadow_space_y.GetSquaredMagnitude())
            new_shadow_space_y = C2DVector(-new_shadow_space_x.Y, new_shadow_space_x.X);
        else
            new_shadow_space_x = C2DVector(new_shadow_space_y.Y, -new_shadow_space_y.X);

        new_shadow_transform.Initialise(new_shadow_space_x.X, new_shadow_space_y.X, 0.0f, 0.0f,
                                        new_shadow_space_x.Y, new_shadow_space_y.Y, 0.0f, 0.0f,
                                        0.0f, 0.0f, 1.0f, 0.0f,
                                        0.0f, 0.0f, 0.0f, 1.0f);

        if (PerspectiveShadowsEnabled)
        {
            float end_frustrum_width = (frustrum_points[2] - frustrum_points[1]).GetMagnitude();
            float diag = C2DVector(end_frustrum_width * 0.5f, MaxShadowDistance).GetMagnitude();
            float diag_to_center_ratio = MaxShadowDistance / diag;
            float sub_center_distance = diag_to_center_ratio * MaxShadowDistance;
            float reduced_frust_width = sub_center_distance / MaxShadowDistance * end_frustrum_width;
            float new_shadow_distance = ForegroundBufferSpan / reduced_frust_width * MaxShadowDistance / 0.5f;
            SceneData.ShadowFadeEnd = new_shadow_distance;
            for (long n = 0; n < point_count; n++)
            {
                shadow_space_control_points[n] *= SceneData.ShadowFadeEnd / MaxShadowDistance;
            }
        }
        else
        {
            SceneData.ShadowFadeEnd = MaxShadowDistance;
        }
    }

    if (SceneData.ShadowFadeStart < 0.0f) SceneData.ShadowFadeStart = 0.0f; else if (SceneData.ShadowFadeStart > SceneData.ShadowFadeEnd * 0.9) SceneData.ShadowFadeStart = (float)(SceneData.ShadowFadeEnd * 0.9);

    new_shadow_transform.PostScale(C3DVector(0.5f, 0.5f, 0.5f));
    new_shadow_transform.PostTranslate(C3DVector(0.5f, 0.5f, 0.5f));

    float new_shadow_minx = 1e30f;
    float new_shadow_maxx = -1e30f;
    float new_shadow_miny = 1e30f;
    float new_shadow_maxy = -1e30f;
    for (long n = 0; n < point_count; n++)
    {
        C3DVector rotated_point = shadow_space_control_points[n];
        new_shadow_transform.PerspectiveTransformPoint(rotated_point);
        GFLimitLower(new_shadow_maxx, rotated_point.X);
        GFLimitUpper(new_shadow_minx, rotated_point.X);
        GFLimitLower(new_shadow_maxy, rotated_point.Y);
        GFLimitUpper(new_shadow_miny, rotated_point.Y);
    }

    FABLE_RETAIL_ASSERT(new_shadow_maxx > new_shadow_minx);
    FABLE_RETAIL_ASSERT(new_shadow_maxy > new_shadow_miny);

    new_shadow_transform.PostTranslate(C3DVector(-new_shadow_minx, -new_shadow_miny, 0.0f));
    new_shadow_transform.PostScale(C3DVector(1.0f / (new_shadow_maxx - new_shadow_minx), 1.0f / (new_shadow_maxy - new_shadow_miny), 1.0f));

    SceneData.WorldToShadowBufferMatrix = CMatrix4x4(MATRIX34_INIT_IDENTITY);
    SceneData.WorldToShadowBufferMatrix.PostTranslate(-SceneData.CameraPos);
    SceneData.WorldToShadowBufferMatrix.PostMultiply(CMatrix4x4(local_world_to_base_shadow_space));
    SceneData.WorldToShadowBufferMatrix.PostMultiply(new_shadow_transform);

    BuildDerivedShadowMatriciesFromWorldToShadowMatrix();
    CalcClipVolume(camera);
}
