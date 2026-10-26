// Behaviour test for 00b61b30: src/00b61b30.cpp copied verbatim (shared headers expanded inline), callees stubbed below so calls
// and arguments are observable. CEngineSkyRenderer::BuildOuterSkyMesh(CTVertexColTex1**, unsigned short**, long, long)
#include <cstdio>
#include <cstring>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
// ---- source (verbatim) ----
// CEngineSkyRenderer::BuildOuterSkyMesh(CTVertexColTex1**, unsigned short**, long, long) -- fill the outer sky dome: a top centre vertex and a 36-step ring of low/high vertex pairs (radius 6500, z -500 / 7000), then the cap fan and the band quads as an index list
// Leaf: BuildOuterSkyMesh
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B61B30, length 416 bytes, `ret 0x10` (this unused).
// Layout: PDB CTVertexColTex1Base (24 bytes): X +0, Y +4, Z +8, DiffuseColour +0xc, U1 +0x10, V1 +0x14.
// Constants: 0.0001f @ 0x0129BA3C (cap texcoord), 0.0f @ 0x0122DEDC, 6500.0f @ 0x012A2930 (ring radius),
//   1/35 @ 0x012A292C (u step), 2pi/35 @ 0x012A2928 (angle step).
// Shape: the running index is declared at the index fill (declared at the top it takes eax early and the loop-invariant
//   0xffffffff colour loses its register); the centre's colour is stored after both texcoords; angle and u are declared
//   before the ring pointer (u's zero store precedes `lea edx,[ecx+18h]`).
extern "C" double __cdecl cos(double);
extern "C" double __cdecl sin(double);
#pragma intrinsic(cos, sin)

class CTVertexColTex1 { public:
    float         X;                               // +0x00
    float         Y;                               // +0x04
    float         Z;                               // +0x08
    unsigned long DiffuseColour;                   // +0x0c
    float         U1;                              // +0x10
    float         V1;                              // +0x14
};

extern float g_SkyCapTexCoordBias;                 // 0x0129BA3C (0.0001f, a writable global: loaded and negated)

class CEngineSkyRenderer { public:
    void BuildOuterSkyMesh(CTVertexColTex1** vertices, unsigned short** indices, long indexOffset, long vertexOffset);
};

void CEngineSkyRenderer::BuildOuterSkyMesh(CTVertexColTex1** vertices, unsigned short** indices, long indexOffset,
                                           long vertexOffset)
{
    CTVertexColTex1* centre = &(*vertices)[vertexOffset];
    centre->X = 0.0f;
    centre->Y = 0.0f;
    centre->Z = 7000.0f;
    centre->U1 = -g_SkyCapTexCoordBias;
    centre->V1 = -g_SkyCapTexCoordBias;
    centre->DiffuseColour = 0;
    float angle = 0.0f;
    float u = 0.0f;
    CTVertexColTex1* vertex = centre + 1;
    unsigned long i;
    for (i = 0; i < 36; ++i) {
        if (i == 35)
            angle = 0.0f;
        float c = (float)cos(angle);
        float s = (float)sin(angle);
        CTVertexColTex1* lower = vertex++;
        lower->Z = -500.0f;
        lower->V1 = 1.0f;
        lower->U1 = u;
        lower->DiffuseColour = 0xffffffff;
        lower->X = c * 6500.0f;
        lower->Y = s * 6500.0f;
        CTVertexColTex1* upper = vertex++;
        upper->X = c * 6500.0f;
        upper->Y = s * 6500.0f;
        upper->Z = 7000.0f;
        upper->V1 = 0.0f;
        upper->U1 = u;
        upper->DiffuseColour = 0;
        u += 0.028571429f;
        angle += 0.17951958f;
    }
    long index = 0;
    unsigned short* index16 = *indices + indexOffset;
    long k = 2;
    for (i = 0; i < 35; ++i) {
        index16[index++] = (unsigned short)vertexOffset;
        index16[index++] = (unsigned short)(vertexOffset + k);
        k += 2;
        index16[index++] = (unsigned short)(vertexOffset + k);
    }
    index16[index++] = (unsigned short)vertexOffset;
    index16[index++] = (unsigned short)(vertexOffset + k);
    index16[index++] = (unsigned short)(vertexOffset + 2);
    k = 1;
    for (i = 0; i < 35; ++i) {
        index16[index++] = (unsigned short)(vertexOffset + k);
        index16[index++] = (unsigned short)(vertexOffset + k + 2);
        index16[index++] = (unsigned short)(vertexOffset + k + 1);
        index16[index++] = (unsigned short)(vertexOffset + k + 2);
        index16[index++] = (unsigned short)(vertexOffset + k + 3);
        index16[index++] = (unsigned short)(vertexOffset + k + 1);
        k += 2;
    }
    index16[index++] = (unsigned short)(vertexOffset + k);
    index16[index++] = (unsigned short)(vertexOffset + 1);
    index16[index++] = (unsigned short)(vertexOffset + k + 1);
    index16[index++] = (unsigned short)(vertexOffset + 1);
    index16[index++] = (unsigned short)(vertexOffset + 2);
    index16[index++] = (unsigned short)(vertexOffset + k + 1);
}

// ---- stubs and driver ----
float g_SkyCapTexCoordBias = 0.0001f;
static float fabsf_(float x) { return x < 0 ? -x : x; }
int main()
{
    static CTVertexColTex1 verts[90];
    static unsigned short idx[340];
    memset(verts, 0xcd, sizeof verts); memset(idx, 0xcd, sizeof idx);
    CTVertexColTex1* vp = verts; unsigned short* ip = idx;
    CEngineSkyRenderer r;
    const long vo = 5, io = 7;
    r.BuildOuterSkyMesh(&vp, &ip, io, vo);
    // centre
    CTVertexColTex1& c0 = verts[vo];
    CHECK(c0.X == 0.0f && c0.Y == 0.0f && c0.Z == 7000.0f && c0.DiffuseColour == 0);
    CHECK(c0.U1 == -0.0001f && c0.V1 == -0.0001f);
    CHECK(verts[vo - 1].DiffuseColour == 0xcdcdcdcd && verts[vo + 73].DiffuseColour == 0xcdcdcdcd);   // bounds
    // ring: 36 low/high pairs, the last pair back at angle 0
    float u = 0.0f;
    for (int k = 0; k < 36; ++k) {
        float a = (k == 35) ? 0.0f : k * 0.17951958f;
        const CTVertexColTex1& lo = verts[vo + 1 + 2 * k];
        const CTVertexColTex1& hi = verts[vo + 2 + 2 * k];
        CHECK(fabsf_(lo.X - (float)cos(a) * 6500.0f) < 0.05f && fabsf_(lo.Y - (float)sin(a) * 6500.0f) < 0.05f);
        CHECK(lo.Z == -500.0f && lo.V1 == 1.0f && lo.DiffuseColour == 0xffffffff && fabsf_(lo.U1 - u) < 1e-5f);
        CHECK(hi.X == lo.X && hi.Y == lo.Y && hi.Z == 7000.0f && hi.V1 == 0.0f && hi.DiffuseColour == 0 && hi.U1 == lo.U1);
        u += 0.028571429f;
    }
    // indices: cap fan (35 + closing), then 35 band quads + closing quad
    unsigned short e[340]; int n = 0;
    long kk = 2;
    for (int t = 0; t < 35; ++t) { e[n++] = (unsigned short)vo; e[n++] = (unsigned short)(vo + kk); kk += 2; e[n++] = (unsigned short)(vo + kk); }
    e[n++] = (unsigned short)vo; e[n++] = (unsigned short)(vo + kk); e[n++] = (unsigned short)(vo + 2);
    kk = 1;
    for (int t = 0; t < 35; ++t) {
        e[n++] = (unsigned short)(vo + kk); e[n++] = (unsigned short)(vo + kk + 2); e[n++] = (unsigned short)(vo + kk + 1);
        e[n++] = (unsigned short)(vo + kk + 2); e[n++] = (unsigned short)(vo + kk + 3); e[n++] = (unsigned short)(vo + kk + 1);
        kk += 2;
    }
    e[n++] = (unsigned short)(vo + kk); e[n++] = (unsigned short)(vo + 1); e[n++] = (unsigned short)(vo + kk + 1);
    e[n++] = (unsigned short)(vo + 1); e[n++] = (unsigned short)(vo + 2); e[n++] = (unsigned short)(vo + kk + 1);
    CHECK(n == 324);
    int bad = 0; for (int t = 0; t < n; ++t) if (idx[io + t] != e[t]) ++bad;
    CHECK(bad == 0);
    CHECK(idx[io - 1] == 0xcdcd && idx[io + 324] == 0xcdcd);
    CHECK(idx[io + 105] == vo && idx[io + 106] == vo + 72 && idx[io + 107] == vo + 2);   // closing fan triangle
    if (fails == 0) printf("00b61b30_TEST PASS\n");
    return fails;
}
