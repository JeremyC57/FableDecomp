// CEngineSkyRenderer::RenderScreenSpaceSprite(const CTexture* texture, float x, float y, float width, float height, CRGBColour colour) -- draw a screen-space sky sprite: convert the rectangle's corners from screen to homogenous space through the engine camera, build four pre-transformed vertices at Z 0.9999 and draw the quad with the sprite shaders
// Leaf: ?RenderScreenSpaceSprite@CEngineSkyRenderer@@AAEXPBVCTexture@@MMMMVCRGBColour@@@Z
// Flags: /GS
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B63530, `ret 18h`, /GS cookie (0x0139C8A8, check 0x00BFE9F9).
// Identity: FableWin.pdb ?RenderScreenSpaceSprite@CEngineSkyRenderer@@AAEXPBVCTexture@@MMMMVCRGBColour@@@Z (four floats, private);
//   engine/CEngineSkyRenderer.h RENDERSUNFLARE declares it; RenderSunFlare 0x00B64550 calls it.
// Callees: CEngineCamera::TransformScreenTo2DHomogenousSpace 0x00B2FB70 (FableWin.pdb) on NEngineInternal::GPCamera 0x01436EA0,
//   CShaderRenderManager::ApplyVertexShader 0x00988020 / ApplyPixelShader 0x00988140 and the inline PrepareForRender(0) on
//   NEngineInternal::GPShaderRenderManager 0x01436E14, CRenderManagerCore::DrawAsIndexedTris 0x00A0AEA0 on GPRenderManager 0x01436E18.
// Layout: engine/CEngineSkyRenderer.h (ScreenSpaceSpriteVertexShader +0x140, ScreenSpaceSpritePixelShader +0x148); the vertex is
//   CTVertexRHWColSpecTex1 (0x20 bytes; the specular word is left unset).
#define FABLE_RETAIL_CIVCOUNTEDPOINTER_RELEASE_ISNULL
#define FABLE_RETAIL_CIVCOUNTEDPOINTEEBASE_RELEASE_COUNT
#define FABLE_RETAIL_CRENDERSTATEMANAGER_SETRENDERSTATE_DESIRED
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
#define FABLE_RETAIL_CENGINECAMERA_SCREENTOHOMOGENOUS
#define FABLE_RETAIL_CRENDERMANAGERCORE_DRAWASINDEXEDTRIS
#include "fable_compiler.h"
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
#include "engine/CTVertexRHWColSpecTex1.h"

namespace NEngineInternal { extern CRenderManager* GPRenderManager; }                  // 0x01436E18
namespace NEngineInternal { extern CShaderRenderManager* GPShaderRenderManager; }      // 0x01436E14
namespace NEngineInternal { extern CEngineCamera* GPCamera; }                          // 0x01436EA0

void CEngineSkyRenderer::RenderScreenSpaceSprite(const CTexture* texture, float x, float y, float width, float height, CRGBColour colour)
{
    C2DVector bottom_right(width + x, height + y);
    unsigned long d3d_colour = (((((unsigned long)colour.GetA() << 8) + colour.GetR()) << 8) + colour.GetG() << 8) + colour.GetB();
    C2DVector top_left(x, y);
    NEngineInternal::GPCamera->TransformScreenTo2DHomogenousSpace(top_left);
    NEngineInternal::GPCamera->TransformScreenTo2DHomogenousSpace(bottom_right);
    CTVertexRHWColSpecTex1 vertices[4];
    vertices[0].X = top_left.X;
    vertices[0].Y = top_left.Y;
    vertices[0].Z = 0.9999f;
    vertices[0].U1 = 0.0f;
    vertices[0].V1 = 0.0f;
    vertices[0].DiffuseColour = d3d_colour;
    vertices[0].RHW = 1.0f;
    vertices[1].X = bottom_right.X;
    vertices[1].Y = top_left.Y;
    vertices[1].Z = 0.9999f;
    vertices[1].U1 = 1.0f;
    vertices[1].V1 = 0.0f;
    vertices[1].DiffuseColour = d3d_colour;
    vertices[1].RHW = 1.0f;
    vertices[2].X = bottom_right.X;
    vertices[2].Y = bottom_right.Y;
    vertices[2].Z = 0.9999f;
    vertices[2].U1 = 1.0f;
    vertices[2].V1 = 1.0f;
    vertices[2].DiffuseColour = d3d_colour;
    vertices[2].RHW = 1.0f;
    vertices[3].X = top_left.X;
    vertices[3].Y = bottom_right.Y;
    vertices[3].Z = 0.9999f;
    vertices[3].U1 = 0.0f;
    vertices[3].V1 = 1.0f;
    vertices[3].DiffuseColour = d3d_colour;
    vertices[3].RHW = 1.0f;
    unsigned short indices[6];
    indices[0] = 0;
    indices[1] = 1;
    indices[2] = 2;
    indices[3] = 0;
    indices[4] = 2;
    indices[5] = 3;
    NEngineInternal::GPShaderRenderManager->ApplyVertexShader(ScreenSpaceSpriteVertexShader, false);
    NEngineInternal::GPShaderRenderManager->ApplyPixelShader(ScreenSpaceSpritePixelShader);
    NEngineInternal::GPShaderRenderManager->PrepareForRender(0);
    NEngineInternal::GPRenderManager->DrawAsIndexedTris(&texture, 1, vertices, sizeof(CTVertexRHWColSpecTex1), 4, indices, 2);
}
