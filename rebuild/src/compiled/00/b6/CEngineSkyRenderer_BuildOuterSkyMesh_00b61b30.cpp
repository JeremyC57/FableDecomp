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