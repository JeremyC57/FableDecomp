// CEngineShadowRenderer::UpdateShadowSpace(const CEngineCamera& camera) -- build this frame's directional-light shadow space: normalise
// Grade: DIFFER(7162v7176) -- wfE6 succ-6: frame 0x3c8 v 0x3d0 (was 0x3b0), nf5 328 (was 330), nf2 1588/1760; else region 3+16 (instruction-identical to retail 0x00B4E589..0x00B4E678 apart from slots). Residue: x87 operand/term order keyed by late c2 field records (SetInverse/frustum/scl, unchanged by the scope work) + the 4/8-byte slot partition (retail esp+0x84 = both y_axis alone). Grade with --flags /d2inlT314 and the batch hdr/. Prev best: differ/00b4d370_prev_succ5.cpp (7192v7176, frame 0x3b0).
// the lighting manager's diffuse vector into SceneData.ShadowLightVector, get the local-world-to-base shadow matrix for it, take the
// camera's view rotation (translation cleared) inverse, place the camera corner and the 5x5 far-plane grid of view-space points at
// MaxShadowDistance, move all 26 into base space, rotate the light-space picture so the camera axis is +Y (flip it when the frustum lies
// behind), then either a perspective warp (PerspectiveShadowsEnabled and the interpolated factor t > 0: shrink the points so the four far
// corners fit, perspective matrix n+1 / n) or an axis fit on the grid's mid lines (with the perspective-off ShadowFadeEnd); clamp
// ShadowFadeStart into [0, 0.9 * ShadowFadeEnd]; scale/translate by 0.5 into texture space, fit the 26 projected points into [0,1]^2 and
// compose WorldToShadowBufferMatrix = translate(-CameraPos) * local_to_base * rot; then the derived matrices and the clip volume.
// Leaf: ?UpdateShadowSpace@CEngineShadowRenderer@@AAEXABVCEngineCamera@@@Z
// Flags: /d2inlT314
// Inline-budget: /d2inlT314 -- flagless the row calls CMatrix4x4::PostMultiply out of line at all three sites (9 calls v retail 5; wfE6 succ-8);
//   PostMultiply expands from T~190 (callee-only cost) and T 190..1000 give identical code; same TU and callee as the landed sibling 0x00B4FC40
//   SetupShadowFuncRenderStates (/d2inlT314, PostMultiply scores 313 v default 74; unreferenced retail COMDAT 0x00B507E0).
// FITTED (wfE6 successor-2, chosen by grading, code-neutral in behaviour): FABLE_RETAIL_CMATRIX3X4_SETINVERSE_TRANSLATION_LOCAL (SetInverse
//   reads m's translation into a C3DVector first: retail's cofactor order 0x00B4D40A..); the else-branch axes as new locals of the else
//   block (retail stores both straight to their homes 0x00B4E5A5..0x00B4E5E2). EVIDENCED: TRANSFORMPOINT_LTR = FableWin 0x024543E0 text
//   (the translation rows then factor like retail 0x00B4D5C1); `light_points[k] *= scale` (retail's 13-step loop starts at X, 0x00B4E78A).
//   EVIDENCED (succ-3): u / one_minus_u defined per point inside the j loop after v / one_minus_v (retail 0x00B4D902..0x00B4D928 loads
//   u and 1-u from memory first and keeps v / 1-v in st to the end: c2 orders commutative operands by record key, and a float local
//   gets its record at its first definition; VC7.1 hoists u back out of the j loop, retail 0x00B4D8D2).
// FITTED (succ-6, nested braces, code-neutral in behaviour; measured by phook packer traces, NOTES succ-6): (1) one block from local_to_base to the last PostMultiply(rot): retail builds that PostMultiply result in local_to_base's home (0x00B4EE98 lea edx,[esp+0x104]; 0x00B4EF4C copy from esp+0x100; esp+0x130..0x13F never touched); c2 lets a slot member join an address-taken local (local_to_base = NRV return slot) only when that local is NOT function-scope; (2) the first y_axis / x_axis / rot.Initialise in their own block: retail's else-branch x_axis shares the first x_axis's home (esp+0x18, 0x00B4E5A7 v 0x00B4DD22).
// Retail: Fable.exe @ 0x00B4D370, 7176 bytes, thiscall, ret 4, esp frame (sub esp,3D0h). Identity: Ego_r.pdb 0x009F0370 / FableWin.pdb
//   0x02D74FB0 private ?UpdateShadowSpace@CEngineShadowRenderer@@AAEXABVCEngineCamera@@@Z; only caller UpdateShadowScene 0x00B4F390 (tail jmp).
// Callees: C3DVector::AccurateNormalise 0x00A14440, C2DVector::AccurateNormalise 0x00A14510, CalcLocalWorldToBaseShadowSpace 0x00B4B8B0,
//   BuildDerivedShadowMatriciesFromWorldToShadowMatrix 0x00B4BA60, CalcClipVolume 0x00B4BCC0 (the only calls). Inline (FableWin COMDATs of
//   this TU): CEngineLightingManager::PeekDiffuseVector, CMatrix3x4::SetTranslateOnly / SetInverse / TransformPoint, CMatrix4x4 ctors,
//   Initialise (16 floats and 3x4), PerspectiveTransformPoint, PostMultiply, PostScale, PostTranslate, GFLimit<float,int,int>,
//   C2DVector::GetSquaredMagnitude / GetMagnitude, C3DVector::GetMagnitude.
#include "fable_compiler.h"
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
#define FABLE_RETAIL_C3DVECTOR_ACCURATENORMALISE
#define FABLE_RETAIL_CMATRIX3X4_TRANSFORMPOINT_LTR
#define FABLE_RETAIL_CMATRIX3X4_SETINVERSE_TRANSLATION_LOCAL
#define FABLE_RETAIL_C2DVECTOR
#define FABLE_RETAIL_C2DVECTOR_ROTATE
#define FABLE_RETAIL_C2DVECTOR_NORMALISE
#define FABLE_RETAIL_C2DVECTOR_MAGNITUDE_SQUARED_LOCAL
#define FABLE_RETAIL_C2DVECTOR_FROM3D
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

namespace NEngineInternal { extern CEngineLightingManager* GPLightingManager; }   // 0x01436E9C

void CEngineShadowRenderer::UpdateShadowSpace(const CEngineCamera& camera)
{
    C3DVector light_vector = NEngineInternal::GPLightingManager->PeekDiffuseVector();
    light_vector.AccurateNormalise();
    SceneData.ShadowLightVector = light_vector;
    {
        CMatrix3x4 local_to_base = CalcLocalWorldToBaseShadowSpace(light_vector, MaxShadowDistance * 2.0f, 512.0f);

        CMatrix3x4 view = CameraViewMatrix;
        view.SetTranslateOnly(0.0f, 0.0f, 0.0f);
        CMatrix3x4 inverse;
        inverse.SetInverse(view);

        C3DVector frustum[5] = {
            C3DVector(0.0f, 0.0f, 0.0f),
            C3DVector(-MaxShadowDistance, -MaxShadowDistance, MaxShadowDistance),
            C3DVector(MaxShadowDistance, -MaxShadowDistance, MaxShadowDistance),
            C3DVector(-MaxShadowDistance, MaxShadowDistance, MaxShadowDistance),
            C3DVector(MaxShadowDistance, MaxShadowDistance, MaxShadowDistance)
        };
        for (long k = 0; k < 5; ++k)
            inverse.TransformPoint(frustum[k]);

        C3DVector points[26];
        points[0] = frustum[0];
        C3DVector* row = &points[1];
        for (long i = 0; i <= 4; ++i) {
            C3DVector* point = row;
            row += 5;
            for (long j = 0; j <= 4; ++j) {
                float v = j * 0.25f;
                float one_minus_v = 1.0f - v;
                float u = i * 0.25f;
                float one_minus_u = 1.0f - u;
                float w0 = v * u;
                float w1 = one_minus_v * u;
                float w2 = v * one_minus_u;
                float w3 = one_minus_v * one_minus_u;
                float inv = 1.0f / (w2 + w3 + w1 + w0);
                w0 *= inv;
                w1 *= inv;
                w2 *= inv;
                w3 *= inv;
                C3DVector p = frustum[1] * w0 + frustum[2] * w1 + frustum[3] * w2 + frustum[4] * w3;
                p.AccurateNormalise();
                p *= MaxShadowDistance;
                *point++ = p;
            }
        }

        C3DVector light_points[26];
        for (long k = 0; k < 26; ++k) {
            C3DVector p = points[k];
            local_to_base.TransformPoint(p);
            light_points[k] = p;
        }

        CMatrix4x4 rot;
        {
            C2DVector y_axis(light_points[13] - light_points[0]);
            y_axis.AccurateNormalise();
            C2DVector x_axis;
            x_axis = C2DVector(y_axis.Y, -y_axis.X);
            rot.Initialise(x_axis.X, y_axis.X, 0.0f, 0.0f,
                           x_axis.Y, y_axis.Y, 0.0f, 0.0f,
                           0.0f, 0.0f, 1.0f, 0.0f,
                           0.0f, 0.0f, 0.0f, 1.0f);
        }

        float y_min = 1e30f;
        float y_max = -1e30f;
        {
            C3DVector q = light_points[1];
            rot.PerspectiveTransformPoint(q);
            if (y_min > q.Y) y_min = q.Y;
            if (y_max < q.Y) y_max = q.Y;
        }
        {
            C3DVector q = light_points[5];
            rot.PerspectiveTransformPoint(q);
            if (y_min > q.Y) y_min = q.Y;
            if (y_max < q.Y) y_max = q.Y;
        }
        {
            C3DVector q = light_points[21];
            rot.PerspectiveTransformPoint(q);
            if (y_min > q.Y) y_min = q.Y;
            if (y_max < q.Y) y_max = q.Y;
        }
        {
            C3DVector q = light_points[25];
            rot.PerspectiveTransformPoint(q);
            if (y_min > q.Y) y_min = q.Y;
            if (y_max < q.Y) y_max = q.Y;
        }

        float near_y;
        if (y_min < 0.0f) {
            if (y_max > 0.0f)
                near_y = 0.0f;
            else {
                rot.PostScale(C3DVector(-1.0f, -1.0f, 1.0f));
                near_y = -y_max;
            }
        } else
            near_y = y_min;

        float t = 0.0f;
        if (PerspectiveShadowsEnabled) {
            float start = PerspectiveInterpStart;
            float end = PerspectiveInterpEnd;
            t = (near_y - start) / (end - start);
            GFLimit(t, 0, 1);
        }
        if (t > 0.0f) {
            float f = ForegroundBufferSpan / MaxShadowDistance;
            float n = f / ((1.0f - t) * f + t - f);
            float limit = f / n;
            float scale = 1.0f;
            {
                C3DVector q = light_points[1];
                rot.PerspectiveTransformPoint(q);
                C2DVector projected(q.X / (n + q.Y), q.Y / (n + q.Y));
                if (fabs(projected.X) > limit) {
                    float z = projected.Y / fabs(projected.X) * limit;
                    float candidate = n * z / (1.0f - z) / q.Y;
                    if (scale > candidate) scale = candidate;
                }
            }
            {
                C3DVector q = light_points[5];
                rot.PerspectiveTransformPoint(q);
                C2DVector projected(q.X / (n + q.Y), q.Y / (n + q.Y));
                if (fabs(projected.X) > limit) {
                    float z = projected.Y / fabs(projected.X) * limit;
                    float candidate = n * z / (1.0f - z) / q.Y;
                    if (scale > candidate) scale = candidate;
                }
            }
            {
                C3DVector q = light_points[21];
                rot.PerspectiveTransformPoint(q);
                C2DVector projected(q.X / (n + q.Y), q.Y / (n + q.Y));
                if (fabs(projected.X) > limit) {
                    float z = projected.Y / fabs(projected.X) * limit;
                    float candidate = n * z / (1.0f - z) / q.Y;
                    if (scale > candidate) scale = candidate;
                }
            }
            {
                C3DVector q = light_points[25];
                rot.PerspectiveTransformPoint(q);
                C2DVector projected(q.X / (n + q.Y), q.Y / (n + q.Y));
                if (fabs(projected.X) > limit) {
                    float z = projected.Y / fabs(projected.X) * limit;
                    float candidate = n * z / (1.0f - z) / q.Y;
                    if (scale > candidate) scale = candidate;
                }
            }
            for (long k = 0; k < 26; ++k) {
                light_points[k] *= scale;
            }
            SceneData.ShadowFadeEnd = scale * MaxShadowDistance;
            CMatrix4x4 perspective;
            float n1 = n + 1.0f;
            perspective.Initialise(n1, 0.0f, 0.0f, 0.0f,
                                   0.0f, n1, 0.0f, 1.0f,
                                   0.0f, 0.0f, n1, 0.0f,
                                   0.0f, 0.0f, 0.0f, n);
            rot.PostMultiply(perspective);
        } else {
            C2DVector x_axis(light_points[15] - light_points[11]);
            C2DVector y_axis(light_points[23] - light_points[3]);
            if (x_axis.GetSquaredMagnitude() > y_axis.GetSquaredMagnitude())
                y_axis = C2DVector(-x_axis.Y, x_axis.X);
            else
                x_axis = C2DVector(y_axis.Y, -y_axis.X);
            rot.Initialise(x_axis.X, y_axis.X, 0.0f, 0.0f,
                           x_axis.Y, y_axis.Y, 0.0f, 0.0f,
                           0.0f, 0.0f, 1.0f, 0.0f,
                           0.0f, 0.0f, 0.0f, 1.0f);
            if (PerspectiveShadowsEnabled) {
                float length = (frustum[2] - frustum[1]).GetMagnitude();
                C2DVector half_extent(0.5f * length, MaxShadowDistance);
                float one_over_max = 1.0f / MaxShadowDistance;
                float radius = half_extent.GetMagnitude();
                SceneData.ShadowFadeEnd = ForegroundBufferSpan / (MaxShadowDistance / radius * MaxShadowDistance * one_over_max * length) * MaxShadowDistance * 2.0f;
                float scale = SceneData.ShadowFadeEnd * one_over_max;
                for (long k = 0; k < 26; ++k)
                    light_points[k] *= scale;
            } else
                SceneData.ShadowFadeEnd = MaxShadowDistance;
        }

        if (SceneData.ShadowFadeStart < 0.0f)
            SceneData.ShadowFadeStart = 0.0f;
        else if (SceneData.ShadowFadeStart > SceneData.ShadowFadeEnd * 0.9)
            SceneData.ShadowFadeStart = (float)(SceneData.ShadowFadeEnd * 0.9);

        rot.PostScale(C3DVector(0.5f, 0.5f, 0.5f));
        rot.PostTranslate(C3DVector(0.5f, 0.5f, 0.5f));

        float x_min = 1e30f;
        float x_max = -1e30f;
        float y_min2 = 1e30f;
        float y_max2 = -1e30f;
        C3DVector* point = light_points;
        for (long k = 0; k < 26; ++k, ++point) {
            C3DVector q = *point;
            rot.PerspectiveTransformPoint(q);
            if (x_max < q.X) x_max = q.X;
            if (x_min > q.X) x_min = q.X;
            if (y_max2 < q.Y) y_max2 = q.Y;
            if (y_min2 > q.Y) y_min2 = q.Y;
        }
        rot.PostTranslate(C3DVector(-x_min, -y_min2, 0.0f));
        rot.PostScale(C3DVector(1.0f / (x_max - x_min), 1.0f / (y_max2 - y_min2), 1.0f));

        SceneData.WorldToShadowBufferMatrix = CMatrix4x4(MATRIX34_INIT_IDENTITY);
        SceneData.WorldToShadowBufferMatrix.PostTranslate(C3DVector(-SceneData.CameraPos.X, -SceneData.CameraPos.Y, -SceneData.CameraPos.Z));
        SceneData.WorldToShadowBufferMatrix.PostMultiply(CMatrix4x4(local_to_base));
        SceneData.WorldToShadowBufferMatrix.PostMultiply(rot);
    }
    BuildDerivedShadowMatriciesFromWorldToShadowMatrix();
    CalcClipVolume(camera);
}
