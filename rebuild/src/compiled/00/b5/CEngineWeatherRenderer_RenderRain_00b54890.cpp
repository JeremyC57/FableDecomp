// CEngineWeatherRenderer::RenderRain(const CEngineComponentRenderParams& params) -- draw the rain particle boxes: inside a render-state capture block, bind the environment's PC rain volume texture and the rain vertex / index buffers, upload the camera's two non-homogenous view matrices (transposed, constants 1..6), the drop size / fade / wind / time constants and the five per-set wind and speed factors, then walk the 81 particle box columns over nine height levels (each box clipped against the camera unless the boxes are locked, then against the landscape; boxes inside a weather mask sphere go to the clipping list) and render the plain boxes with the particle vertex shader and the masked ones with the clipping vertex shader
// Pair-landing (chunk 2037): the same TU now holds the three clip bodies retail calls -- ClipBoxToCamera 0x00B51F60 and
//   ClipBoxToWeatherMaskSphere 0x00B52090 (ported onto the shared headers from the landed CWeatherRendererMethods::ClipBoxToCamera /
//   CEngineWeatherRenderer::IsPointInPrecipitation; each compiled out of line grades RELOCATION_MATCH against its retail bytes) and
//   ClipBoxToLandscape 0x00B52DD0 (this chunk; the same body text as the matched src/00b52dd0.cpp, which grades RELOCATION_MATCH at
//   /d2inlT507; here it is compiled at the TU's default budget) -- each __declspec(noinline) as retail calls them. With their side effects visible VC7.1
//   schedules the two inline push_back C3DVector copies load-ahead as retail (0x00B54C8D..0x00B54C9F, 0x00B54CD3..0x00B54CE4); the 2031/2008 hand-backs
//   graded DIFFER only there without them.
// Retail-asm: same-TU callee body only (this row's own leaf has no asm): ClipBoxToLandscape 0x00B52DD0 expands the shared helpers GFFloatToLongFloor (0x00B52E22 `fld [v] ; fsub [half] ; fistp dword ptr [r] ; fild [r] ; fadd [one] ; fstp [n] ; mov eax,[v] ; cmp eax,[n] ; jne ; inc dword ptr [r]`), GFFloatToLongCeil (0x00B532A3 `fld ; fadd ; fistp dword ptr ; fild ; fsub ; fstp ; mov ; cmp ; jne ; dec`) and GFFloatToLongNear (0x00B53231 `fld [v] ; fistp dword ptr [r]`); each helper header holds the proof that VC7.1 emits no bare dword fistp from C++.
// Leaf: ?RenderRain@CEngineWeatherRenderer@@AAEXABVCEngineComponentRenderParams@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00B54890, 1431 bytes, thiscall, `ret 4`.
// Identity (two sources): Ego_r.pdb 0x009FAAD0 / FableWin.pdb 0x02D3BE30 ?RenderRain@CEngineWeatherRenderer@@AAEXABVCEngineComponentRenderParams@@@Z
//   (private); the body renders through RenderRainBox 0x00B54100 (Ego_r ?RenderRainBox@CEngineWeatherRenderer@@AAEXABVC3DVector@@@Z) with the
//   Rain* shaders and reads Settings.Rain* only (the snow twin is RenderSnow).
// Callees: CGraphicInfo::GetVolumeTexture 0x009FFB20 (graphic bank 0 of NEngineInternal::GPResourceManager 0x01436E98, the environment def's
//   RainTexturePC), CShaderRenderManager::SetUserVertexShaderConstant(unsigned long, float x 4) 0x00989B00 / ApplyPixelShader 0x00988140 /
//   ApplyVertexShader 0x00988020 (NEngineInternal::GPShaderRenderManager 0x01436E14), SetCommonParticleShaderConstants 0x00B52160,
//   ClipBoxToCamera 0x00B51F60, ClipBoxToLandscape 0x00B52DD0, ClipBoxToWeatherMaskSphere 0x00B52090, STLport
//   vector<C3DVector>::_M_insert_overflow 0x0066E370, RenderRainBox 0x00B54100, free 0x00BFEA14, RestoreCaptureBlock 0x00A05840; the inline
//   CRenderManagerCore AttachVolumeTextureToStage / SelectVertexBuffer / SelectIndexBuffer (NEngineInternal::GPRenderManager 0x01436E18).
#include "fable_compiler.h"
#define FABLE_RETAIL_CENGINECOMPONENT
#define FABLE_RETAIL_CENGINEWEATHERRENDERER
#define FABLE_RETAIL_CENGINEWEATHERRENDERER_BUFFERS
#define FABLE_RETAIL_CENGINEWEATHERRENDERER_RENDERRAIN
#define FABLE_RETAIL_CENGINEWEATHERSETTINGS_TAIL
#define FABLE_RETAIL_CENGINERESOURCEMANAGER
#define FABLE_RETAIL_STATICBLOCKS_TEXTURE_HELPERS
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CIVCOUNTEDPOINTER
#define FABLE_RETAIL_CMOVABLERESOURCE
#define FABLE_RETAIL_CGRAPHICINFO_GETVOLUMETEXTURE
#define FABLE_RETAIL_CRENDERMANAGERCORE
#define FABLE_RETAIL_CRENDERMANAGERCORE_VOLUMETEXTURE_INLINE
#define FABLE_RETAIL_CTEXTURE
#define FABLE_RETAIL_CENGINECAMERA_POSITION
#define FABLE_RETAIL_CENGINECAMERA_NONHOMOGENOUS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CENVIRONMENTDEF_PARENT
#define FABLE_RETAIL_C3DBOUNDINGBOX
#define FABLE_RETAIL_C3DBOUNDINGSPHERE_CENTER_VALUE
#define FABLE_RETAIL_CENGINECAMERA_CLIPBOX
#define FABLE_RETAIL_CENGINEWEATHERRENDERER_CLIPBODIES
#define FABLE_RETAIL_CENGINEMAP_PEEK
#define FABLE_RETAIL_CENGINEMAP_PEEK_BODIES
#define FABLE_RETAIL_CIDRAWMAP
#define FABLE_RETAIL_CIDRAWMAP_DRAW
#define FABLE_RETAIL_CENGINEWORLDMAP
#define FABLE_RETAIL_CENGINEWORLDMAP_GETMAPXY
#define FABLE_RETAIL_CENGINEWORLDMAP_PEEKLANDSCAPE
#define FABLE_RETAIL_CENGINEMAP_MAPHASDATA_IFRETURN
#include "fable_string.h"
#include "engine/C3DVector.h"
#include "engine/CEngineWeatherRenderer.h"
#include "engine/CEngineResourceManager.h"
#include "engine/CGraphicInfo.h"
#include "engine/CVolumeTexture.h"
#include "engine/CRenderManager.h"
#include "engine/CShaderRenderManager.h"
#include "engine/CRenderStateCaptureBlock.h"
#include "engine/CEngineCamera.h"
#include "engine/CEnvironmentDef.h"
#include "engine/CArray.h"
#include "engine/C3DBoundingSphere.h"
#include "engine/CEngineMap.h"
#include "engine/CEngineWorldMap.h"
#include "engine/CIDrawMap.h"
#include "engine/GFFloatToLongFloor.h"
#include "engine/GFFloatToLongCeil.h"
extern "C" double __cdecl sqrt(double value);
#pragma intrinsic(sqrt)
#define WEATHER_MIN(a, b) ((a) < (b) ? (a) : (b))

namespace NEngineInternal { extern CEngineResourceManager* GPResourceManager; }        // 0x01436E98
namespace NEngineInternal { extern CRenderManager* GPRenderManager; }                  // 0x01436E18
namespace NEngineInternal { extern CShaderRenderManager* GPShaderRenderManager; }      // 0x01436E14
namespace NEngineInternal { extern CEngineCamera* GPCamera; }                          // 0x01436EA0
extern const CEnvironmentDef* g_pEngineEnvironmentDef;                                // 0x01436E24 (established descriptive spelling)

// FableWin.pdb ?PeekGraphic@CGraphicDataBank@@QBEPBVCGraphicInfo@@K@Z, inline in retail (0x00B548CE..0x00B548FE: the index tested
// against GraphicList's size, a null graphic otherwise), as the landed 0x00BA48F0 spells it.
inline const CGraphicInfo* CGraphicDataBank::PeekGraphic(unsigned long index) const
{
    return index < GraphicList.size() ? &GraphicList[index] : 0;
}


// 0x00B51F60 (same TU, before RenderRain / RenderSnow; landed as CWeatherRendererMethods::ClipBoxToCamera): out of line, as retail calls it.
#ifdef _MSC_VER
__declspec(noinline)
#endif
CEngineWeatherRenderer::EWeatherBoxVisible CEngineWeatherRenderer::ClipBoxToCamera(const C3DVector& pos, float radius) const
{
    const CEngineCamera* camera = NEngineInternal::GPCamera;
    float box_radius = WEATHER_MIN(WEATHER_MIN(Settings.ParticleBoxSpan.X, Settings.ParticleBoxSpan.Y), Settings.ParticleBoxSpan.Z);
    float x = camera->PeekCameraPos().X - pos.X, y = camera->PeekCameraPos().Y - pos.Y, z = camera->PeekCameraPos().Z - pos.Z;
    float squared = y * y;
    squared += z * z;
    squared += x * x;
    if (sqrt(squared) - box_radius <= radius) {
        CMatrix3x4 box;
        box.E11 = Settings.ParticleBoxSpan.X;
        box.E22 = Settings.ParticleBoxSpan.Y;
        box.E33 = Settings.ParticleBoxSpan.Z;
        box.E12 = 0; box.E13 = 0; box.E21 = 0; box.E23 = 0; box.E31 = 0; box.E32 = 0;
        box.E41 = pos.X; box.E42 = pos.Y; box.E43 = pos.Z;
        if (camera->ClipBox(box))
            return WEATHER_BOX_VISIBLE;
    }
    return WEATHER_BOX_CLIPPED;
}

// 0x00B52090 (same TU; landed as CEngineWeatherRenderer::IsPointInPrecipitation): out of line, as retail calls it.
#ifdef _MSC_VER
__declspec(noinline)
#endif
CEngineWeatherRenderer::EWeatherBoxVisible CEngineWeatherRenderer::ClipBoxToWeatherMaskSphere(const C3DVector& pos) const
{
    float box_radius = (float)sqrt(GetBoxBoundingSphereRadiusSquared());
    for (long i = 0; i < 3; ++i) {
        const C3DBoundingSphere& sphere = WeatherMaskList[i].BSphere;
        if ((pos - sphere.GetCenter()).GetMagnitude() < box_radius + sphere.Radius)
            return WEATHER_BOX_CLIPPED;
    }
    return WEATHER_BOX_VISIBLE;
}

// 0x00B52160 (same TU, compiled before RenderRain): the box span with the 2^23 rounding constant, then each weather mask's inverse
// transform (transposed) and box corners. Visible but kept out of line as retail calls it, so VC7.1 knows the index it steps through
// the reference is not captured (RenderRain keeps it in ebx after the call, 0x00B54B56).
#ifdef _MSC_VER
__declspec(noinline)
#endif
void CEngineWeatherRenderer::SetCommonParticleShaderConstants(long& index)
{
    NEngineInternal::GPShaderRenderManager->SetUserVertexShaderConstant(index++, Settings.ParticleBoxSpan.X, Settings.ParticleBoxSpan.Y, Settings.ParticleBoxSpan.Z, 8388608.0f);
    for (long m = 0; m < 3; ++m) {
        const CMatrix3x4& transform = WeatherMaskList[m].InvTransform;
        NEngineInternal::GPShaderRenderManager->SetUserVertexShaderConstant(index++, transform.E11, transform.E21, transform.E31, transform.E41);
        NEngineInternal::GPShaderRenderManager->SetUserVertexShaderConstant(index++, transform.E12, transform.E22, transform.E32, transform.E42);
        NEngineInternal::GPShaderRenderManager->SetUserVertexShaderConstant(index++, transform.E13, transform.E23, transform.E33, transform.E43);
        NEngineInternal::GPShaderRenderManager->SetUserVertexShaderConstant(index++, WeatherMaskList[m].BBox.PeekMinCoord(), 0.0f);
        NEngineInternal::GPShaderRenderManager->SetUserVertexShaderConstant(index++, WeatherMaskList[m].BBox.PeekMaxCoord(), 0.0f);
    }
}

// 0x00B52DD0 (same TU, row of this chunk): out of line, as retail calls it.
#ifdef _MSC_VER
__declspec(noinline)
#endif
CEngineWeatherRenderer::EWeatherBoxVisible CEngineWeatherRenderer::ClipBoxToLandscape(const C3DVector& pos) const
{
    float halfX = Settings.ParticleBoxSpan.X * 0.5f;
    float halfY = Settings.ParticleBoxSpan.Y * 0.5f;
    float top = pos.Z + Settings.ParticleBoxSpan.Z * 0.5f;
    {
        long x = GFFloatToLongFloor(pos.X - halfX);
        long y = GFFloatToLongFloor(pos.Y - halfY);
        if (!g_pEngineWorldMap->MapHasDataAtPoint(x, y) || g_pEngineWorldMap->PeekLandscapeHeight(x, y) < top)
            return WEATHER_BOX_VISIBLE;
    }
    {
        long x = GFFloatToLongFloor(pos.X - halfX);
        long y = GFFloatToLongCeil(pos.Y + halfY);
        if (!g_pEngineWorldMap->MapHasDataAtPoint(x, y) || g_pEngineWorldMap->PeekLandscapeHeight(x, y) < top)
            return WEATHER_BOX_VISIBLE;
    }
    {
        long x = GFFloatToLongCeil(pos.X + halfX);
        long y = GFFloatToLongFloor(pos.Y - halfY);
        if (!g_pEngineWorldMap->MapHasDataAtPoint(x, y) || g_pEngineWorldMap->PeekLandscapeHeight(x, y) < top)
            return WEATHER_BOX_VISIBLE;
    }
    {
        long x = GFFloatToLongCeil(pos.X + halfX);
        long y = GFFloatToLongCeil(pos.Y + halfY);
        if (!g_pEngineWorldMap->MapHasDataAtPoint(x, y) || g_pEngineWorldMap->PeekLandscapeHeight(x, y) < top)
            return WEATHER_BOX_VISIBLE;
    }
    return WEATHER_BOX_CLIPPED;
}

void CEngineWeatherRenderer::RenderRain(const CEngineComponentRenderParams& params)
{
    CRenderStateScopedCaptureBlock capture;
    CVolumeTexture* texture;
    if (!NEngineInternal::GPResourceManager->PeekGraphicBank((EGraphicBankNo)0)->PeekGraphic(g_pEngineEnvironmentDef->RainTexturePC)->GetVolumeTexture(&texture, 0))
        return;
    NEngineInternal::GPRenderManager->AttachVolumeTextureToStage(0, texture);
    NEngineInternal::GPRenderManager->SelectVertexBuffer(0, &RainVertexBuffer);
    NEngineInternal::GPRenderManager->SelectIndexBuffer(&RainIndexBuffer, 0);

    const CMatrix3x4& view = NEngineInternal::GPCamera->PeekNonHomogenousViewTransform();
    NEngineInternal::GPShaderRenderManager->SetUserVertexShaderConstant(1, view.E11, view.E21, view.E31, view.E41);
    NEngineInternal::GPShaderRenderManager->SetUserVertexShaderConstant(2, view.E12, view.E22, view.E32, view.E42);
    NEngineInternal::GPShaderRenderManager->SetUserVertexShaderConstant(3, view.E13, view.E23, view.E33, view.E43);
    const CMatrix3x4& inverse = NEngineInternal::GPCamera->PeekInverseNonHomogenousViewTransform();
    NEngineInternal::GPShaderRenderManager->SetUserVertexShaderConstant(4, inverse.E11, inverse.E21, inverse.E31, inverse.E41);
    NEngineInternal::GPShaderRenderManager->SetUserVertexShaderConstant(5, inverse.E12, inverse.E22, inverse.E32, inverse.E42);
    NEngineInternal::GPShaderRenderManager->SetUserVertexShaderConstant(6, inverse.E13, inverse.E23, inverse.E33, inverse.E43);
    NEngineInternal::GPShaderRenderManager->SetUserVertexShaderConstant(7, Settings.RainDropSize.X,
        (Settings.RainDropSize.Y - Settings.RainDropMinSize.Y) * Settings.CurrentRainStrength + Settings.RainDropMinSize.Y,
        Settings.RainFadeEnd / (Settings.RainFadeStart - Settings.RainFadeEnd) + 1.0f,
        1.0f / (Settings.RainFadeEnd - Settings.RainFadeStart));
    long constant = 9;
    NEngineInternal::GPShaderRenderManager->SetUserVertexShaderConstant(8, Settings.WindVector.X * Settings.RainWindSpeedMultiplier,
        Settings.WindVector.Y * Settings.RainWindSpeedMultiplier, Settings.WindVector.Z * Settings.RainWindSpeedMultiplier, Settings.RainTimePassed);
    SetCommonParticleShaderConstants(constant);
    for (long set = 0; set < 5; ++set)
        NEngineInternal::GPShaderRenderManager->SetUserVertexShaderConstant(constant++, Settings.RainWindFactors[(unsigned long)set % 5],
            Settings.RainSpeedFactors[(unsigned long)set % 5], Settings.RainMaxAlpha, 0.0f);

    float max_z = Settings.ParticleBoxSpan.Z * 5.0f;
    CArray<C3DVector> boxes;
    CArray<C3DVector> clipped_boxes;
    long level = 1;
    for (float z = Settings.ParticleBoxSpan.Z * -4.0f; z < max_z; z += Settings.ParticleBoxSpan.Z, ++level) {
        for (long i = 0; i < 81; ++i) {
            C3DVector pos(Settings.ParticleBoxPos[i].X, Settings.ParticleBoxPos[i].Y, Settings.ParticleBoxPos[i].Z + z);
            float radius = Settings.RainFadeEnd;
            EWeatherBoxVisible& visible = SavedBoxVisibility[i * level];
            if (!Settings.ParticleBoxLocked)
                visible = ClipBoxToCamera(pos, radius);
            if (visible != WEATHER_BOX_VISIBLE)
                continue;
            if (ClipBoxToLandscape(pos) != WEATHER_BOX_VISIBLE)
                continue;
            if (ClipBoxToWeatherMaskSphere(pos) == WEATHER_BOX_CLIPPED)
                clipped_boxes.push_back(pos);
            else
                boxes.push_back(pos);
        }
    }

    NEngineInternal::GPShaderRenderManager->ApplyPixelShader(RainParticlePixelShader);
    NEngineInternal::GPShaderRenderManager->ApplyVertexShader(RainParticleVertexShader, false);
    long count = boxes.size();
    for (long b = 0; b < count; ++b)
        RenderRainBox(boxes[b]);
    NEngineInternal::GPShaderRenderManager->ApplyVertexShader(RainParticleClippingVertexShader, false);
    long clipped_count = clipped_boxes.size();
    for (long c = 0; c < clipped_count; ++c)
        RenderRainBox(clipped_boxes[c]);
}