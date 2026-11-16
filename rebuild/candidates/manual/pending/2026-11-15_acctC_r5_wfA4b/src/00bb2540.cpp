// CEngineClothRenderer::RenderPolygons(const CParticleSimulation&, const CParticleProgram&, const CMatrix3x4&, const CEngineLocalLightArray&, const CTexture**, unsigned long, bool, bool, unsigned long, bool, bool, unsigned long, ERenderPassType, CEngineInternalPrimitiveMeshBase&) -- draw a cloth simulation: create its two-buffer render data on first use, recompute the vertex normals, fill the next vertex buffer with position / normal / UV (a second, normal-flipped copy when double sided), build the triangle + quad index list (winding swapped on the back copy), then set the world transform, point lights and cull mode, prepare the shader state and draw indexed
// Grade: RELOCATION_MATCH (2905v2905) -- wfA4 opus pass 2: the earlier local-model draft re-authored on the shared headers.
// Matching facts (EVIDENCED = retail bytes / PDB, FITTED = chosen by grading among natural spellings):
//   EVIDENCED: the vertices are CTVertexNormTex1 (FVF 0x112, stride 0x20) filled through the FableWin CGenericVertex setters SetPosition /
//     SetNormal / SetU1V1 and C4DVector::ToC3DVector; both render-vertex indices are read first (0x00BB2770: the texture index spills to
//     [esp+0x88]); PositionBuffer is read once before the loops (edx, 0x00BB26FB) while VertexNormals is re-read per vertex; the render data
//     is initialised through the new pointer (ebx, 0x00BB2607..0x00BB265F) after Reset; the two winding bytes are one 2-byte object packed
//     into the dead render_tris argument slot ([esp+0x84] / [esp+0x85]) and swapped with FableWin's ??$SWAP@E@@YIXAAE0@Z; the vertex / index
//     Lock calls take the flags first (0x00BB26CB: push 0 ; push 0 ; push flags); FableWin GetDynamicVB* / GetDynamicIB* accessors.
//   FITTED: the `positions` local declared between PeekRenderVertices and PeekIndexedTextureCoords (keeps `this` in ebp across Lock, load
//     scheduled as 0x00BB26FB); the quad loop indexes quads[q] while the triangle loop walks a pointer (quad base biased +2, 0x00BB2CE9);
//     the SWAP before the base update (0x00BB2D4F: winding[0] read before the add).
// Parameter names: no PDB carries them (FableWin.pdb gives the signature only); the unused ones are left unnamed.
// Leaf: ?RenderPolygons@CEngineClothRenderer@@AAEXABVCParticleSimulation@@ABVCParticleProgram@@ABVCMatrix3x4@@ABVCEngineLocalLightArray@@PAPBVCTexture@@K_N5K55KW4ERenderPassType@@AAVCEngineInternalPrimitiveMeshBase@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00BB2540, 2905 bytes, thiscall `ret 38h`, frame 0x4C.
// Identity: Ego_r.pdb / FableWin.pdb ?RenderPolygons@CEngineClothRenderer@@AAEX... (private); callers RenderClothPrimitive 0x00BB3E80 and
//   Render2DClothPrimitive 0x00BB30A0 (both matched: `RenderPolygons(simulation, program, transform, lights, &texture, 1, false, two_sided,
//   sub_primitive.PeekRenderFlags(), true, true, 0x109, pass, primitive)`).
// Callees: operator new 0x00BFEA1A, CEngineParticleSimulationRenderData() 0x00BB63A0, CCountedPointer<CParticleSimulationRenderData>::Reset
//   0x00BB64A0, CVertexBufferWin32::Initialise 0x00A63150, CVertexBufferBase::Lock 0x00AC20A0 / Unlock 0x00AC20B0, CIndexBuffer::Initialise
//   0x00AC1F70 / Lock 0x00AC1FB0 / Unlock 0x00AC1F00, CEngineClothRenderer::CalculateVertexNormals 0x00BB1F50, SetWorldTransform 0x009881F0,
//   SetupShaderPointLights 0x00B44F20, the inline PrepareForRender Update* helpers, RealiseRenderState 0x00A058C0, RestoreCaptureBlock
//   0x00A05840, operator delete 0x00BFE9BC; IDirect3DDevice9 SetStreamSource / SetTexture / SetIndices / DrawIndexedPrimitive.
#include "fable_compiler.h"
#define FABLE_RETAIL_CVERTEXSHADER
#define FABLE_RETAIL_CINDEXBUFFER
#define FABLE_RETAIL_CMOVABLERESOURCE
#define FABLE_RETAIL_CSIMPLEMEMORYPOOL
#define FABLE_RETAIL_CENGINECLOTHRENDERER_MEMBERS
#define FABLE_RETAIL_CENGINECLOTHRENDERER_RENDER
#define FABLE_RETAIL_CENGINEPRIMITIVERENDERUTIL
#define FABLE_RETAIL_CENGINEPRIMITIVERENDERUTIL_DYNAMICVB
#define FABLE_RETAIL_CENGINEPRIMITIVERENDERUTIL_DYNAMICIB
#define FABLE_RETAIL_CENGINEPRIMITIVERENDERERSHAREDDATA_MEMBERS
#define FABLE_RETAIL_CENGINEPRIMITIVERENDERERSHAREDDATA_DYNAMICVB
#define FABLE_RETAIL_CENGINELIGHTINGMANAGER
#define FABLE_RETAIL_CPARTICLEPROGRAM
#define FABLE_RETAIL_CPARTICLEPROGRAM_RENDERACCESS
#define FABLE_RETAIL_CPARTICLEPROGRAM_TEXCOORDACCESS
#define FABLE_RETAIL_CPARTICLESIMULATION_MUTABLE_RENDERDATA
#define FABLE_RETAIL_CPARTICLESIMULATIONRENDERDATA_VIRTUAL_DTOR
#define FABLE_RETAIL_CVERTEXBUFFERWIN32
#define FABLE_RETAIL_CCOUNTEDPOINTER_RESET_CAMERA_DECL
#define FABLE_RETAIL_CGENERICVERTEX_INLINE_SETTERS
#define FABLE_RETAIL_C3DPRIMITIVE_CORNER_ACCESSORS
#define FABLE_RETAIL_CGENERICVERTEX_INLINE_SETU1V1
#define FABLE_RETAIL_CMATRIX3X4_TRANSPOSE
#define FABLE_RETAIL_CRGBFLOATCOLOUR
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_CRENDERMANAGERCORE_FORCEINLINE_HELPERS
#include "engine/CMatrix3x4.h"
#include "engine/CRenderManager.h"
#include "engine/CShaderRenderManager.h"
#include "engine/CIndexBuffer.h"
#include "engine/CTexture.h"
#include "engine/CVertexBufferWin32.h"
#include "engine/CRenderStateCaptureBlock.h"
#include "engine/CEngineLightingManager.h"
#include "engine/CEnginePrimitiveRendererSharedData.h"
#include "engine/CEnginePrimitiveRenderUtil.h"
#include "engine/C4DVector.h"
#include "engine/C3DVector.h"
#include "engine/C2DVector.h"
#include "engine/CArray.h"
#include "engine/CCountedPointer.h"
#include "engine/CParticleSimulation.h"
#include "engine/CParticleProgram.h"
#include "engine/CEngineParticleSimulationRenderData.h"
#include "engine/C3DClothRenderVertex.h"
#include "engine/C3DTriangle2.h"
#include "engine/C3DQuad2.h"
#include "engine/CTVertexNormTex1.h"
#include "engine/CEngineClothRenderer.h"
#include "engine/RenderStateEnums.h"

namespace NEngineInternal { extern CRenderManager* GPRenderManager; }                                      // 0x01436E18
namespace NEngineInternal { extern CShaderRenderManager* GPShaderRenderManager; }                          // 0x01436E14
namespace NEngineInternal { extern CEngineLightingManager* GPLightingManager; }                            // 0x01436E9C
namespace NEngineInternal { extern CEnginePrimitiveRendererSharedData* GPPrimitiveRendererSharedData; }   // 0x01436E78

FABLE_FORCEINLINE unsigned long __fastcall CEnginePrimitiveRenderUtil::GetDynamicVBUsage() { return NEngineInternal::GPPrimitiveRendererSharedData->DynamicVBUsage; }
FABLE_FORCEINLINE EVertexBufferPool __fastcall CEnginePrimitiveRenderUtil::GetDynamicVBPool() { return NEngineInternal::GPPrimitiveRendererSharedData->DynamicVBPool; }
FABLE_FORCEINLINE unsigned long __fastcall CEnginePrimitiveRenderUtil::GetDynamicVBLockFlags() { return NEngineInternal::GPPrimitiveRendererSharedData->DynamicVBLockFlags; }
FABLE_FORCEINLINE unsigned long __fastcall CEnginePrimitiveRenderUtil::GetDynamicIBUsage() { return NEngineInternal::GPPrimitiveRendererSharedData->DynamicIBUsage; }
FABLE_FORCEINLINE EIndexBufferPool __fastcall CEnginePrimitiveRenderUtil::GetDynamicIBPool() { return NEngineInternal::GPPrimitiveRendererSharedData->DynamicIBPool; }
FABLE_FORCEINLINE unsigned long __fastcall CEnginePrimitiveRenderUtil::GetDynamicIBLockFlags() { return NEngineInternal::GPPrimitiveRendererSharedData->DynamicIBLockFlags; }

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

void CEngineClothRenderer::RenderPolygons(const CParticleSimulation& simulation, const CParticleProgram& program, const CMatrix3x4& transform,
    const CEngineLocalLightArray& lights, const CTexture** textures, unsigned long texture_count, bool, bool double_sided,
    unsigned long, bool render_tris, bool render_quads, unsigned long shader_flags, ERenderPassType, CEngineInternalPrimitiveMeshBase&)
{
    unsigned long tri_count = render_tris ? program.GetRenderTriangleCount() : 0;
    unsigned long quad_count = render_quads ? program.GetRenderQuadCount() : 0;
    unsigned long prim_count = tri_count + quad_count * 2;
    if (prim_count == 0)
        return;
    unsigned long sides = (double_sided ? 1 : 0) + 1;
    unsigned long total_prims = sides * prim_count;
    unsigned long vertex_count = program.GetRenderVertexCount();
    unsigned long total_vertices = vertex_count * sides;
    if (simulation.RenderData.IsNull()) {
        CEngineParticleSimulationRenderData* render_data = new CEngineParticleSimulationRenderData;
        simulation.RenderData.Reset(render_data);
        for (long i = 0; i < 2; ++i) {
            if (!render_data->VertexBuffers[i].Initialise((EVertexType)0x112, total_vertices, sizeof(CTVertexNormTex1),
                    GetDynamicVBUsage(), GetDynamicVBPool())) {
                simulation.RenderData.ReleaseData();
                return;
            }
        }
    }
    CRenderStateScopedCaptureBlock capture;
    CalculateVertexNormals(simulation, program);
    CEngineParticleSimulationRenderData* data = static_cast<CEngineParticleSimulationRenderData*>(simulation.RenderData.Get());
    CVertexBufferWin32& vertex_buffer = data->VertexBuffers[data->CurrentVB];
    data->CurrentVB = (data->CurrentVB + 1) % 2;
    CTVertexNormTex1* vertices = (CTVertexNormTex1*)vertex_buffer.Lock(GetDynamicVBLockFlags(), 0, 0);
    const C3DClothRenderVertex* render_vertices = program.PeekRenderVertices();
    const C4DVector* positions = PositionBuffer;
    const C2DVector* coords = program.PeekIndexedTextureCoords();
    unsigned long out = 0;
    for (unsigned long side = 0; side < sides; ++side) {
        for (unsigned long i = 0; i < vertex_count; ++i, ++out) {
            const C3DClothRenderVertex& render_vertex = render_vertices[i];
            unsigned long position_index = render_vertex.PositionIndex;
            unsigned long tex_coord_index = render_vertex.TexCoordIndex;
            CTVertexNormTex1& v = vertices[out];
            v.SetPosition(positions[position_index].ToC3DVector());
            C3DVector normal = VertexNormals[position_index].ToC3DVector();
            if (side == 1)
                normal = -normal;
            v.SetNormal(normal);
            v.SetU1V1(coords[tex_coord_index].X, coords[tex_coord_index].Y);
        }
    }
    vertex_buffer.Unlock();
    __int64 index_bytes = total_prims * 3 * sizeof(unsigned short);
    if (index_bytes < 0 || index_bytes > 0x7fffffff)
        return;
    if ((long)index_bytes > IndexBufferSize) {
        IndexBuffer.Initialise((unsigned long)index_bytes, GetDynamicIBUsage(), (EIndexBufferFormat)0x65, GetDynamicIBPool(), (EPrimitiveType)4);
        IndexBufferSize = (long)index_bytes;
    }
    unsigned short* indices = (unsigned short*)IndexBuffer.Lock(GetDynamicIBLockFlags(), 0, 0);
    const C3DTriangle2* tris = program.PeekRenderTriangles();
    unsigned short base = 0;
    unsigned char winding[2] = { 0, 1 };
    for (unsigned long side = 0; side < sides; ++side) {
        const C3DTriangle2* tri = tris;
        for (unsigned long t = 0; t < tri_count; ++t, ++tri) {
            indices[winding[0]] = tri->GetA() + base;
            indices[winding[1]] = tri->GetB() + base;
            indices[2] = tri->GetC() + base;
            indices += 3;
        }
        const C3DQuad2* quads = program.PeekRenderQuads();
        for (unsigned long q = 0; q < quad_count; ++q) {
            indices[winding[0]] = quads[q].GetA() + base;
            indices[winding[1]] = quads[q].GetB() + base;
            indices[2] = quads[q].GetC() + base;
            indices += 3;
            indices[winding[0]] = quads[q].GetB() + base;
            indices[winding[1]] = quads[q].GetD() + base;
            indices[2] = quads[q].GetC() + base;
            indices += 3;
        }
        SWAP(winding[0], winding[1]);
        base = (unsigned short)(base + vertex_count);
    }
    IndexBuffer.Unlock();
    NEngineInternal::GPShaderRenderManager->SetWorldTransform(transform);
    NEngineInternal::GPLightingManager->SetupShaderPointLights(lights);
    NEngineInternal::GPRenderManager->SetPolyCullMode((POLY_CULL_MODE)0);
    NEngineInternal::GPShaderRenderManager->PrepareForRender(shader_flags);
    NEngineInternal::GPShaderRenderManager->DrawBuffersIndexed(textures, texture_count, vertex_buffer, 0, total_vertices, IndexBuffer, 0,
                                                              total_prims, PT_NULL);
}
