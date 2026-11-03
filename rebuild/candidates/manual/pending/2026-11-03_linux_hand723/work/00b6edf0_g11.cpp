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
#include "engine/C3DTriangle2.h"

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
            long index = x;
            index += y * 17;
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
    C3DTriangle2* triangles = (C3DTriangle2*)ForegroundTextureIndexBuffers.Lock(0, 0, ForegroundTextureIndexBuffers.GetSize());
    triangles[0].SetA(0);  triangles[0].SetB(1);  triangles[0].SetC(2);
    triangles[1].SetA(0);  triangles[1].SetB(2);  triangles[1].SetC(3);
    triangles[2].SetA(4);  triangles[2].SetB(5);  triangles[2].SetC(6);
    triangles[3].SetA(4);  triangles[3].SetB(6);  triangles[3].SetC(7);
    triangles[4].SetA(7);  triangles[4].SetB(6);  triangles[4].SetC(9);
    triangles[5].SetA(7);  triangles[5].SetB(9);  triangles[5].SetC(8);
    triangles[6].SetA(10);  triangles[6].SetB(11);  triangles[6].SetC(12);
    triangles[7].SetA(10);  triangles[7].SetB(12);  triangles[7].SetC(13);
    triangles[8].SetA(10);  triangles[8].SetB(11);  triangles[8].SetC(9);
    triangles[9].SetA(10);  triangles[9].SetB(9);  triangles[9].SetC(8);
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
