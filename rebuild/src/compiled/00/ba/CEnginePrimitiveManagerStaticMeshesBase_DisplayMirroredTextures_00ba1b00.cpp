// CEnginePrimitiveManagerStaticMeshesBase::DisplayMirroredTextures(const CTexture** textures, unsigned long textureCount, C3DPrimitive2* primitive, ERenderPassType pass, CEngineSubPrimitiveStaticMesh& subPrimitive, unsigned long renderFlags) -- highlight the primitive's mirrored-texture triangles: build a 16-bit index buffer from its mirrored vertex index list and draw it twice, tinted red then green
// Leaf: ?DisplayMirroredTextures@CEnginePrimitiveManagerStaticMeshesBase@@AAEXPAPBVCTexture@@KPAVC3DPrimitive2@@W4ERenderPassType@@AAVCEngineSubPrimitiveStaticMesh@@K@Z
// Retail: Fable.exe 0x00BA1B00, 2113 bytes, private thiscall, `ret 18h`, esp frame (sub esp,44h; ebx pushed after the early-out
//   tests). Name: Ego_r.pdb + FableWin.pdb ?DisplayMirroredTextures@CEnginePrimitiveManagerStaticMeshesBase@@AAEXPAPBVCTexture@@KPAV
//   C3DPrimitive2@@W4ERenderPassType@@AAVCEngineSubPrimitiveStaticMesh@@K@Z (both name sets; the gate is the renderer's
//   HighlightMirroredTexturesEnabled +0x262 and the list is C3DPrimitive2::MirroredTextureVertexIndexList +0x94, Ego_r names).
// Callees: CShaderRenderManager::ApplyVertexShader 0x00988020 / ApplyPixelShader 0x00988140 / SetVSConstants 0x0098B930 and the
//   PrepareForRender Update* helpers (0x00988A20 .. 0x00989700), ResetWorldTransform 0x00988290, CIndexBuffer() 0x00AC1EC0 /
//   Initialise 0x00AC1F70 / Lock 0x00AC1FB0 / Unlock 0x00AC1F00 / Uninitialise 0x00AC1EE0 (the inline ~CIndexBuffer, vtable
//   0x0129EF2C), CRenderStateManager::RealiseRenderState 0x00A058C0, __ftol2 0x00BFEA70, IDirect3DDevice9 SetStreamSource (+0x190),
//   SetTexture (+0x104), SetIndices (+0x1A0), DrawIndexedPrimitive (+0x148). Globals: NEngineInternal::GPShaderRenderManager
//   0x01436E14, GPRenderManager 0x01436E18, g_pPrimitiveRenderer 0x01436E78 (DynamicIBUsage / Pool / LockFlags +0xD0..+0xD8,
//   PixelShaderDiffuse +0xF4), 1/255 0x01231724, 255.0 0x01230014.
// Shape: two highlight passes (red, then green) over one dynamic 16-bit index buffer filled from the mirrored vertex index list;
//   each pass is CEnginePrimitiveRenderUtil::SetTextureFactor (pixel-shader constant 2 when pixel shaders are on, else the
//   TEXTUREFACTOR state), RealiseRenderState, the inline PrepareForRender(renderFlags) and the inline nine-argument
//   DrawBuffersIndexed (vertex count index_count / 2, primitive count index_count / 6, the buffer's own primitive type); then the
//   inline CRenderManagerCore::FlushRender (stream 0, indices and every used texture stage released).
// Levers: (1) CIndexBuffer::Lock takes the lock flags first (Lock(flags, 0, 0): retail pushes 0, 0, flags); (2) the closing reset is
//   one inline member, FlushRender (FableWin.pdb-only ?FlushRender@CRenderManagerCore@@QAEXXZ 0x02CABE80; no Ego_r or retail copy):
//   written as separate statements VC7.1 re-reads the manager pointer each time where retail keeps it in ebp and &PD3DDevice in esi;
//   (3) the inline helpers retail expands (SetTextureFactor, CRenderStateManager::SetTextureFactor, GetPixelShadersEnabled,
//   SelectVertexBuffer, both DrawBuffersIndexed overloads, FlushRender) are same-TU __forceinline bodies of the shared headers'
//   declarations, as the landed DrawBuffersIndexed 0x00BA7DB0 / RenderMeshPrimitiveBlocksDegenerateTriangles 0x00B9E750 write them.
// Headers: engine/CShaderRenderManager.h, CRenderManager.h (CRenderManagerCore / CRenderStateManager retail branches), CIndexBuffer.h;
//   opt-ins (hdr/): CEnginePrimitiveRendererSharedData members, the CEnginePrimitiveManagerStaticMeshesBase declaration, the
//   CEnginePrimitiveRenderUtil::SetTextureFactor declaration. C3DPrimitive2 / CEngineSubPrimitiveStaticMesh have no header (local
//   models of the two members read).
#include "fable_compiler.h"
#define FABLE_RETAIL_CRGBFLOATCOLOUR
#define FABLE_RETAIL_CRENDERMANAGERCORE_FORCEINLINE_HELPERS
#define FABLE_RETAIL_CENGINEPRIMITIVERENDERUTIL_SETTEXTUREFACTOR
#define FABLE_RETAIL_CENGINEPRIMITIVERENDERERSHAREDDATA_MEMBERS
#define FABLE_RETAIL_CENGINEPRIMITIVEMANAGERSTATICMESHESBASE_DISPLAYMIRRORED
#define FABLE_RETAIL_CENGINEPRIMITIVERENDERER
#include "engine/CRenderManager.h"
#include "engine/CShaderRenderManager.h"
#include "engine/CIndexBuffer.h"
#include "engine/CTexture.h"
#include "engine/CVertexBufferWin32.h"
#include "engine/CVertexShader.h"
#include "engine/CPixelShader.h"
#include "engine/CRGBFloatColour.h"
#include "engine/C4DVector.h"
#include "engine/CEnginePrimitiveRenderUtil.h"
#include "engine/CEnginePrimitiveRendererSharedData.h"
#include "engine/CEnginePrimitiveRenderer.h"
#include "engine/CEnginePrimitiveManagerStaticMeshesBase.h"
#include "engine/CArray.h"

enum EIndexBufferFormat { INDEX_BUFFER_FORMAT_16 = 0x65 };      // D3DFMT_INDEX16
enum ERenderPassType;

namespace NEngineInternal { extern CRenderManager* GPRenderManager; }              // 0x01436E18
namespace NEngineInternal { extern CShaderRenderManager* GPShaderRenderManager; }  // 0x01436E14
extern CEnginePrimitiveRenderer* g_pPrimitiveRenderer;                             // 0x01436E78

// C3DPrimitive2 (Ego_r.pdb; retail 4 lower past the 12-byte ClothPrimitives CArray and 8 lower past the second CArray): no header.
class C3DPrimitive2 {
public:
    unsigned char          _pad00[0x30];
    CVertexBufferWin32*    VertexBuffer;                    // +0x30 scoped_ptr<CVertexBufferWin32>
    unsigned char          _pad34[0x94 - 0x34];
    CArray<unsigned long>  MirroredTextureVertexIndexList;  // +0x94
};
// CEngineSubPrimitiveStaticMesh : CEngineSubPrimitiveBase (Ego_r.pdb): Alpha +0x08. No header.
class CEngineSubPrimitiveStaticMesh {
public:
    void*         _vftable;
    unsigned long RenderFlags;                              // +0x04
    unsigned char Alpha;                                    // +0x08
};

FABLE_FORCEINLINE bool CShaderRenderManager::GetPixelShadersEnabled() const { return PixelShadersEnabled; }
FABLE_FORCEINLINE void CRenderStateManager::SetTextureFactor(unsigned long factor) { SetRenderState(factor, DeviceState.TextureFactor); }
FABLE_FORCEINLINE void CShaderRenderManager::SelectVertexBuffer(CVertexBufferWin32& vertexBuffer)
{
    D3DDevice->SetStreamSource(0, vertexBuffer.PVertexBuffer, 0, vertexBuffer.Stride);
}
FABLE_FORCEINLINE void CShaderRenderManager::DrawBuffersIndexed(unsigned long minVertexIndex, unsigned long numVertices, CIndexBuffer& indexBuffer,
                                                            unsigned long startIndex, unsigned long primitiveCount, EPrimitiveType primitiveType)
{
    D3DDevice->SetIndices(indexBuffer.PD3DIndexBuffer);
    if (primitiveType == PT_NULL)
        primitiveType = *(EPrimitiveType*)indexBuffer.PrimitiveType;
    RenderManager->RealiseRenderState();
    D3DDevice->DrawIndexedPrimitive(primitiveType, 0, minVertexIndex, numVertices, startIndex, primitiveCount);
}
FABLE_FORCEINLINE void CShaderRenderManager::DrawBuffersIndexed(const CTexture** textures, unsigned long textureCount, CVertexBufferWin32& vertexBuffer,
                                                            unsigned long minVertexIndex, unsigned long numVertices, CIndexBuffer& indexBuffer,
                                                            unsigned long startIndex, unsigned long primitiveCount, EPrimitiveType primitiveType)
{
    SelectVertexBuffer(vertexBuffer);
    for (unsigned long i = 0; i < textureCount; ++i)
        RenderManager->AttachTextureToStage(i, textures[i]);
    DrawBuffersIndexed(minVertexIndex, numVertices, indexBuffer, startIndex, primitiveCount, primitiveType);
}
FABLE_FORCEINLINE void CRenderManagerCore::FlushRender()
{
    SelectVertexBuffer(0, 0);
    PD3DDevice->SetIndices(0);
    for (long stage = 0; stage < TextureStagesInUse; ++stage) {
        if (CurrentlySelectedTextures[stage]) {
            CurrentlySelectedTextures[stage] = 0;
            PD3DDevice->SetTexture(stage, 0);
        }
    }
    TextureStagesInUse = 0;
}
FABLE_FORCEINLINE void __fastcall CEnginePrimitiveRenderUtil::SetTextureFactor(const CRGBFloatColour& colour)
{
    if (NEngineInternal::GPShaderRenderManager->GetPixelShadersEnabled())
        NEngineInternal::GPShaderRenderManager->SetPixelShaderConstant(2, C4DVector(colour.R, colour.G, colour.B, colour.A));
    else
        NEngineInternal::GPRenderManager->SetTextureFactor(colour.GetRGBColour().IntValue);
}

void CEnginePrimitiveManagerStaticMeshesBase::DisplayMirroredTextures(const CTexture** textures, unsigned long textureCount, C3DPrimitive2* primitive,
                                                                      ERenderPassType pass, CEngineSubPrimitiveStaticMesh& subPrimitive,
                                                                      unsigned long renderFlags)
{
    unsigned long index_count = primitive->MirroredTextureVertexIndexList.size();
    if (index_count == 0 || !g_pPrimitiveRenderer->HighlightMirroredTexturesEnabled || pass == 1)
        return;
    NEngineInternal::GPShaderRenderManager->ApplyVertexShader(((const CVertexShader*)VertexShadersStatic)[PointLightShaderIndex[0]], true);
    NEngineInternal::GPShaderRenderManager->ApplyPixelShader(g_pPrimitiveRenderer->PixelShaderDiffuse);
    CIndexBuffer index_buffer;
    index_buffer.Initialise(index_count * 2, g_pPrimitiveRenderer->DynamicIBUsage, INDEX_BUFFER_FORMAT_16, g_pPrimitiveRenderer->DynamicIBPool,
                            PT_TRIANGLE_LIST);
    unsigned short* indices = (unsigned short*)index_buffer.Lock(g_pPrimitiveRenderer->DynamicIBLockFlags, 0, 0);
    for (unsigned long i = 0; i < index_count; ++i)
        indices[i] = (unsigned short)primitive->MirroredTextureVertexIndexList[i];
    index_buffer.Unlock();

    CEnginePrimitiveRenderUtil::SetTextureFactor(CRGBFloatColour(1.0f, 0.0f, 0.0f, subPrimitive.Alpha * (1.0f / 255.0f)));
    NEngineInternal::GPRenderManager->RealiseRenderState();
    NEngineInternal::GPShaderRenderManager->PrepareForRender(renderFlags);
    NEngineInternal::GPShaderRenderManager->DrawBuffersIndexed(textures, textureCount, *primitive->VertexBuffer, 0, index_count / 2, index_buffer, 0,
                                                               index_count / 6, PT_NULL);

    CEnginePrimitiveRenderUtil::SetTextureFactor(CRGBFloatColour(0.0f, 1.0f, 0.0f, subPrimitive.Alpha * (1.0f / 255.0f)));
    NEngineInternal::GPRenderManager->RealiseRenderState();
    NEngineInternal::GPShaderRenderManager->PrepareForRender(renderFlags);
    NEngineInternal::GPShaderRenderManager->DrawBuffersIndexed(textures, textureCount, *primitive->VertexBuffer, 0, index_count / 2, index_buffer, 0,
                                                               index_count / 6, PT_NULL);

    NEngineInternal::GPRenderManager->FlushRender();
}