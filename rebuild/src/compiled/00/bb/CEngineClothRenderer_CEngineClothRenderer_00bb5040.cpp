// CEngineClothRenderer::CEngineClothRenderer() -- retail 0x00BB5040, 0x12D8 bytes (EngineClothRenderer.cpp).
// Builds the cloth renderer: base CEnginePrimitiveRenderUtil, five CArray<CVertexShader> tri-shader tables (the two colour-buffer
// shadow tables only when the pixel-shader version is >= ps_1_4 = 1400), six CVertexShader members, the index buffer and two
// CSimpleMemoryPool(0, 0x1000) buffers, and fetches every vertex shader by name from vertex shader bank 0 (resource manager +0x29C0).
// Callees: CBaseClassNonCopyable ctor 0x0099A2F0, CVertexShader ctor 0x00A62FB0, CIndexBuffer ctor 0x00AC1EC0,
// CSimpleMemoryPool ctor 0x00A5C570, vector<CVertexShader>::resize 0x00B6CBD0, CCharString ctor 0x0099EBF0 / dtor 0x0099EAE0,
// CShaderResource::GetVertexShader 0x00A5D5F0, CDisplayManager::GetPixelShaderVersion 0x009BEE00.
// Leaf: ??0CEngineClothRenderer@@QAE@XZ
#define FABLE_RETAIL_CENGINERESOURCEMANAGER
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CARRAY
#define FABLE_RETAIL_CVERTEXSHADER
#define FABLE_RETAIL_CINDEXBUFFER
#define FABLE_RETAIL_CMOVABLERESOURCE
#define FABLE_RETAIL_CSIMPLEMEMORYPOOL
#define FABLE_RETAIL_CDISPLAYMANAGER_PIXELSHADERVERSION
#define FABLE_RETAIL_CENGINECLOTHRENDERER_MEMBERS
#include "engine/CEnginePrimitiveRenderUtil.h"
#include "engine/CArray.h"
#include "engine/CVertexShader.h"
#include "engine/CIndexBuffer.h"
#include "engine/CSimpleMemoryPool.h"
#include "engine/CEngineClothRenderer.h"
#include "engine/CEngineResourceManager.h"
#include "engine/CShaderResource.h"
#include "engine/Direct3D9Types.h"
#include "engine/CDisplayManager.h"

class C4DVector;
namespace NEngineInternal {
extern CEngineResourceManager* GPResourceManager;   // 0x01436E98
extern CDisplayManager* GPDisplayManager;           // 0x01436E1C
}

CEngineClothRenderer::CEngineClothRenderer()
    : IndexBufferSize(-1)
    , PositionBufferPool(0, 0x1000)
    , NormalBufferPool(0, 0x1000)
{
    PositionBuffer = 0;
    PositionBufferSize = 0;
    NormalBuffer = 0;
    NormalBufferSize = 0;
    const CShaderResource* shaders = NEngineInternal::GPResourceManager->GetVShaderResource((EShaderBankNo)0);

    TriVertexShaders.resize(6);
    TriVertexShaders[0] = shaders->GetVertexShader("VSHADER_STATIC_DIRLIGHT_FOG");
    TriVertexShaders[1] = shaders->GetVertexShader("VSHADER_STATIC_DIRLIGHT_2POINTLIGHTS_FOG");
    TriVertexShaders[2] = shaders->GetVertexShader("VSHADER_STATIC_DIRLIGHT_2POINTLIGHTS_FOG");
    TriVertexShaders[3] = shaders->GetVertexShader("VSHADER_STATIC_DIRLIGHT_4POINTLIGHTS_FOG");
    TriVertexShaders[4] = shaders->GetVertexShader("VSHADER_STATIC_DIRLIGHT_4POINTLIGHTS_FOG");
    TriVertexShaders[5] = shaders->GetVertexShader("VSHADER_STATIC_DIRLIGHT_5POINTLIGHTS_FOG");
    TriVertexShaders2D = shaders->GetVertexShader("VSHADER_2D_STATIC_DIRLIGHT");
    TriBackfaceVertexShaders2D = shaders->GetVertexShader("VSHADER_2D_STATIC_DIRLIGHT_BACKFACE");

    TriVertexShadersDepthBufferShadow.resize(6);
    TriVertexShadersDepthBufferShadow[0] = shaders->GetVertexShader("VSHADER_STATIC_DIRLIGHT_FOG_SHADOW");
    TriVertexShadersDepthBufferShadow[1] = shaders->GetVertexShader("VSHADER_STATIC_DIRLIGHT_2POINTLIGHTS_FOG_SHADOW");
    TriVertexShadersDepthBufferShadow[2] = shaders->GetVertexShader("VSHADER_STATIC_DIRLIGHT_2POINTLIGHTS_FOG_SHADOW");
    TriVertexShadersDepthBufferShadow[3] = shaders->GetVertexShader("VSHADER_STATIC_DIRLIGHT_4POINTLIGHTS_FOG_SHADOW");
    TriVertexShadersDepthBufferShadow[4] = shaders->GetVertexShader("VSHADER_STATIC_DIRLIGHT_4POINTLIGHTS_FOG_SHADOW");
    TriVertexShadersDepthBufferShadow[5] = shaders->GetVertexShader("VSHADER_STATIC_DIRLIGHT_5POINTLIGHTS_FOG_SHADOW");

    TriVertexShadersSpotDepthBufferShadow.resize(6);
    TriVertexShadersSpotDepthBufferShadow[0] = shaders->GetVertexShader("VSHADER_STATIC_DIRLIGHT_FOG_SPOT_SHADOW");
    TriVertexShadersSpotDepthBufferShadow[1] = shaders->GetVertexShader("VSHADER_STATIC_DIRLIGHT_2POINTLIGHTS_FOG_SPOT_SHADOW");
    TriVertexShadersSpotDepthBufferShadow[2] = shaders->GetVertexShader("VSHADER_STATIC_DIRLIGHT_2POINTLIGHTS_FOG_SPOT_SHADOW");
    TriVertexShadersSpotDepthBufferShadow[3] = shaders->GetVertexShader("VSHADER_STATIC_DIRLIGHT_4POINTLIGHTS_FOG_SPOT_SHADOW");
    TriVertexShadersSpotDepthBufferShadow[4] = shaders->GetVertexShader("VSHADER_STATIC_DIRLIGHT_4POINTLIGHTS_FOG_SPOT_SHADOW");
    TriVertexShadersSpotDepthBufferShadow[5] = shaders->GetVertexShader("VSHADER_STATIC_DIRLIGHT_5POINTLIGHTS_FOG_SPOT_SHADOW");

    if (NEngineInternal::GPDisplayManager->GetPixelShaderVersion() >= 1400) {
        TriVertexShadersColourBufferShadow.resize(6);
        TriVertexShadersColourBufferShadow[0] = shaders->GetVertexShader("VSHADER_STATIC_DIRLIGHT_FOG_COLBUFF_SHADOW");
        TriVertexShadersColourBufferShadow[1] = shaders->GetVertexShader("VSHADER_STATIC_DIRLIGHT_2POINTLIGHTS_FOG_COLBUFF_SHADOW");
        TriVertexShadersColourBufferShadow[2] = shaders->GetVertexShader("VSHADER_STATIC_DIRLIGHT_2POINTLIGHTS_FOG_COLBUFF_SHADOW");
        TriVertexShadersColourBufferShadow[3] = shaders->GetVertexShader("VSHADER_STATIC_DIRLIGHT_4POINTLIGHTS_FOG_COLBUFF_SHADOW");
        TriVertexShadersColourBufferShadow[4] = shaders->GetVertexShader("VSHADER_STATIC_DIRLIGHT_4POINTLIGHTS_FOG_COLBUFF_SHADOW");
        TriVertexShadersColourBufferShadow[5] = shaders->GetVertexShader("VSHADER_STATIC_DIRLIGHT_5POINTLIGHTS_FOG_COLBUFF_SHADOW");
        TriVertexShadersSpotColourBufferShadow.resize(6);
        TriVertexShadersSpotColourBufferShadow[0] = shaders->GetVertexShader("VSHADER_STATIC_DIRLIGHT_FOG_SPOT_COLBUFF_SHADOW");
        TriVertexShadersSpotColourBufferShadow[1] = shaders->GetVertexShader("VSHADER_STATIC_DIRLIGHT_2POINTLIGHTS_FOG_SPOT_COLBUFF_SHADOW");
        TriVertexShadersSpotColourBufferShadow[2] = shaders->GetVertexShader("VSHADER_STATIC_DIRLIGHT_2POINTLIGHTS_FOG_SPOT_COLBUFF_SHADOW");
        TriVertexShadersSpotColourBufferShadow[3] = shaders->GetVertexShader("VSHADER_STATIC_DIRLIGHT_4POINTLIGHTS_FOG_SPOT_COLBUFF_SHADOW");
        TriVertexShadersSpotColourBufferShadow[4] = shaders->GetVertexShader("VSHADER_STATIC_DIRLIGHT_4POINTLIGHTS_FOG_SPOT_COLBUFF_SHADOW");
        TriVertexShadersSpotColourBufferShadow[5] = shaders->GetVertexShader("VSHADER_STATIC_DIRLIGHT_5POINTLIGHTS_FOG_SPOT_COLBUFF_SHADOW");
    }
    TriVertexShaderOutline = shaders->GetVertexShader("VSHADER_STATIC_OUTLINE");
    TriVertexShaderOutlineSmoothFill = shaders->GetVertexShader("VSHADER_STATIC_OUTLINE_SMOOTH_FILL");

    EnableCloth = true;
    ClothDebug = false;
}