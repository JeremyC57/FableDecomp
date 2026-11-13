// CEngineClothRenderer::CalculateVertexNormals(const CParticleSimulation& simulation, const CParticleProgram& program) -- rebuild the cloth's per-vertex normals: size and clear the normal buffer, add every render triangle's face normal (p2 - p0) x (p1 - p0) to its three corners (the hand SSE step when the CPU has SSE, x87 otherwise), add both halves' normals of every render quad to their corners, then normalise the buffer
// Grade: RELOCATION_MATCH (1508 bytes). Shape: PositionBuffer is read before the triangle count (gives retail's edi for the count's begin load, 0x00BB1FDB); the cross product's three components each grouped as a unit (FABLE_RETAIL_C3DVECTOR_CROSS_XYZ_GROUP, no code: three PAREN tuples per cross) put c2's 81-tuple scheduling cuts where retail has them in the triangle arm (corner adds 0x00BB21CC..0x00BB221B) and the quad loop; subtraction keeps the default text; corners read through FableWin's inline C3DTriangle2 / C3DQuad2 GetA..GetD (COMDATs 0x02E77990.. listed after this function).
// Leaf: ?CalculateVertexNormals@CEngineClothRenderer@@AAEXABVCParticleSimulation@@ABVCParticleProgram@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00BB1F50, 1508 bytes, thiscall, `ret 8` (the simulation argument is unused).
// Identity (two sources): Ego_r.pdb 0x00A67E10 / FableWin.pdb 0x02E76000 ?CalculateVertexNormals@CEngineClothRenderer@@AAEXABVCParticleSimulation@@ABVCParticleProgram@@@Z
//   (private); Ego_r donor order GetNormalBuffer 0x00A67D30 -> FrameStart -> CalculateVertexNormals = retail 0x00BB1E90 (called first) .. 0x00BB1F50.
// Callees: CEngineClothRenderer::GetNormalBuffer 0x00BB1E90, NVectorMath::GFClear4DVectors 0x00A5C300 / GFNormalise3DVectors 0x00A5C360;
//   the SSE flag g_SSEAvailable 0x013D2880.
// Retail-asm: 0x00BB209F `movaps xmm2,[esi+eax]` .. 0x00BB20B7 `shufps xmm0,xmm0,9` .. 0x00BB20C7 `mulps xmm0,xmm1` .. 0x00BB20DC `addps xmm1,xmm0` (block 0x00BB2079..0x00BB20ED, operands re-read from frame words, no aligned frame) -- the SSE triangle-normal block, reached only through the shared helper
//   engine/CEngineClothRendererSSE.h GF_SSE_ADD_TRIANGLE_NORMAL (evidence: that header). Per-site attempts table (ASM_RULES rule 4,
//   chunk 3017 NOTES.md + probe/asm/table.md): 31 plain-C++ spellings (x87 under /O2; scalar movss/mulss under /arch:SSE / SSE2) and
//   12 intrinsic spellings (packed xmm code, but every one with an `and esp,-16` aligned frame, compiler-chosen xmm numbering and no frame
//   re-read of the operands) -- none emits retail's block; the helper (control) does, under all three flag sets.
#include "fable_compiler.h"
#define FABLE_RETAIL_CPARTICLEPROGRAM
#define FABLE_RETAIL_CPARTICLEPROGRAM_RENDERACCESS
#define FABLE_RETAIL_CMOVABLERESOURCE
#define FABLE_RETAIL_C4DVECTOR
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_C3DVECTOR_CROSS_XYZ_GROUP
#define FABLE_RETAIL_C4DVECTOR_ADDASSIGN
#define FABLE_RETAIL_C3DPRIMITIVE_CORNER_ACCESSORS
#include "engine/C3DVector.h"
#include "engine/C4DVector.h"
#include "engine/CParticleProgram.h"
#include "engine/CEngineClothRenderer.h"
#include "engine/NVectorMath.h"
#include "engine/CEngineClothRendererSSE.h"

void CEngineClothRenderer::CalculateVertexNormals(const CParticleSimulation& simulation, const CParticleProgram& program)
{
    unsigned long vertex_count = program.GetVertexPositionCount();
    VertexNormals = GetNormalBuffer(vertex_count);
    C4DVector* normals = VertexNormals;
    NVectorMath::GFClear4DVectors(normals, vertex_count, C4DVector(0.0f, 0.0f, 0.0f, 0.0f));
    const C3DClothRenderVertex* vertices = program.PeekRenderVertices();
    const C4DVector* positions = PositionBuffer;
    unsigned long triangle_count = program.GetRenderTriangleCount();
    const C3DTriangle2* triangles = program.PeekRenderTriangles();
    for (unsigned long t = 0; t < triangle_count; ++t) {
        unsigned long index0 = vertices[triangles[t].GetA()].PositionIndex;
        unsigned long index1 = vertices[triangles[t].GetB()].PositionIndex;
        unsigned long index2 = vertices[triangles[t].GetC()].PositionIndex;
        if (g_SSEAvailable) {
            GF_SSE_ADD_TRIANGLE_NORMAL(positions, normals, index0, index1, index2);
        } else {
            C3DVector p0 = positions[index0].ToC3DVector();
            C3DVector p1 = positions[index1].ToC3DVector();
            C3DVector p2 = positions[index2].ToC3DVector();
            C4DVector normal((p2 - p0) ^ (p1 - p0), 0.0f);
            VertexNormals[index0] += normal;
            VertexNormals[index1] += normal;
            VertexNormals[index2] += normal;
        }
    }
    unsigned long quad_count = program.GetRenderQuadCount();
    const C3DQuad2* quads = program.PeekRenderQuads();
    for (unsigned long q = 0; q < quad_count; ++q) {
        unsigned long index0 = vertices[quads[q].GetA()].PositionIndex;
        unsigned long index1 = vertices[quads[q].GetB()].PositionIndex;
        unsigned long index2 = vertices[quads[q].GetC()].PositionIndex;
        unsigned long index3 = vertices[quads[q].GetD()].PositionIndex;
        C3DVector p0 = positions[index0].ToC3DVector();
        C3DVector p1 = positions[index1].ToC3DVector();
        C3DVector p2 = positions[index2].ToC3DVector();
        C3DVector p3 = positions[index3].ToC3DVector();
        C4DVector normal1((p0 - p1) ^ (p2 - p1), 0.0f);
        VertexNormals[index0] += normal1;
        VertexNormals[index1] += normal1;
        VertexNormals[index2] += normal1;
        C4DVector normal2((p2 - p1) ^ (p3 - p1), 0.0f);
        VertexNormals[index1] += normal2;
        VertexNormals[index3] += normal2;
        VertexNormals[index2] += normal2;
    }
    NVectorMath::GFNormalise3DVectors(VertexNormals, sizeof(C4DVector), VertexNormals, sizeof(C4DVector), vertex_count);
}