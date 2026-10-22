// Behaviour test for 00b61cd0: final.cpp copied verbatim (32-bit `long` under g++; build with `g++ -w -fpermissive`).
// No callees: checks the centre vertex, the 36-vertex ring (cos/sin of the running angle, the last one at angle 0) and
// the 36-triangle fan indices written from firstIndex. CEngineSkyRenderer::BuildBaseBandMesh
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <math.h>
#ifndef _MSC_VER
#include <stdint.h>
#define __forceinline inline
#define __declspec(x)
#define __fastcall
#define __cdecl
#define __int64 int64_t
#define long int
#endif
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
// ---- source (verbatim) ----
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
template <class T> struct CArray {
    T* Data;
};

struct CEngineSkyRenderer {
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

// ---- test ----
static float Abs(float v) { return v < 0.0f ? -v : v; }
int main()
{
    const long firstVertex = 5;
    const long firstIndex = 7;
    SSkyVertex verts[5 + 37 + 2];
    unsigned short idx[7 + 108 + 2];
    std::memset(verts, 0x5a, sizeof(verts));
    std::memset(idx, 0xa5, sizeof(idx));
    CArray<SSkyVertex> vertices; vertices.Data = verts;
    CArray<unsigned short> indices; indices.Data = idx;
    CEngineSkyRenderer renderer;
    renderer.BuildBaseBandMesh(vertices, indices, firstIndex, firstVertex);

    // untouched guards
    unsigned char guard[sizeof(SSkyVertex)]; std::memset(guard, 0x5a, sizeof(guard));
    CHECK(std::memcmp(&verts[4], guard, sizeof(guard)) == 0);
    CHECK(std::memcmp(&verts[5 + 37], guard, sizeof(guard)) == 0);
    CHECK(idx[6] == 0xa5a5 && idx[7 + 108] == 0xa5a5);

    // centre vertex
    const SSkyVertex& c = verts[firstVertex];
    CHECK(c.X == 0.0f && c.Y == 0.0f && c.Z == -10000.0f && c.U == 0.0f && c.V == 0.0f && c.Colour == 0);

    // ring
    float angle = 0.0f;
    for (int i = 0; i < 36; ++i) {
        const SSkyVertex& r = verts[firstVertex + 1 + i];
        if (i == 35) angle = 0.0f;
        float ex = (float)(cos(angle) * 6500.0f);
        float ey = (float)(sin(angle) * 6500.0f);
        CHECK(Abs(r.X - ex) < 0.01f);
        CHECK(Abs(r.Y - ey) < 0.01f);
        CHECK(r.Z == -500.0f && r.U == 0.0f && r.V == 0.0f && r.Colour == 0xffffffff);
        angle += 0.17951958f;
    }
    CHECK(verts[firstVertex + 36].X == 6500.0f && verts[firstVertex + 36].Y == 0.0f);
    CHECK(Abs(verts[firstVertex + 1].X - 6500.0f) < 0.01f);
    CHECK(Abs(verts[firstVertex + 2].X - 6500.0f * 0.98392f) < 1.0f);

    // fan
    int n = 0;
    for (int j = 1; j <= 35; ++j) {
        CHECK(idx[firstIndex + n] == firstVertex); ++n;
        CHECK(idx[firstIndex + n] == firstVertex + 1 + j); ++n;
        CHECK(idx[firstIndex + n] == firstVertex + j); ++n;
    }
    CHECK(idx[firstIndex + n] == firstVertex); ++n;
    CHECK(idx[firstIndex + n] == firstVertex + 1); ++n;
    CHECK(idx[firstIndex + n] == firstVertex + 36); ++n;
    CHECK(n == 108);

    if (fails == 0) printf("00b61cd0_TEST PASS\n"); else printf("FAILS: %d\n", fails);
    return fails ? 1 : 0;
}