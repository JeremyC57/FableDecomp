// CEngineWaterRenderer::BuildConstantBuffers() -- build the shared water index/vertex buffers: a 16x16-quad patch triangulated in 2x2 blocks with alternating diagonals, stripified into the foreground index buffer, plus the fixed 30-index / 14-vertex texture-quad buffers
// Leaf: ?BuildConstantBuffers@CEngineWaterRenderer@@AAEXXZ
// Flags: none
// Grade: pending
#define FABLE_RETAIL_CENGINEWATERRENDERER
#define FABLE_RETAIL_CENGINERESOURCEMANAGER
#define FABLE_RETAIL_CTRIANGLESTRIPIFIER2
#include "engine/CEngineWaterRenderer.h"
#include "engine/CEngineResourceManager.h"
#include "engine/CTriangleStripifier2.h"

extern CEngineResourceManager* g_pResourceManager;    // 0x01436E98 (descriptive; no PDB name for this global)

class CTVertexColTex1Base {                           // Ego_r.pdb, 0x18 bytes
public:
    float         X;                                  // +0x00
    float         Y;                                  // +0x04
    float         Z;                                  // +0x08
    unsigned long DiffuseColour;                      // +0x0C
    float         U1;                                 // +0x10
    float         V1;                                 // +0x14
};

void CEngineWaterRenderer::BuildConstantBuffers()
{
    CArray<unsigned short> polys;
    for (long y = 0; y < 16; y += 2) {
        for (long x = 0; x < 16; x += 2) {
            long index = x + y * 17;
            for (long j = 0; j < 2; ++j) {
                long rowStart = index;
                for (long i = 0; i < 2; ++i) {
                    if ((j & 1) == (i & 1)) {
                        polys.push_back(index);
                        polys.push_back(index + 18);
                        polys.push_back(index + 17);
                        polys.push_back(index);
                        polys.push_back(index + 1);
                        polys.push_back(index + 18);
                    } else {
                        polys.push_back(index);
                        polys.push_back(index + 1);
                        polys.push_back(index + 17);
                        polys.push_back(index + 17);
                        polys.push_back(index + 1);
                        polys.push_back(index + 18);
                    }
                    ++index;
                }
                index = rowStart + 17;
            }
        }
    }

    CArray<unsigned short> list;
    CArray<unsigned short> strip;
    CTriangleStripifier2 stripifier;
    stripifier.GenerateTriangleListAndStrip(polys, list, strip);

    long stripLength = strip.size();
    ForegroundIndexBuffers.Uninitialise();
    ForegroundIndexBuffers.Initialise(stripLength * 2, g_pResourceManager->GetIndexBufferUsage(), (EIndexBufferFormat)0x65,
                                      g_pResourceManager->GetIndexBufferMemPool(), (EPrimitiveType)5);
    unsigned char* stripData = ForegroundIndexBuffers.Lock(0, 0, 0);
    memcpy(stripData, strip.GetAsCArray(), stripLength * 2);
    ForegroundIndexBuffers.Unlock();
    WaterForegroundPolyCount = stripLength - 2;

    ForegroundTextureIndexBuffers.Uninitialise();
    ForegroundTextureIndexBuffers.Initialise(0x3c, g_pResourceManager->GetIndexBufferUsage(), (EIndexBufferFormat)0x65,
                                             g_pResourceManager->GetIndexBufferMemPool(), (EPrimitiveType)4);
    unsigned short* indices = (unsigned short*)ForegroundTextureIndexBuffers.Lock(0, 0, ForegroundTextureIndexBuffers.GetSize());
    indices[0] = 0;   indices[1] = 1;   indices[2] = 2;
    indices[3] = 0;   indices[4] = 2;   indices[5] = 3;
    indices[6] = 4;   indices[7] = 5;   indices[8] = 6;
    indices[9] = 4;   indices[10] = 6;  indices[11] = 7;
    indices[12] = 7;  indices[13] = 6;  indices[14] = 9;
    indices[15] = 7;  indices[16] = 9;  indices[17] = 8;
    indices[18] = 10; indices[19] = 11; indices[20] = 12;
    indices[21] = 10; indices[22] = 12; indices[23] = 13;
    indices[24] = 10; indices[25] = 11; indices[26] = 9;
    indices[27] = 10; indices[28] = 9;  indices[29] = 8;
    ForegroundTextureIndexBuffers.Unlock();

    ForegroundTextureVertexBuffers.Uninitialise();
    ForegroundTextureVertexBuffers.Initialise((EVertexType)0, 14, sizeof(CTVertexColTex1Base), g_pResourceManager->GetVertexBufferUsage(),
                                              (EVertexBufferPool)1);
    CTVertexColTex1Base* v = (CTVertexColTex1Base*)ForegroundTextureVertexBuffers.Lock(0, 0, ForegroundTextureVertexBuffers.GetSize());
    v[0].X = -1.0f;  v[0].Y = 1.0f;  v[0].Z = 1.0f;  v[0].U1 = 0.0f;  v[0].V1 = 0.0f;  v[0].DiffuseColour = 0xffffffff;
    v[1].X = 1.0f;  v[1].Y = 1.0f;  v[1].Z = 1.0f;  v[1].U1 = 1.0f;  v[1].V1 = 0.0f;  v[1].DiffuseColour = 0xffffffff;
    v[2].X = 1.0f;  v[2].Y = -1.0f;  v[2].Z = 1.0f;  v[2].U1 = 1.0f;  v[2].V1 = 1.0f;  v[2].DiffuseColour = 0xffffffff;
    v[3].X = -1.0f;  v[3].Y = -1.0f;  v[3].Z = 1.0f;  v[3].U1 = 0.0f;  v[3].V1 = 1.0f;  v[3].DiffuseColour = 0xffffffff;
    v[4].X = -1.0f;  v[4].Y = 0.0f;  v[4].Z = 1.0f;  v[4].U1 = 0.0f;  v[4].V1 = 0.0f;  v[4].DiffuseColour = 0x00ffffff;
    v[5].X = 1.0f;  v[5].Y = 0.0f;  v[5].Z = 1.0f;  v[5].U1 = 1.0f;  v[5].V1 = 0.0f;  v[5].DiffuseColour = 0x00ffffff;
    v[6].X = 1.0f;  v[6].Y = -0.1f;  v[6].Z = 1.0f;  v[6].U1 = 1.0f;  v[6].V1 = 0.1f;  v[6].DiffuseColour = 0xffffffff;
    v[7].X = -1.0f;  v[7].Y = -0.1f;  v[7].Z = 1.0f;  v[7].U1 = 0.0f;  v[7].V1 = 0.1f;  v[7].DiffuseColour = 0xffffffff;
    v[8].X = -1.0f;  v[8].Y = -1.0f;  v[8].Z = 1.0f;  v[8].U1 = 0.0f;  v[8].V1 = 1.0f;  v[8].DiffuseColour = 0xffffffff;
    v[9].X = 1.0f;  v[9].Y = -1.0f;  v[9].Z = 1.0f;  v[9].U1 = 1.0f;  v[9].V1 = 1.0f;  v[9].DiffuseColour = 0xffffffff;
    v[10].X = -1.0f;  v[10].Y = 1.0f;  v[10].Z = 1.0f;  v[10].U1 = 0.0f;  v[10].V1 = 0.0f;  v[10].DiffuseColour = 0xffffffff;
    v[11].X = 1.0f;  v[11].Y = 1.0f;  v[11].Z = 1.0f;  v[11].U1 = 1.0f;  v[11].V1 = 0.0f;  v[11].DiffuseColour = 0xffffffff;
    v[12].X = 1.0f;  v[12].Y = -0.1f;  v[12].Z = 1.0f;  v[12].U1 = 1.0f;  v[12].V1 = 1.0f;  v[12].DiffuseColour = 0xffffffff;
    v[13].X = -1.0f;  v[13].Y = -0.1f;  v[13].Z = 1.0f;  v[13].U1 = 0.0f;  v[13].V1 = 1.0f;  v[13].DiffuseColour = 0xffffffff;
    ForegroundTextureVertexBuffers.Unlock();
}
