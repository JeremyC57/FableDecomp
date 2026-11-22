// CEngineSkyRenderer::BuildCloudMesh(CTVertexColTex1**, unsigned short**, long, long) -- builds the cloud dome mesh.
// Leaf: ?BuildCloudMesh@CEngineSkyRenderer@@AAEXPAPAVCTVertexColTex1@@PAPAGJJ@Z
// Flags: default
// Original static constants: Engine_Sky_Renderer.obj CodeView S_LDATA32 records.
#define FABLE_RETAIL_CENGINESKYRENDERER
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
static const float CLOUD_V_ANGLE_FACTOR = (float)((CLOUD_LOW_ANGLE - CLOUD_HIGH_ANGLE) / 8.0);
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