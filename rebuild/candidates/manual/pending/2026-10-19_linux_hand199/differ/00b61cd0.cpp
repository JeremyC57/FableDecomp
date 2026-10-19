// CEngineSkyRenderer::BuildBaseBandMesh(CTVertexColTex1** vertices, unsigned short** indices, long indexOffset, long vertexOffset) -- write the sky's base band: a centre vertex 10000 below, a closed ring of 36 vertices (radius 6500, 500 below, white) and the 35+1 triangles of the fan joining them
// Grade: DIFFER(201v209) -- retail hoists the ring Z (-500.0f) into ebp and runs the vertex walk as eax (current) / edx (next) with the ring counter in ecx; ours stores Z as an immediate and walks with ecx / edx; the store order and the index loop agree
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B61CD0, length 224 bytes (ret 10h; `this` unused).
// Callees: the fcos/fsin intrinsics (cos/sin inline). Float pool: 0.0f 0x0122DEDC, 6500.0f 0x012A2930, 2*pi/35 0x012A2928.
// Layout: CTVertexColTex1 (0x18): Pos X/Y/Z +0, Colour +0xC, U +0x10, V +0x14.
// Shape: the ring angle lives on the FPU stack (reset to 0 on the last vertex so the ring closes exactly); the constant
// stores of each ring vertex are hoisted into registers (Z in ebp, colour -1 in ebx, 0 in edi) ahead of the two
// transcendental stores; the index loop runs i = 1..35 with the fan indices vertexOffset, vertexOffset+i+1,
// vertexOffset+i, and the closing triangle reuses i (= 36) after the loop.
#include <math.h>

struct CTVertexColTex1 {
    float         X, Y, Z;                      // +0x00
    unsigned long Colour;                       // +0x0c
    float         U, V;                         // +0x10
};

class CEngineSkyRenderer {
public:
    void BuildBaseBandMesh(CTVertexColTex1** vertices, unsigned short** indices, long indexOffset, long vertexOffset);
};

void CEngineSkyRenderer::BuildBaseBandMesh(CTVertexColTex1** vertices, unsigned short** indices, long indexOffset,
                                           long vertexOffset)
{
    CTVertexColTex1* vertex = *vertices + vertexOffset;
    vertex->X = 0.0f;
    vertex->Y = 0.0f;
    vertex->Z = -10000.0f;
    vertex->U = 0.0f;
    vertex->V = 0.0f;
    vertex->Colour = 0;
    ++vertex;
    float angle = 0.0f;
    for (long ring = 0; ring < 36; ++ring) {
        if (ring == 35)
            angle = 0.0f;
        CTVertexColTex1* rim = vertex++;
        rim->X = (float)cos(angle) * 6500.0f;
        rim->Y = (float)sin(angle) * 6500.0f;
        rim->Z = -500.0f;
        rim->V = 0.0f;
        rim->U = 0.0f;
        rim->Colour = 0xffffffff;
        angle += 0.17951958f;
    }
    unsigned short* index = *indices + indexOffset;
    long count = 0;
    long i;
    for (i = 1; i < 36; ++i) {
        index[count++] = (unsigned short)vertexOffset;
        index[count++] = (unsigned short)(vertexOffset + 1 + i);
        index[count++] = (unsigned short)(vertexOffset + i);
    }
    index[count++] = (unsigned short)vertexOffset;
    index[count++] = (unsigned short)(vertexOffset + 1);
    index[count++] = (unsigned short)(vertexOffset + i);
}
