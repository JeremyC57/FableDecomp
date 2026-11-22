// CEngineSkyRenderer::BuildCloudMesh(CTVertexColTex1**, unsigned short**, long, long) -- builds the cloud dome mesh.
// Leaf: ?BuildCloudMesh@CEngineSkyRenderer@@AAEXPAPAVCTVertexColTex1@@PAPAGJJ@Z
// Flags: default
// Original static constants: Engine_Sky_Renderer.obj CodeView S_LDATA32 records.
#define FABLE_RETAIL_CENGINESKYRENDERER_BUILDCLOUDMESH
#include "engine/CEngineSkyRenderer.h"
#define FABLE_RETAIL_CTVERTEXCOLTEX1_SETTERS
#include "engine/CTVertexColTex1.h"
#include "engine/GFSin.h"
#include "engine/GFCos.h"
#include <math.h>
#pragma intrinsic(cos, sin)

static const float CLOUD_LOW_ANGLE = 0.47123894f;
static const float CLOUD_MAX_XY_DIAMETER = cos(CLOUD_LOW_ANGLE) * 13000.0f;
static const float CLOUD_HIGH_ANGLE = 1.5707964f;
// The fixture changes this value once to exercise the alpha clamp.
static float CLOUD_V_ANGLE_FACTOR = (float)((CLOUD_LOW_ANGLE - CLOUD_HIGH_ANGLE) / 8.0);
static const float SKY_ONE_ON_STEPS_H = (float)(1.0 / 36.0);

void CEngineSkyRenderer::BuildCloudMesh(CTVertexColTex1** verts_ptr, unsigned short** indices_ptr,
                                        long index_offset, long vertex_offset)
{
    CTVertexColTex1* verts = &(*verts_ptr)[vertex_offset];
    long v = 0;
    for (long cv = 0; cv < 9; ++cv) {
        float vangle = cv * CLOUD_V_ANGLE_FACTOR + CLOUD_HIGH_ANGLE;
        float cos_vangle = (float)cos(vangle);
        float sin_vangle = (float)sin(vangle);
        float zpos = 3250.0f * sin_vangle + CloudHeightOffset;
        float xyradius = cos_vangle * 6500.0f;
        float texture_coord_multiplier = (CloudTextureOffset - sin(vangle)) * CloudTextureMulitplier;
        for (long ch = 0; ch < 37; ++ch) {
            int ch2 = ch < 36 ? ch : 0;
            float xpos = (float)(xyradius * GFSin(ch2 * SKY_ONE_ON_STEPS_H));
            float ypos = (float)(xyradius * GFCos(ch2 * SKY_ONE_ON_STEPS_H));
            CTVertexColTex1& vertex = verts[v++];
            vertex.SetPosition(C3DVector(xpos, ypos, zpos));
            vertex.U1 = (xpos / CLOUD_MAX_XY_DIAMETER) * texture_coord_multiplier;
            vertex.V1 = (ypos / CLOUD_MAX_XY_DIAMETER) * texture_coord_multiplier;
            vertex.DiffuseColour = 0x00ffffff;
            vertex.DiffuseColour +=
                (unsigned long)(unsigned char)((0.0f > 1.0f - cos_vangle * 1.105f
                    ? 0.0f : 1.0f - cos_vangle * 1.105f) * 255.0f) << 24;
        }
    }
    unsigned short* ids = &(*indices_ptr)[index_offset];
    long index = 0;
    for (long cv = 0; cv < 8; ++cv) {
        for (long ch = 0; ch < 36; ++ch) {
            long v0 = cv * 37 + ch;
            long v2 = (cv + 1) * 37 + ch;
            long v1 = (cv + 1) * 37 + ch + 1;
            long v3 = cv * 37 + ch + 1;
            ids[index++] = (unsigned short)(vertex_offset + v0);
            ids[index++] = (unsigned short)(vertex_offset + v1);
            ids[index++] = (unsigned short)(vertex_offset + v2);
            ids[index++] = (unsigned short)(vertex_offset + v1);
            ids[index++] = (unsigned short)(vertex_offset + v0);
            ids[index++] = (unsigned short)(vertex_offset + v3);
        }
    }
}

#include <stdio.h>
#include <string.h>

static const float CloudTextureExtent = 11583.084f;
static const float CloudRingAngleStep = -0.13744468f;
static const float CloudSegmentStep = 1.0f / 36.0f;
static const double TWO_PI = 6.2831854820251465;

static bool EqualVertexComponent(float built, double expected)
{
    return fabs(built - expected) < 0.01;
}

int main()
{
    CEngineSkyRenderer renderer;
    renderer.CloudHeightOffset = 11.0f;
    renderer.CloudTextureOffset = 0.75f;
    renderer.CloudTextureMulitplier = 1.25f;
    CTVertexColTex1 vertices[335];
    unsigned short indices[1730];
    memset(vertices, 0, sizeof(vertices));
    memset(indices, 0xff, sizeof(indices));
    vertices[0].DiffuseColour = 0x11223344UL;
    vertices[334].DiffuseColour = 0x55667788UL;
    CTVertexColTex1* pvertices = vertices;
    unsigned short* pindices = indices;
    renderer.BuildCloudMesh(&pvertices, &pindices, 1, 1);
    if (pvertices != vertices || pindices != indices || indices[0] != 0xffff
        || indices[1729] != 0xffff || vertices[0].DiffuseColour != 0x11223344UL
        || vertices[334].DiffuseColour != 0x55667788UL)
        return 1;

    for (long ring = 0; ring < 9; ++ring) {
        const float angle = ring * CloudRingAngleStep + 1.5707964f;
        const double sa = sin(angle);
        const double ca = cos(angle);
        const double radius = ca * 6500.0;
        const double height = 3250.0 * sa + renderer.CloudHeightOffset;
        const double tex = (renderer.CloudTextureOffset - sa) * renderer.CloudTextureMulitplier;
        const double opacity = 1.0 - ca * 1.105f;
        const unsigned long alpha = opacity < 0.0 ? 0 : (unsigned char)(opacity * 255.0);
        for (long segment = 0; segment < 37; ++segment) {
            const double theta = (segment == 36 ? 0.0f : segment * CloudSegmentStep) * TWO_PI;
            const double x = radius * sin(theta);
            const double y = radius * cos(theta);
            const CTVertexColTex1& vertex = vertices[1 + ring * 37 + segment];
            if (!EqualVertexComponent(vertex.X, x) || !EqualVertexComponent(vertex.Y, y)
                || !EqualVertexComponent(vertex.Z, height) || !EqualVertexComponent(vertex.U1, x / CloudTextureExtent * tex)
                || !EqualVertexComponent(vertex.V1, y / CloudTextureExtent * tex)
                || vertex.DiffuseColour != ((alpha << 24) | 0x00ffffffUL))
                return 2;
        }
    }
    long i = 1;
    for (unsigned short row = 0; row < 8; ++row) {
        for (unsigned short column = 0; column < 36; ++column) {
            const unsigned short a = (unsigned short)(1 + row * 37 + column);
            const unsigned short b = (unsigned short)(a + 1);
            const unsigned short c = (unsigned short)(a + 37);
            const unsigned short d = (unsigned short)(c + 1);
            const unsigned short expected[] = {a, d, c, d, a, b};
            for (unsigned int vertex = 0; vertex < 6; ++vertex)
                if (indices[i++] != expected[vertex])
                    return 3;
        }
    }
    CLOUD_V_ANGLE_FACTOR = -0.2f;
    renderer.BuildCloudMesh(&pvertices, &pindices, 1, 1);
    if (vertices[1 + 7 * 37].DiffuseColour != 0x00ffffffUL
        || (vertices[1].DiffuseColour >> 24) == 0)
        return 4;
    puts("00b61dd0_TEST PASS");
    return 0;
}