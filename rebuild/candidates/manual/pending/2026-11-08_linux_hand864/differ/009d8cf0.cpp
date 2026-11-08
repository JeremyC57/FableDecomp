// CRenderManager2D::ClearRenderTarget(const CRGBColour& colour, unsigned long flags) -- clear the render target by drawing a full-viewport quad through the 2D vertex shader (colour / z / stencil per flags), or hand off to the display manager when alternate clears are off
// Grade: DIFFER(3385v3385) -- 12 lines: the three texture-stage bookmark blocks OR the CurrentBookmarkMask in the other operand order (retail `mov edx,[eax+8] ; mov ecx,[esi+2814h] ; or ecx,edx`, varying per site); chunk 864 (1 grade, probe): the bookmark temporaries are uniform in ours (5-ref temp prio 40 -> ecx, 3-ref 24 -> edx at every site) while retail's order differs per site -- a per-site CSE/tuple-order effect (research 7: R6-class bookmark flips); 100+ earlier grades
// Next idea: per-site operand order of the OR; park with 0x009FA780 (same family)
// Leaf: ?ClearRenderTarget@CRenderManager2D@@QAEXABVCRGBColour@@K@Z
// Flags: /GS
#define FABLE_RETAIL_CRENDERMANAGER2D
#define FABLE_RETAIL_CRENDERMANAGERCORE
#define FABLE_RETAIL_CRENDERMANAGERCORE_DRAWASINDEXEDTRIS_USER
#define FABLE_RETAIL_CRENDERSTATEMANAGER
#define FABLE_RETAIL_CRENDERSTATEMANAGER_COLOURWRITE4
#define FABLE_RETAIL_CRENDERSTATEMANAGER_STENCIL_INLINE
#define FABLE_RETAIL_CRENDERSTATEMANAGER_STENCILOP_INLINE
#define FABLE_RETAIL_CDISPLAYMANAGER
#define FABLE_RETAIL_CDISPLAYMANAGER_GETVIEWPORT
#define FABLE_RETAIL_CDISPLAYMANAGER_SETVIEWPORT
#define FABLE_RETAIL_C2DBOXI_CTORS
#define FABLE_RETAIL_C2DBOXI_COPY
#define FABLE_RETAIL_C2DBOXI
#define FABLE_RETAIL_SCOPED_PTR
#define FABLE_RETAIL_CFONTBANK
#define FABLE_RETAIL_CVERTEXSHADER
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CTEXTURE
#define FABLE_RETAIL_CPIXELFORMAT
#define FABLE_RETAIL_CSURFACE
#define FABLE_RETAIL_CVERTEXSHADERCONSTANTLAYOUT
#include <string.h>
#include "engine/IDirect3DDevice9.h"
#include "engine/Direct3D9Types.h"
#include "stlport/fable_containers.h"
#include "engine/CRenderManager2D.h"
#include "engine/CDisplayManager.h"
#include "engine/CRGBColour.h"
#include "engine/CShaderRenderManager.h"
#include "engine/CScopedShadersEnable.h"
#include "engine/CScopedVertexShaderConstantLayout.h"
#include "engine/CRenderStateCaptureBlock.h"
#include "engine/CVertexShaderConstantLayoutEmpty.h"
static CVertexShaderConstantLayoutEmpty GVertexShaderConstantLayoutEmpty;

void CRenderManager2D::ClearRenderTarget(const CRGBColour& colour, unsigned long flags)
{
    if (flags == 0)
        flags = 7;
    if (!AlternateRenderTargetClearEnabled) {
        GetDisplayManager().ClearRenderTarget(colour, flags);
        return;
    }
    CScopedShadersEnable enabled(true, false);
    CRenderStateScopedCaptureBlock rsblock;
    SetFogEnable(false);
    SetSpecularEnable(false);
    SetAlphaBlendEnable(false);
    SetAlphaTestEnable(false);
    if (flags & 4)
        SetColourWriteEnable(true, true, true, true);
    else
        SetColourWriteEnable(false, false, false, false);
    if (flags & 1) {
        SetZBufferEnable(true);
        SetZBufferWriteEnable(true);
        SetZBufferTestFunc(ZBUFFER_FUNC_ALWAYS);
    } else
        SetZBufferEnable(false);
    if (flags & 2) {
        SetStencilEnable(true);
        SetStencilFunc(STENCIL_FUNC_ALWAYS);
        SetStencilOpPass(STENCIL_OP_MODE_REPLACE);
        SetStencilOpFail(STENCIL_OP_MODE_REPLACE);
        SetStencilOpZFail(STENCIL_OP_MODE_REPLACE);
        SetStencilMask(0xff);
        SetStencilRef(0);
    } else
        SetStencilEnable(false);

    C2DBoxI old_viewport = GetDisplayManager().GetViewport();
    C2DExtentsI dimensions = GetDisplayManager().GetRenderTargetDimensions();
    GetDisplayManager().SetViewport(C2DBoxI(0, 0, dimensions.X, dimensions.Y));

    CTVertexRHWColSpecTex1 vertices[4];
    vertices[0].X = -1.0f; vertices[0].Y = 1.0f;  vertices[0].Z = 0.99999994f;
    vertices[1].X = 1.0f;  vertices[1].Y = 1.0f;  vertices[1].Z = 0.99999994f;
    vertices[2].X = 1.0f;  vertices[2].Y = -1.0f; vertices[2].Z = 0.99999994f;
    vertices[3].X = -1.0f; vertices[3].Y = -1.0f; vertices[3].Z = 0.99999994f;
    for (long i = 0; i < 4; ++i) {
        vertices[i].U1 = 0.0f;
        vertices[i].V1 = 0.0f;
        vertices[i].DiffuseColour = colour.IntValue;
        vertices[i].RHW = 1.0f;
    }
    unsigned short indices[6];
    indices[0] = 0;
    indices[1] = 1;
    indices[2] = 2;
    indices[3] = 0;
    indices[4] = 2;
    indices[5] = 3;

    CShaderRenderManager& shaders = CShaderRenderManager::Get();
    shaders.RegisterVertexShaderConstantLayout(0, &GVertexShaderConstantLayoutEmpty);
    CScopedVertexShaderConstantLayout constant_layout(0);
    shaders.ApplyVertexShader(VertexShader2D, false);
    shaders.PrepareForRender(0);
    SetTextureOperation(0, TEXTURE_OP_SELECT_ARG1);
    SetTextureOperation(1, TEXTURE_OP_DISABLE);
    SetTextureAlphaOperation(0, TEXTURE_OP_SELECT_ARG1);
    SetTextureAlphaOperation(1, TEXTURE_OP_DISABLE);
    SetTextureArgument1(0, TEXTURE_ARG_DIFFUSE);
    SetTextureAlphaArgument1(0, TEXTURE_ARG_DIFFUSE);
    DrawAsIndexedTris(vertices, sizeof(CTVertexRHWColSpecTex1), 4, indices, 2);
    GetDisplayManager().SetViewport(old_viewport);
}
