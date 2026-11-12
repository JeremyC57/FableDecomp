// CEngineSkyRenderer::RenderSunFlare() -- draw the sun's lens flare: inside a render-state capture block read last frame's sun occlusion result (give up when it is negative or below 1/255, clamp it to 1), set additive-colour blending with z-buffering off, project the sun direction (2000 units from the camera) to the screen, give up when it is behind the camera, then for each lens-flare element of the sky def (until one without a texture) draw its texture as a screen-space sprite placed along the line from the screen centre through the sun, tinted by the element colour, theme colour 11, the visibility and the distance ratio (an unloaded texture is requested and skipped), and finally the sun-flare sprite itself at the sun (theme colour 10 times the visibility)
// Grade: RELOCATION_MATCH (2631v2631, session 3 grade 46). Shape notes (each one closed a residue, grades 34..46 in grades.log):
//   - the final sun sprite sits in its own `if (sun_texture != 0) { }` scope: its CGraphicFrame then shares the loop frame's slot
//     ([esp+20h], frame 0x7C not 0x80) and VC7.1 gives each of its three exits (two BeginLoading arms + the end) its own
//     RestoreCaptureBlock epilogue as retail does;
//   - the flare ratio is `C2DVector(screen - centre).GetMagnitude() / C2DVector(centre).GetMagnitude()` on unnamed temporaries, and the
//     sprite position repeats `(screen.X - centre_x) * position + centre_x`: the shared difference is a CSE kept on the x87 stack
//     (`fst [esp+30h] ; .. fld st(0) ; fmul st(1) ; fld st(2) ; fmul st(3) ; faddp ; fsqrt ; fstp st(2) ; fstp st(0)`);
//   - TransformWorldToView expands CMatrix3x4::TransformPoint in the FABLE_RETAIL_CMATRIX3X4_TRANSFORMPOINT_SUMS form (X row
//     E21*y+E31*z first, Y / Z rows left to right; 0x00B647EC..0x00B6485C);
//   - the unloaded-texture test is an if/else around the frame (a `continue` left the frame's zero store live in the loop).
// Leaf: ?RenderSunFlare@CEngineSkyRenderer@@AAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00B64550, 2631 bytes, thiscall, `ret`, 0x7C-byte frame, no EH frame; ends at 0x00B64F96 (RenderClouds 0x00B64FA0).
// Identity: FableWin.pdb ?RenderSunFlare@CEngineSkyRenderer@@AAEXXZ (debug COMDAT 0x02D42EC0, followed by TransformViewToScreen /
//   TransformWorldToView = first used here) + Ego_r.pdb 0x00A17F00.
// Callees: CEngineOcclusionManager::GetPrevFrameFlareTestResultFromRetrivalID 0x00B8D3C0, GFVXToAX 0x009E1E10, GFVYToAY 0x009E1E40,
//   CGraphicInfoFrame::BeginLoading(unsigned long, unsigned long) const 0x009FE620, CGraphicInfo::GetFrame(CGraphicFrame&, 3 x unsigned long) const
//   0x009FF8D0, __ftol2 0x00BFEA70, CEngineSkyRenderer::RenderScreenSpaceSprite 0x00B63530, CGraphicFrameData::Uninitialise 0x009FCBA0,
//   operator delete 0x00BFE9BC, CRenderStateManager::RestoreCaptureBlock 0x00A05840.
// Globals: g_pOcclusionManager 0x01436E5C, g_pEngineLightingManager 0x01436E9C, g_pEngineCamera 0x01436EA0 (bound spellings; no PDB data names),
//   g_pEngineWorldMap 0x01436E8C, NEngineInternal::GPResourceManager 0x01436E98, GPRenderManager 0x01436E18.
#include "fable_compiler.h"
// Shared helper bodies (session 3): the FableWin-sized helper opt-ins below are the set RenderClouds 0x00B64FA0's best source uses, so the
// three CEngineSkyRenderer rows see one body per helper; this row matches with or without them (grades 48 and 98).
#define FABLE_RETAIL_CRENDERSTATECAPTUREBLOCK_INLINE
#define FABLE_RETAIL_CGRAPHICFRAME_RELEASEDATA
#define FABLE_RETAIL_CGRAPHICINFOFRAME_TOUCHFRAME
#define FABLE_RETAIL_CGRAPHICINFOFRAME_GETBANK_DIRECT
#define FABLE_RETAIL_CRESOURCEBANK_CURRENTFRAME_DIRECT
#define FABLE_RETAIL_CFRAMEDATA_ISLOADING
#define FABLE_RETAIL_CFRAMEDATA_ISLOADING_ISNULL
#define FABLE_RETAIL_CIVCOUNTEDPOINTER_GET
#define FABLE_RETAIL_CMANAGEDTEXTURE_TOUCHTEXTURE_GET
#define FABLE_RETAIL_CSHADERRENDERMANAGER_PSCONST_INDEX
#define FABLE_RETAIL_C2DVECTOR
#define FABLE_RETAIL_C4DVECTOR
#define FABLE_RETAIL_CARRAY
#define FABLE_RETAIL_CENGINECOMPONENT
#define FABLE_RETAIL_CENGINESKYRENDERER
#define FABLE_RETAIL_CENGINESKYRENDERER_RENDERSUNFLARE
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
#define FABLE_RETAIL_CGRAPHICINFOFRAME_BEGINLOADING
#define FABLE_RETAIL_CGRAPHICINFO_GETFRAME3
#define FABLE_RETAIL_CRENDERMANAGERCORE
#define FABLE_RETAIL_CTEXTURE
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CENGINEWORLDMAP
#define FABLE_RETAIL_CENGINEWORLDMAP_GAMEWORLD
#define FABLE_RETAIL_CENGINECAMERA_POSITION
#define FABLE_RETAIL_CENGINECAMERA_VIEWTOSCREEN
#define FABLE_RETAIL_CENGINELIGHTINGMANAGER
#define FABLE_RETAIL_CENGINELIGHTINGMANAGER_PEEKSUNVECTOR
#define FABLE_RETAIL_CENGINEOCCLUSIONMANAGER
#define FABLE_RETAIL_CENGINEOCCLUSIONMANAGER_FLARE
#define FABLE_RETAIL_CMATRIX3X4
#define FABLE_RETAIL_CMATRIX3X4_TRANSFORMPOINT_SUMS
#define FABLE_RETAIL_CRGBFLOATCOLOUR
#include <math.h>
#include "fable_string.h"
#include "engine/C2DVector.h"
#include "engine/C3DVector.h"
#include "engine/C4DVector.h"
#include "engine/CRGBColour.h"
#include "engine/CRGBFloatColour.h"
#include "engine/CEngineSkyRenderer.h"
#include "engine/CEngineResourceManager.h"
#include "engine/CGraphicInfo.h"
#include "engine/CRenderManager.h"
#include "engine/CShaderRenderManager.h"
#include "engine/CRenderStateCaptureBlock.h"
#include "engine/CIDrawWorld.h"
#include "engine/CIDrawEnvironment.h"
#include "engine/CEngineWorldMap.h"
#include "engine/CEngineCamera.h"
#include "engine/CEngineLightingManager.h"
#include "engine/CEngineOcclusionManager.h"
#include "engine/CSkyDef.h"

namespace NEngineInternal { extern CEngineResourceManager* GPResourceManager; }        // 0x01436E98
namespace NEngineInternal { extern CRenderManager* GPRenderManager; }                  // 0x01436E18
extern CEngineWorldMap* g_pEngineWorldMap;                                            // 0x01436E8C
extern CEngineOcclusionManager* g_pOcclusionManager;                                  // 0x01436E5C
extern CEngineLightingManager* g_pEngineLightingManager;                                    // 0x01436E9C
extern CEngineCamera* g_pEngineCamera;                                                // 0x01436EA0
float __fastcall GFVXToAX(float x);                                                   // 0x009E1E10
float __fastcall GFVYToAY(float y);                                                   // 0x009E1E40

FABLE_FORCEINLINE const CGraphicInfo* CGraphicDataBank::PeekGraphic(unsigned long index) const
{
    return index < GraphicList.size() ? &GraphicList[index] : 0;
}
FABLE_FORCEINLINE const CGraphicInfoFrame* CGraphicDataBank::PeekGraphicInfoFrame(unsigned long graphic, unsigned long frame) const
{
    return PeekGraphic(graphic) ? PeekGraphic(graphic)->PeekInfoFrame(frame) : 0;
}

void CEngineSkyRenderer::RenderSunFlare()
{
    CRenderStateScopedCaptureBlock capture;
    const CIDrawEnvironmentParams& params = g_pEngineWorldMap->PeekGameWorld()->DrawGetEnvironment()->PeekInterpolatedParams();
    float visibility = g_pOcclusionManager->GetPrevFrameFlareTestResultFromRetrivalID(0x1000000);
    if (visibility < 0.0f)
        return;
    if (visibility > 1.0f)
        visibility = 1.0f;
    else if (visibility < 1.0f / 255.0f)
        return;
    C3DVector sun = g_pEngineLightingManager->PeekSunVector();
    NEngineInternal::GPRenderManager->SetSourceAlphaBlendMode(ALPHA_BLEND_ONE);
    NEngineInternal::GPRenderManager->SetDestAlphaBlendMode(ALPHA_BLEND_INV_SRC_COLOR);
    NEngineInternal::GPRenderManager->SetAlphaBlendEnable(true);
    NEngineInternal::GPRenderManager->SetZBufferEnable(false);
    C3DVector screen = sun * 2000.0f + g_pEngineCamera->PeekCameraPos();
    g_pEngineCamera->TransformWorldToView(screen);
    if (screen.Z > 0.01) {
        g_pEngineCamera->TransformViewToScreen(screen);
        float centre_x = GFVXToAX(32768.0f);
        float centre_y = GFVYToAY(32768.0f);
        long count = PSkyDef->FlareElements.size();
        for (long i = 0; i < count; ++i) {
            const CLensFlareElementDef& element = PSkyDef->FlareElements[i];
            long texture_id = element.Texture;
            if (texture_id == 0)
                break;
            const CGraphicDataBank* bank = NEngineInternal::GPResourceManager->PeekGraphicBank((EGraphicBankNo)0);
            if (!bank->PeekGraphicInfoFrame(texture_id, 0)->IsFrameDataLoaded()) {
                bank->PeekGraphicInfoFrame(texture_id, 0)->BeginLoading(0, 0);
            } else {
                CGraphicFrame frame;
                if (NEngineInternal::GPResourceManager->PeekGraphicBank((EGraphicBankNo)0)->PeekGraphic(texture_id)->GetFrame(frame, 0, 0, 1)) {
                    const CTexture* texture = frame.GetTexture();
                    long size = (long)GFVXToAX(PSkyDef->FlareElements[i].Radius);
                    float ratio = C2DVector(screen.X - centre_x, screen.Y - centre_y).GetMagnitude() / C2DVector(centre_x, centre_y).GetMagnitude();
                    ratio = ratio < 0.0f ? 0.0f : (ratio > 1.0f ? 1.0f : ratio);
                    CRGBFloatColour colour = CRGBFloatColour(PSkyDef->FlareElements[i].Colour) * params.ThemeChannels[0].Colours[11] * visibility * ratio;
                    float position = PSkyDef->FlareElements[i].Position;
                    float x = (screen.X - centre_x) * position + centre_x;
                    float y = (screen.Y - centre_y) * position + centre_y;
                    RenderScreenSpaceSprite(texture, x - size, y - size, (float)(size * 2), (float)(size * 2), colour.GetRGBColour());
                }
            }
        }
        long sun_texture = PSkyDef->SunFlareTexture;
        if (sun_texture != 0) {
            const CGraphicDataBank* bank = NEngineInternal::GPResourceManager->PeekGraphicBank((EGraphicBankNo)0);
            if (!bank->PeekGraphicInfoFrame(sun_texture, 0)->IsFrameDataLoaded()) {
                bank->PeekGraphicInfoFrame(sun_texture, 0)->BeginLoading(0, 0);
                return;
            }
            CGraphicFrame frame;
            if (NEngineInternal::GPResourceManager->PeekGraphicBank((EGraphicBankNo)0)->PeekGraphic(sun_texture)->GetFrame(frame, 0, 0, 0)) {
                const CTexture* texture = frame.GetTexture();
                long size = (long)GFVXToAX((float)PSkyDef->SunFlareRadius);
                CRGBFloatColour colour = params.ThemeChannels[0].Colours[10] * visibility;
                RenderScreenSpaceSprite(texture, screen.X - size, screen.Y - size, (float)(size * 2), (float)(size * 2), colour.GetRGBColour());
            }
        }
    }
}
