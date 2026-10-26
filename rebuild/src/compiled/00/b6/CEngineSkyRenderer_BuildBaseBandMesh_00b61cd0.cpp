// CEngineSkyRenderer::BuildBaseBandMesh(CArray<SSkyVertex>&, CArray<unsigned short>&, long, long) -- build the sky's base band: a centre vertex 10000 below the camera, a ring of 36 vertices of radius 6500 at depth 500 (the last one closing the ring at angle 0), and the 36-triangle fan joining them
// Leaf: BuildBaseBandMesh
// Retail: Fable.exe (TLC, VC7.1) @ 0x00B61CD0, 209 bytes (+ int3 padding), `ret 10h` (`this` unused).
// Callees: none (intrinsic fcos/fsin). Float pool: 0x0122DEDC = 0.0f, 0x012A2930 = 6500.0f, 0x012A2928 = 2*pi/35
//   (0.17951958f); -10000.0f and -500.0f are immediates.
// Layout: the vertex (24 bytes) {X, Y, Z, Colour +0xc, U +0x10, V +0x14}; both arrays are reached through their first word
//   (the data pointer).
// Shape: the angle lives on the x87 stack for the whole loop (`if (i == 35) angle = 0.0f;` reloads the zero); each ring
//   vertex's constants are stored before its cos/sin terms; the fan loop writes (base, base + j + 1, base + j) for j = 1..35
//   (a separate triangle counter, which VC turns into a count-down) with a running index, then the closing triangle (base,
//   base + 1, base + 36); the first vertex is the LAST argument (after the index array and the first index).
#include <math.h>
#pragma intrinsic(cos, sin)

struct SSkyVertex {                                    // 24 bytes
    float         X, Y, Z;                             // +0x00
    unsigned long Colour;                              // +0x0c
    float         U, V;                                // +0x10
};
template <class T> class CArray { public:
    T* Data;
};

class CEngineSkyRenderer { public:
    void BuildBaseBandMesh(CArray<SSkyVertex>& vertices, CArray<unsigned short>& indices, long firstIndex, long firstVertex);
};

void CEngineSkyRenderer::BuildBaseBandMesh(CArray<SSkyVertex>& vertices, CArray<unsigned short>& indices, long firstIndex, long firstVertex)
{
    float angle = 0.0f;
    SSkyVertex* centre = &vertices.Data[firstVertex];
    centre->X = 0.0f;
    centre->Y = 0.0f;
    centre->Z = -10000.0f;
    centre->U = 0.0f;
    centre->V = 0.0f;
    centre->Colour = 0;
    SSkyVertex* vertex = centre + 1;
    for (unsigned long i = 0; i < 36; ++i) {
        if (i == 35)
            angle = 0.0f;
        SSkyVertex* ring = vertex++;
        ring->Z = -500.0f;
        ring->V = 0.0f;
        ring->U = 0.0f;
        ring->Colour = 0xffffffff;
        ring->X = (float)(cos(angle) * 6500.0f);
        ring->Y = (float)(sin(angle) * 6500.0f);
        angle += 0.17951958f;
    }
    unsigned short* index = &indices.Data[firstIndex];
    long n = 0;
    unsigned long j = 1;
    for (long triangle = 0; triangle < 35; ++triangle, ++j) {
        index[n++] = (unsigned short)firstVertex;
        index[n++] = (unsigned short)(firstVertex + 1 + j);
        index[n++] = (unsigned short)(firstVertex + j);
    }
    index[n++] = (unsigned short)firstVertex;
    index[n++] = (unsigned short)(firstVertex + 1);
    index[n++] = (unsigned short)(firstVertex + j);
}