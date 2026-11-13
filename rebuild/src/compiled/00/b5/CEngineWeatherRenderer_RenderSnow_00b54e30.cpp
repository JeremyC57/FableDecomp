// CEngineWeatherRenderer::RenderSnow(const CEngineComponentRenderParams& params) -- draw the snow particle boxes: inside a render-state capture block, fetch frame 0 of the environment's snow texture (requesting the load and drawing nothing while it is not loaded), bind it on stage 3 with the snow vertex buffer, point sprites and point scaling on, upload the clamped scaled current wind times the snow wind speed with the snow time (constant 1), the snow fade terms (2), the viewport-height flake size and the flake scale distance factors (3) and limits (4), the common particle constants and the five per-set snow wind / speed factors, then walk the 81 particle box columns over nine height levels (each box clipped against the camera unless the boxes are locked, then against the landscape; boxes inside a weather mask sphere go to the clipping list) and render the plain boxes with the snow particle vertex shader and the masked ones with the clipping vertex shader
// Pair-landing (chunk 2037): the same TU now holds the three clip bodies retail calls -- ClipBoxToCamera 0x00B51F60 and
//   ClipBoxToWeatherMaskSphere 0x00B52090 (ported onto the shared headers from the landed CWeatherRendererMethods::ClipBoxToCamera /
//   CEngineWeatherRenderer::IsPointInPrecipitation; each compiled out of line grades RELOCATION_MATCH against its retail bytes) and
//   ClipBoxToLandscape 0x00B52DD0 (this chunk; the same body text as the matched src/00b52dd0.cpp, which grades RELOCATION_MATCH at
//   /d2inlT507; here it is compiled at the TU's default budget) -- each __declspec(noinline) as retail calls them. With their side effects visible VC7.1
//   schedules the two inline push_back C3DVector copies load-ahead as retail (0x00B553BD..0x00B553CF, 0x00B55403..0x00B55414); the 2031/2008 hand-backs
//   graded DIFFER only there without them.
// Retail-asm: same-TU callee body only (this row's own leaf has no asm): ClipBoxToLandscape 0x00B52DD0 expands the shared helpers GFFloatToLongFloor (0x00B52E22 `fld [v] ; fsub [half] ; fistp dword ptr [r] ; fild [r] ; fadd [one] ; fstp [n] ; mov eax,[v] ; cmp eax,[n] ; jne ; inc dword ptr [r]`), GFFloatToLongCeil (0x00B532A3 `fld ; fadd ; fistp dword ptr ; fild ; fsub ; fstp ; mov ; cmp ; jne ; dec`) and GFFloatToLongNear (0x00B53231 `fld [v] ; fistp dword ptr [r]`); each helper header holds the proof that VC7.1 emits no bare dword fistp from C++.
// Leaf: ?RenderSnow@CEngineWeatherRenderer@@AAEXABVCEngineComponentRenderParams@@@Z
// Inline-budget: FABLE_FORCEINLINE Lionhead header helpers (owner 2026-11-01 rule: retail copies uncalled or absent), measured with the c2
//   inline trace (tools/inl.sh, default threshold 74): CGraphicDataBank::PeekGraphicInfoFrame (c1 102, score 102 at f 1; retail copy 0x0041C1F0
//   uncalled), CGraphicInfoFrame::GetFrame (overlay; retail copy 0x0041C260 uncalled), CManagedTexture::TouchTexture (c1 56, score 133;
//   copy 0x0041BB70 uncalled), CResource::TouchResource (c1 51, score 199; copy 0x0041B9C0 uncalled), ~CGraphicFrame (c1 88, score 88;
//   ~CFrameData 0x009FE6C0 calls UnInitialise yet expands the member destructor), CEngineEnvironment::GetClampedScaledCurrentWindVector
//   (c1 73, score 83 at f 0.5) -- with them every other inline decision of the trace is r=1 as retail.
// Retail: Fable.exe (TLC, VC7.1) @ 0x00B54E30, 1901 bytes (the extent is right: prologue at the function start after RenderRain's int3
//   padding, last instruction `ret 4` at 0x00B5559A, int3 padding after), thiscall, `ret 4`, 0x44-byte frame, no EH frame.
// Identity (two sources): Ego_r.pdb 0x009FB0F0 / FableWin.pdb ?RenderSnow@CEngineWeatherRenderer@@AAEXABVCEngineComponentRenderParams@@@Z
//   (private), Ego_r donor order RenderRain 0x009FAAD0 -> RenderSnow 0x009FB0F0 = retail 0x00B54890 -> 0x00B54E30; the body renders
//   through RenderSnowBox 0x00B54210 (Ego_r ?RenderSnowBox@CEngineWeatherRenderer@@AAEXABVC3DVector@@@Z, landed) and reads Settings.Snow*.
// Callees: CFrameData::BeginLoading 0x009FDE80 / CFrameData(const CGraphicInfoFrame*) 0x009FE380, CResource::ListRemove 0x009FBFB0 /
//   ListInsert 0x009FBF90, CGraphicFrameData::Uninitialise 0x009FCBA0, operator new 0x00BFEA1A / delete 0x00BFE9BC (the expanded
//   GetFrame / ~CGraphicFrame), CShaderRenderManager::SetUserVertexShaderConstant 0x00989B00 / ApplyPixelShader 0x00988140 /
//   ApplyVertexShader 0x00988020, SetCommonParticleShaderConstants 0x00B52160, ClipBoxToCamera 0x00B51F60, ClipBoxToLandscape 0x00B52DD0,
//   ClipBoxToWeatherMaskSphere 0x00B52090, STLport vector<C3DVector>::_M_insert_overflow 0x0066E370, RenderSnowBox 0x00B54210,
//   free 0x00BFEA14, CRenderStateManager::RestoreCaptureBlock 0x00A05840; D3D device SetTexture (+0x104) / SetStreamSource (+0x190).
// Globals: NEngineInternal::GPResourceManager 0x01436E98 (graphic bank 0, +0x954), GPRenderManager 0x01436E18, GPShaderRenderManager
//   0x01436E14, g_pEngineEnvironmentDef 0x01436E24 (SnowTexture +0xFC), g_pEngineEnvironment_01436e48 0x01436E48 (CEngineEnvironment;
//   established spelling of CEngine::Initialise 0x00B26360), g_pDisplayManager_01436e1c 0x01436E1C (ViewportBox +0x1F4), the capture-block
//   state manager 0x013CB508. All four NEngineInternal / g_ spellings are the established (bound) ones; no PDB data symbol exists.
// Shape: the twin of RenderRain 0x00B54890 (c2008 differ/00b54890.cpp: same box walk, same SetCommonParticleShaderConstants body visible
//   and noinline so `constant` stays in ebp). The texture index is read into a named local before the bank (`long texture = ...`): c2
//   then generates the def load first (rotation eax) and the GPResourceManager load second (ecx), retail's 0x00B54E60..0x00B54E71
//   allocation; inline in the call it is generated after the bank (eax / edx). The frame-fetch is the shared GetFrame (overlay
//   engine/CGraphicInfo.h; compiled out of line it grades RELOCATION_MATCH against retail's uncalled copy 0x0041C260). The frame's
//   destructor runs on the success path only: after `frame.GetTexture()` dereferenced the copied frame data, VC7.1 drops its null test
//   (0x00B55542) because the frame never escapes.
#include "fable_compiler.h"
#define FABLE_RETAIL_CENGINECOMPONENT
#define FABLE_RETAIL_CENGINEWEATHERRENDERER
#define FABLE_RETAIL_CENGINEWEATHERRENDERER_BUFFERS
#define FABLE_RETAIL_CENGINEWEATHERRENDERER_RENDERRAIN
#define FABLE_RETAIL_CENGINEWEATHERRENDERER_RENDERSNOW
#define FABLE_RETAIL_CENGINEWEATHERSETTINGS_TAIL
#define FABLE_RETAIL_CENGINERESOURCEMANAGER
#define FABLE_RETAIL_STATICBLOCKS_TEXTURE_HELPERS
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
#define FABLE_RETAIL_CRENDERMANAGERCORE
#define FABLE_RETAIL_CRENDERMANAGERCORE_FORCEINLINE_HELPERS
#define FABLE_RETAIL_CRENDERSTATEMANAGER_POINTSPRITE_INLINE
#define FABLE_RETAIL_CTEXTURE
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CENVIRONMENTDEF_PARENT
#define FABLE_RETAIL_CENGINEENVIRONMENT
#define FABLE_RETAIL_CENGINEENVIRONMENT_WIND
#define FABLE_RETAIL_CDISPLAYMANAGER
#define FABLE_RETAIL_SCOPED_PTR
#define FABLE_RETAIL_CDISPLAYMANAGER_GETVIEWPORT
#define FABLE_RETAIL_C2DBOXI_GETHEIGHT
#define FABLE_RETAIL_C3DBOUNDINGBOX
#define FABLE_RETAIL_C3DBOUNDINGSPHERE_CENTER_VALUE
#define FABLE_RETAIL_CENGINECAMERA_POSITION
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
#include "engine/CRenderManager.h"
#include "engine/CShaderRenderManager.h"
#include "engine/CRenderStateCaptureBlock.h"
#include "engine/CEngineEnvironment.h"
#include "engine/Win32Types.h"
#include "engine/Direct3D9Types.h"
#include "engine/CDisplayManager.h"
#include "engine/CEnvironmentDef.h"
#include "engine/CArray.h"
#include "engine/CEngineCamera.h"
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
extern const CEnvironmentDef* g_pEngineEnvironmentDef;                                // 0x01436E24
extern CEngineEnvironment* g_pEngineEnvironment_01436e48;                             // 0x01436E48
extern CDisplayManager* g_pDisplayManager_01436e1c;                                   // 0x01436E1C

inline const CGraphicInfo* CGraphicDataBank::PeekGraphic(unsigned long index) const
{
    return index < GraphicList.size() ? &GraphicList[index] : 0;
}
FABLE_FORCEINLINE const CGraphicInfoFrame* CGraphicDataBank::PeekGraphicInfoFrame(unsigned long graphic, unsigned long frame) const
{
    return PeekGraphic(graphic) ? PeekGraphic(graphic)->PeekInfoFrame(frame) : 0;
}

namespace NEngineInternal { extern CEngineCamera* GPCamera; }                          // 0x01436EA0

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

// 0x00B52160 (same TU, compiled before RenderSnow; landed under a stand-in spelling): visible but kept out of line as retail calls it, so
// VC7.1 knows the index it steps through the reference is not captured (RenderSnow keeps it in ebp after the call, 0x00B5528A).
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

void CEngineWeatherRenderer::RenderSnow(const CEngineComponentRenderParams& params)
{
    CRenderStateScopedCaptureBlock capture;
    CGraphicFrame frame;
    long texture = g_pEngineEnvironmentDef->SnowTexture;
    if (!NEngineInternal::GPResourceManager->PeekGraphicBank((EGraphicBankNo)0)->PeekGraphicInfoFrame(texture, 0)->GetFrame(frame, 0, 0))
        return;
    NEngineInternal::GPRenderManager->AttachTextureToStage(3, frame.GetTexture());
    NEngineInternal::GPRenderManager->SelectVertexBuffer(0, &SnowVertexBuffer);
    NEngineInternal::GPRenderManager->SetPointSpriteEnable(true);
    NEngineInternal::GPRenderManager->SetPointScaleEnable(true);

    C3DVector wind = g_pEngineEnvironment_01436e48->GetClampedScaledCurrentWindVector(Settings.WeatherWindSpeedMultiplier,
        Settings.WeatherWindMinSpeed, Settings.WeatherWindMaxSpeed);
    NEngineInternal::GPShaderRenderManager->SetUserVertexShaderConstant(1, wind.X * Settings.SnowWindSpeedMultiplier,
        wind.Y * Settings.SnowWindSpeedMultiplier, wind.Z * Settings.SnowWindSpeedMultiplier, Settings.SnowTimePassed);
    NEngineInternal::GPShaderRenderManager->SetUserVertexShaderConstant(2,
        Settings.SnowFadeEnd / (Settings.SnowFadeStart - Settings.SnowFadeEnd) + 1.0f,
        1.0f / (Settings.SnowFadeEnd - Settings.SnowFadeStart), Settings.SnowMaxAlpha, 0.0f);
    NEngineInternal::GPShaderRenderManager->SetUserVertexShaderConstant(3,
        (float)g_pDisplayManager_01436e1c->GetViewport().GetHeight() * Settings.SnowflakeBaseSize,
        Settings.SnowflakeScaleOffset, Settings.SnowflakeScaleDistanceFactor, Settings.SnowflakeScaleDistanceSqrFactor);
    long constant = 5;
    NEngineInternal::GPShaderRenderManager->SetUserVertexShaderConstant(4, Settings.SnowflakeScaleMin, Settings.SnowflakeScaleMax, 0.0f, 0.0f);
    SetCommonParticleShaderConstants(constant);
    for (long set = 0; set < 5; ++set)
        NEngineInternal::GPShaderRenderManager->SetUserVertexShaderConstant(constant++, Settings.SnowWindFactors[set % 5],
            Settings.SnowSpeedFactors[set % 5], 0.0f, 0.0f);

    float max_z = Settings.ParticleBoxSpan.Z * 5.0f;
    CArray<C3DVector> boxes;
    CArray<C3DVector> clipped_boxes;
    long level = 1;
    for (float z = Settings.ParticleBoxSpan.Z * -4.0f; z < max_z; z += Settings.ParticleBoxSpan.Z, ++level) {
        for (long i = 0; i < 81; ++i) {
            C3DVector pos(Settings.ParticleBoxPos[i].X, Settings.ParticleBoxPos[i].Y, Settings.ParticleBoxPos[i].Z + z);
            float radius = Settings.SnowFadeEnd;
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

    NEngineInternal::GPShaderRenderManager->ApplyPixelShader(SnowParticlePixelShader);
    NEngineInternal::GPShaderRenderManager->ApplyVertexShader(SnowParticleVertexShader, false);
    long count = boxes.size();
    for (long b = 0; b < count; ++b)
        RenderSnowBox(boxes[b]);
    NEngineInternal::GPShaderRenderManager->ApplyVertexShader(SnowParticleClippingVertexShader, false);
    long clipped_count = clipped_boxes.size();
    for (long c = 0; c < clipped_count; ++c)
        RenderSnowBox(clipped_boxes[c]);
}