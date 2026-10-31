// CEngineSkyRenderer::BuildCloudMesh(CTVertexColTex1** vertices, unsigned short** indices, long first_index, long first_vertex) -- fill the cloud dome: 9 rings of 37 vertices (zenith down to 27 degrees, radius 6500, height 3250*sin + CloudHeightOffset, u/v scaled by the cloud texture offset and multiplier, alpha fading towards the horizon), then 8 x 36 quads as an index list
// Grade: DIFFER(521v532) 31 diff lines (parked 33) -- u/v as texture_scale * (inv_extent * x) gives retail's fld st(2) ; fmul st(1) ; fmul [scale] for U; retail keeps the vertex counter in a stack slot and bumps it in ecx after the theta `fild` (ours bumps it at the loop top in eax and keeps `esi = ebp`, `add ebp,18h` ahead of the colour store), and retail folds the u/v products as (1/extent * x) * scale with the 2*pi `fmul qword` kept in the theta chain where ours reloads the scale from the frame; ring maths, index list, alpha clamp/_ftol and the frame otherwise identical (four grades: sin/cos order, index locals, u/v forms, counter placement)
// Leaf: ?BuildCloudMesh@CEngineSkyRenderer@@AAEXPAPAVCTVertexColTex1@@PAPAGJJ@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00B61DD0, 532 bytes, thiscall `ret 10h`.
// Identity (proven): Ego_r.pdb donor order BuildOuterSkyMesh 0x00A14D80, BuildBaseBandMesh 0x00A14F90, FrameStart
//   0x00A150A0, RenderStarField 0x00A150B0, BuildCloudMesh 0x00A150C0 = retail 0x00B61B30, 0x00B61CD0, 0x00B61DB0 (all
//   three landed), 0x00B61DC0, this row; Ego_r.pdb ?BuildCloudMesh@CEngineSkyRenderer@@AAEXPAPAVCTVertexColTex1@@PAPAGJJ@Z
//   has the same four arguments as its siblings (`ret 10h`); the landed BuildBuffers 0x00B620A0 calls it with
//   (vertices, indices, 0x144, 0x49) between the outer-sky and base-band meshes. The catalog's
//   "BuildBuffers_BuildGlowParticles" is a placeholder.
// Callees: _ftol 0x00BFEA70 (the alpha byte); intrinsic fsin / fcos.
// Data: 1.0f 0x0122DED8, pi/2 0x012A28FC, 3250.0f 0x012A2934, 6500.0f 0x012A2930, 1.105f 0x012A2900, 2*pi (double)
//   0x0124F2B8, 0.0f 0x0122DEDC, 255.0f 0x01230014 (literal pool); writable globals 0x0139A710 = -0.35*pi/8 (ring angle
//   step) and 0x0139A70C = 1/36 (segment step), and 0x0143782C = cos(0.15*pi) * 13000 (dynamic initializer 0x01224830,
//   landed with the descriptive name kept here).
// Layout: engine/CEngineSkyRenderer.h (CloudHeightOffset +0xc, CloudTextureOffset +0x10, CloudTextureMulitplier +0x14);
//   CTVertexColTex1 (24 bytes) {X, Y, Z, DiffuseColour +0xc, U1 +0x10, V1 +0x14} as the landed siblings.
#define CEngineSkyRenderer CEngineSkyRendererLayout
#include "engine/CEngineSkyRenderer.h"
#undef CEngineSkyRenderer
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

extern float g_CUnprovenCosThreshold_0143782c;     // 0x0143782C (cos(0.15*pi) * 13000, dynamic initializer)
extern float g_CloudRingAngleStep_0139a710;        // 0x0139A710 (-0.35*pi/8)
extern float g_CloudSegmentStep_0139a70c;          // 0x0139A70C (1/36)

class CEngineSkyRenderer : public CEngineSkyRendererLayout {
private:
    void BuildCloudMesh(CTVertexColTex1** vertices, unsigned short** indices, long first_index, long first_vertex);
};

void CEngineSkyRenderer::BuildCloudMesh(CTVertexColTex1** vertices, unsigned short** indices, long first_index,
                                        long first_vertex)
{
    float inv_extent = 1.0f / g_CUnprovenCosThreshold_0143782c;
    CTVertexColTex1* base = &(*vertices)[first_vertex];
    long vertex = 0;
    for (long ring = 0; ring < 9; ++ring) {
        float angle = ring * g_CloudRingAngleStep_0139a710 + 1.5707964f;
        float c = (float)cos(angle);
        float s = (float)sin(angle);
        float height = 3250.0f * s + CloudHeightOffset;
        float radius = c * 6500.0f;
        float texture_scale = (CloudTextureOffset - s) * CloudTextureMulitplier;
        float alpha = 1.0f - c * 1.105f;
        for (long segment = 0; segment < 37; ++segment) {
            CTVertexColTex1* v = &base[vertex++];
            v->DiffuseColour = 0x00ffffff;
            float theta = (float)((segment < 36 ? segment : 0) * g_CloudSegmentStep_0139a70c * 6.2831854820251465);
            v->Z = height;
            float x = (float)sin(theta) * radius;
            float y = (float)cos(theta) * radius;
            v->X = x;
            v->Y = y;
            v->U1 = texture_scale * (inv_extent * x);
            v->V1 = texture_scale * (inv_extent * y);
            v->DiffuseColour += (unsigned long)(unsigned char)((0.0f > alpha ? 0.0f : alpha) * 255.0f) << 24;
        }
    }
    unsigned short* index = &(*indices)[first_index];
    long n = 0;
    for (long row = 0; row < 8; ++row) {
        for (long segment = 0; segment < 36; ++segment) {
            long top_left = row * 37 + segment;
            long bottom_right = (row + 1) * 37 + segment + 1;
            index[n++] = (unsigned short)(first_vertex + top_left);
            index[n++] = (unsigned short)(first_vertex + bottom_right);
            index[n++] = (unsigned short)(first_vertex + (row + 1) * 37 + segment);
            index[n++] = (unsigned short)(first_vertex + bottom_right);
            index[n++] = (unsigned short)(first_vertex + top_left);
            index[n++] = (unsigned short)(first_vertex + row * 37 + segment + 1);
        }
    }
}
