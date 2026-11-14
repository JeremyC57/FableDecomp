// CEngineClothRenderer::RenderPolygons(const CParticleSimulation&, const CParticleProgram&, const CMatrix3x4&, const CEngineLocalLightArray&, const CTexture**, unsigned long, bool, bool, unsigned long, bool, bool, unsigned long, ERenderPassType, CEngineInternalPrimitiveMeshBase&) -- draw a cloth simulation: create its two-buffer render data on first use, recompute the vertex normals, fill the next vertex buffer with position / normal / UV (a second, normal-flipped copy when double sided), build the triangle + quad index list (winding swapped on the back copy), then set the world transform, point lights and cull mode, prepare the shader state and draw indexed
// Grade: DIFFER(2726v2905) -- earlier lane draft, regraded unchanged today (93 frame-masked hunks; local class models: needs the shared CEngineClothRenderer _RENDER / CParticleProgram / CParticleSimulation headers and a new CEngineParticleSimulationRenderData header). 834 lines: first full reconstruction on the shared render headers (scoped capture block, inline PrepareForRender, DrawBuffersIndexed / SelectVertexBuffer bodies of the landed 0x00BA1B00, SetPolyCullMode); residue is frame 0x38 v 0x4c (retail spills `this` and keeps the counters in argument slots), the render-data init loop walking the new pointer, and the 4x-unrolled vertex fill scheduling
// Next idea: model CParticleProgram / CParticleSimulation with chunk 850's pending opt-in headers once landed, then match the vertex loop locals (retail keeps the running vertex index in the dead argument slot [esp+0x84] and the normal through three float temporaries)
// Leaf: ?RenderPolygons@CEngineClothRenderer@@AAEXABVCParticleSimulation@@ABVCParticleProgram@@ABVCMatrix3x4@@ABVCEngineLocalLightArray@@PAPBVCTexture@@K_N5K55KW4ERenderPassType@@AAVCEngineInternalPrimitiveMeshBase@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00BB2540, 2905 bytes, thiscall `ret 38h`, frame 0x4C.
// Identity: Ego_r.pdb / FableWin.pdb ?RenderPolygons@CEngineClothRenderer@@AAEX... (private); siblings GetPositionBuffer 0x00BB1E30,
//   GetNormalBuffer 0x00BB1E90, CalculateVertexNormals 0x00BB1F50.
// Callees: operator new 0x00BFEA1A, CEngineParticleSimulationRenderData() 0x00BB63A0, CCountedPointer<...>::Reset 0x00BB64A0,
//   CVertexBufferWin32::Initialise 0x00A63150, CVertexBufferBase Lock 0x00AC20A0 / Unlock 0x00AC20B0, CIndexBuffer Initialise
//   0x00AC1F70 / Lock 0x00AC1FB0 / Unlock 0x00AC1F00, SetWorldTransform 0x009881F0, SetupShaderPointLights 0x00B44F20, the inline
//   PrepareForRender Update* helpers (0x00988A20 .. 0x00989700), RealiseRenderState 0x00A058C0, RestoreCaptureBlock 0x00A05840;
//   IDirect3DDevice9 SetStreamSource (+0x190), SetTexture (+0x104), SetIndices (+0x1A0), DrawIndexedPrimitive (+0x148).
// Layout: CEngineClothRenderer (Ego_r less 0x18): VertexNormals +0x84, IndexBuffer +0x8C, IndexBufferSize +0xA0, PositionBuffer +0xC4;
//   CParticleProgram RenderTris +0x98, RenderQuads +0xA4, RenderVertices +0xB0, IndexedTextureCoords +0xC4 (chunk 850 header).
#define FABLE_RETAIL_CRGBFLOATCOLOUR
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_CRENDERMANAGERCORE_FORCEINLINE_HELPERS
#include "engine/CRenderManager.h"
#include "engine/CShaderRenderManager.h"
#include "engine/CIndexBuffer.h"
#include "engine/CTexture.h"
#include "engine/CVertexBufferWin32.h"
#include "engine/CRenderStateCaptureBlock.h"
#include "engine/C4DVector.h"
#include "engine/CArray.h"
#include "engine/CCountedPointer.h"
#include "engine/RenderStateEnums.h"

enum EIndexBufferFormat { INDEX_BUFFER_FORMAT_16 = 0x65 };
enum ERenderPassType;

class CMatrix3x4;
class CEngineLocalLightArray;
class CEngineInternalPrimitiveMeshBase;
class CEngineLightingManager { public:
    void SetupShaderPointLights(const CEngineLocalLightArray& lights);   // 0x00B44F20
};
class CEnginePrimitiveRendererConfig { public:
    unsigned char _pad00[0xc4];
    unsigned long VertexBufferUsage;            // +0xc4
    unsigned long VertexBufferPool;             // +0xc8
    unsigned long VertexBufferLockFlags;        // +0xcc
    unsigned long IndexBufferUsage;             // +0xd0
    unsigned long IndexBufferPool;              // +0xd4
    unsigned long IndexBufferLockFlags;         // +0xd8
};
namespace NEngineInternal { extern CRenderManager* GPRenderManager; }              // 0x01436E18
namespace NEngineInternal { extern CShaderRenderManager* GPShaderRenderManager; }  // 0x01436E14
namespace NEngineInternal { extern CEngineLightingManager* GPLightingManager; }    // 0x01436E9C
extern CEnginePrimitiveRendererConfig* g_pPrimitiveRenderer;                      // 0x01436E78

class C2DVector { public: float X, Y; };
struct C3DTriangle2 { unsigned short A, B, C; };
struct C3DQuad2 { unsigned short A, B, C, D; };
struct C3DClothRenderVertex { unsigned short Particle, TexCoord; };
struct CClothVertex { float PX, PY, PZ; float NX, NY, NZ; float U, V; };
class CEngineParticleSimulationRenderData { public:
    CEngineParticleSimulationRenderData();      // 0x00BB63A0
    virtual ~CEngineParticleSimulationRenderData();
    CVertexBufferWin32 VertexBuffers[2];        // +0x04
    long CurrentBuffer;                         // +0x6c
};
template<> void CCountedPointer<CEngineParticleSimulationRenderData>::Reset(CEngineParticleSimulationRenderData*);   // 0x00BB64A0
class CParticleSimulation { public:
    void* _vftable;
    mutable CCountedPointer<CEngineParticleSimulationRenderData> RenderData;   // +0x04
};
class CParticleProgram { public:
    unsigned char _pad00[0x98];
    CArray<C3DTriangle2> RenderTris;            // +0x98
    CArray<C3DQuad2> RenderQuads;               // +0xa4
    CArray<C3DClothRenderVertex> RenderVertices;   // +0xb0
    unsigned char _padbc[0x08];
    CArray<C2DVector> IndexedTextureCoords;     // +0xc4
};
class CEngineClothRenderer { public:
    unsigned char _pad000[0x84];
    C4DVector* VertexNormals;                   // +0x84
    unsigned char _pad088[0x04];
    CIndexBuffer IndexBuffer;                   // +0x8c
    long IndexBufferSize;                       // +0xa0
    unsigned char _pada4[0xc4 - 0xa4];
    C4DVector* PositionBuffer;                  // +0xc4
    void CalculateVertexNormals(const CParticleSimulation& simulation, const CParticleProgram& program);   // 0x00BB1F50
private:
    void RenderPolygons(const CParticleSimulation& simulation, const CParticleProgram& program, const CMatrix3x4& transform,
        const CEngineLocalLightArray& lights, const CTexture** textures, unsigned long texture_count, bool unused1, bool double_sided,
        unsigned long unused2, bool render_tris, bool render_quads, unsigned long render_flags, ERenderPassType pass,
        CEngineInternalPrimitiveMeshBase& mesh);
};

__forceinline void CShaderRenderManager::SelectVertexBuffer(CVertexBufferWin32& vertexBuffer)
{
    D3DDevice->SetStreamSource(0, vertexBuffer.PVertexBuffer, 0, vertexBuffer.Stride);
}
__forceinline void CShaderRenderManager::DrawBuffersIndexed(unsigned long minVertexIndex, unsigned long numVertices, CIndexBuffer& indexBuffer,
                                                            unsigned long startIndex, unsigned long primitiveCount, EPrimitiveType primitiveType)
{
    D3DDevice->SetIndices(indexBuffer.PD3DIndexBuffer);
    if (primitiveType == PT_NULL)
        primitiveType = *(EPrimitiveType*)indexBuffer.PrimitiveType;
    RenderManager->RealiseRenderState();
    D3DDevice->DrawIndexedPrimitive(primitiveType, 0, minVertexIndex, numVertices, startIndex, primitiveCount);
}
__forceinline void CShaderRenderManager::DrawBuffersIndexed(const CTexture** textures, unsigned long textureCount, CVertexBufferWin32& vertexBuffer,
                                                            unsigned long minVertexIndex, unsigned long numVertices, CIndexBuffer& indexBuffer,
                                                            unsigned long startIndex, unsigned long primitiveCount, EPrimitiveType primitiveType)
{
    SelectVertexBuffer(vertexBuffer);
    for (unsigned long i = 0; i < textureCount; ++i)
        RenderManager->AttachTextureToStage(i, textures[i]);
    DrawBuffersIndexed(minVertexIndex, numVertices, indexBuffer, startIndex, primitiveCount, primitiveType);
}

void CEngineClothRenderer::RenderPolygons(const CParticleSimulation& simulation, const CParticleProgram& program, const CMatrix3x4& transform,
    const CEngineLocalLightArray& lights, const CTexture** textures, unsigned long texture_count, bool unused1, bool double_sided,
    unsigned long unused2, bool render_tris, bool render_quads, unsigned long render_flags, ERenderPassType pass,
    CEngineInternalPrimitiveMeshBase& mesh)
{
    unsigned long tri_count = render_tris ? program.RenderTris.size() : 0;
    unsigned long quad_count = render_quads ? program.RenderQuads.size() : 0;
    unsigned long prim_count = tri_count + quad_count * 2;
    if (prim_count == 0)
        return;
    unsigned long sides = (double_sided ? 1 : 0) + 1;
    unsigned long total_prims = sides * prim_count;
    unsigned long vertex_count = program.RenderVertices.size();
    unsigned long total_vertices = vertex_count * sides;
    if (simulation.RenderData.IsNull()) {
        simulation.RenderData.Reset(new CEngineParticleSimulationRenderData);
        for (long i = 0; i < 2; ++i) {
            if (!simulation.RenderData->VertexBuffers[i].Initialise((EVertexType)0x112, total_vertices, sizeof(CClothVertex),
                    g_pPrimitiveRenderer->VertexBufferUsage, (EVertexBufferPool)g_pPrimitiveRenderer->VertexBufferPool)) {
                simulation.RenderData.ReleaseData();
                return;
            }
        }
    }
    CRenderStateScopedCaptureBlock capture;
    CalculateVertexNormals(simulation, program);
    CEngineParticleSimulationRenderData* data = simulation.RenderData.Get();
    CVertexBufferWin32& vertex_buffer = data->VertexBuffers[data->CurrentBuffer];
    data->CurrentBuffer = (data->CurrentBuffer + 1) % 2;
    CClothVertex* vertices = (CClothVertex*)vertex_buffer.Lock(0, 0, g_pPrimitiveRenderer->VertexBufferLockFlags);
    const C3DClothRenderVertex* render_vertices = program.RenderVertices.GetAsCArray();
    const C2DVector* coords = program.IndexedTextureCoords.GetAsCArray();
    unsigned long out = 0;
    for (unsigned long side = 0; side < sides; ++side) {
        for (unsigned long i = 0; i < vertex_count; ++i, ++out) {
            CClothVertex& v = vertices[out];
            const C4DVector& position = PositionBuffer[render_vertices[i].Particle];
            v.PX = position.X; v.PY = position.Y; v.PZ = position.Z;
            const C4DVector& n = VertexNormals[render_vertices[i].Particle];
            C3DVector normal(n.X, n.Y, n.Z);
            if (side == 1)
                normal = -normal;
            v.NX = normal.X; v.NY = normal.Y; v.NZ = normal.Z;
            const C2DVector& uv = coords[render_vertices[i].TexCoord];
            v.U = uv.X; v.V = uv.Y;
        }
    }
    vertex_buffer.Unlock();
    __int64 index_bytes = total_prims * 3 * sizeof(unsigned short);
    if (index_bytes < 0 || index_bytes > 0x7fffffff)
        return;
    if ((long)index_bytes > IndexBufferSize) {
        IndexBuffer.Initialise((unsigned long)index_bytes, g_pPrimitiveRenderer->IndexBufferUsage, INDEX_BUFFER_FORMAT_16,
                               (EIndexBufferPool)g_pPrimitiveRenderer->IndexBufferPool, (EPrimitiveType)4);
        IndexBufferSize = (long)index_bytes;
    }
    unsigned short* indices = (unsigned short*)IndexBuffer.Lock(0, 0, g_pPrimitiveRenderer->IndexBufferLockFlags);
    const C3DTriangle2* tris = program.RenderTris.GetAsCArray();
    unsigned short base = 0;
    unsigned char first = 0, second = 1;
    for (unsigned long side = 0; side < sides; ++side) {
        const C3DTriangle2* tri = tris;
        for (unsigned long t = 0; t < tri_count; ++t, ++tri) {
            indices[first] = tri->A + base;
            indices[second] = tri->B + base;
            indices[2] = tri->C + base;
            indices += 3;
        }
        const C3DQuad2* quad = program.RenderQuads.GetAsCArray();
        for (unsigned long q = 0; q < quad_count; ++q, ++quad) {
            indices[first] = quad->A + base;
            indices[second] = quad->B + base;
            indices[2] = quad->C + base;
            indices += 3;
            indices[first] = quad->B + base;
            indices[second] = quad->D + base;
            indices[2] = quad->C + base;
            indices += 3;
        }
        base = (unsigned short)(base + vertex_count);
        unsigned char swap = first; first = second; second = swap;
    }
    IndexBuffer.Unlock();
    NEngineInternal::GPShaderRenderManager->SetWorldTransform(transform);
    NEngineInternal::GPLightingManager->SetupShaderPointLights(lights);
    NEngineInternal::GPRenderManager->SetPolyCullMode((POLY_CULL_MODE)0);
    NEngineInternal::GPShaderRenderManager->PrepareForRender(render_flags);
    NEngineInternal::GPShaderRenderManager->DrawBuffersIndexed(textures, texture_count, vertex_buffer, 0, total_vertices, IndexBuffer, 0,
                                                              total_prims, PT_NULL);
}
