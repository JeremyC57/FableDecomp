// CEngineWeatherRenderer::RenderSnow(const CEngineComponentRenderParams& params) -- draw the snow particle boxes
// Leaf: ?RenderSnow@CEngineWeatherRenderer@@AAEXABVCEngineComponentRenderParams@@@Z
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

long WeatherDiagnosticExternalTest(float x, float y, float z);   // DIAGNOSTIC: an unknown callee (memory side effects), no address escapes
#ifdef _MSC_VER
__declspec(noinline)
#endif
CEngineWeatherRenderer::EWeatherBoxVisible CEngineWeatherRenderer::ClipBoxToCamera(const C3DVector& pos, float radius) const
{
    return (EWeatherBoxVisible)WeatherDiagnosticExternalTest(pos.X, pos.Y, radius);   // DIAGNOSTIC STUB
}
#ifdef _MSC_VER
__declspec(noinline)
#endif
CEngineWeatherRenderer::EWeatherBoxVisible CEngineWeatherRenderer::ClipBoxToWeatherMaskSphere(const C3DVector& pos) const
{
    return (EWeatherBoxVisible)WeatherDiagnosticExternalTest(pos.Z, pos.Y, pos.X);   // DIAGNOSTIC STUB
}
#ifdef _MSC_VER
__declspec(noinline)
#endif
CEngineWeatherRenderer::EWeatherBoxVisible CEngineWeatherRenderer::ClipBoxToLandscape(const C3DVector& pos) const
{
    return (EWeatherBoxVisible)WeatherDiagnosticExternalTest(pos.X, pos.Y, pos.Z);   // DIAGNOSTIC STUB
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
