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
#include "engine/C3DVector.h"

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
template <class TBase> class CGenericVertex : public TBase {   // Ego_r.pdb / FableWin.pdb ?SetPosition@?$CGenericVertex@... (inline)
public:
    void SetPosition(const C3DVector& position) { this->X = position.X; this->Y = position.Y; this->Z = position.Z; }
    void SetDiffuseColour(unsigned long colour) { this->DiffuseColour = colour; }
    void SetU1V1(float u, float v) { this->U1 = u; this->V1 = v; }
};
class CTVertexColTex1 : public CGenericVertex<CTVertexColTex1Base> { };

void CEngineWaterRenderer::BuildConstantBuffers()
{
    CArray<unsigned short> polys;
    for (long y = 0; y < 16; y += 2) {
        long rowBase = y * 17;
        long x;
        for (x = 0; x < 16; x += 2) {
            long index = x + rowBase;
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
    triangles[0].SetVertexIndex(0, 0);  triangles[0].SetVertexIndex(1, 1);  triangles[0].SetVertexIndex(2, 2);
    triangles[1].SetVertexIndex(0, 0);  triangles[1].SetVertexIndex(1, 2);  triangles[1].SetVertexIndex(2, 3);
    triangles[2].SetVertexIndex(0, 4);  triangles[2].SetVertexIndex(1, 5);  triangles[2].SetVertexIndex(2, 6);
    triangles[3].SetVertexIndex(0, 4);  triangles[3].SetVertexIndex(1, 6);  triangles[3].SetVertexIndex(2, 7);
    triangles[4].SetVertexIndex(0, 7);  triangles[4].SetVertexIndex(1, 6);  triangles[4].SetVertexIndex(2, 9);
    triangles[5].SetVertexIndex(0, 7);  triangles[5].SetVertexIndex(1, 9);  triangles[5].SetVertexIndex(2, 8);
    triangles[6].SetVertexIndex(0, 10);  triangles[6].SetVertexIndex(1, 11);  triangles[6].SetVertexIndex(2, 12);
    triangles[7].SetVertexIndex(0, 10);  triangles[7].SetVertexIndex(1, 12);  triangles[7].SetVertexIndex(2, 13);
    triangles[8].SetVertexIndex(0, 10);  triangles[8].SetVertexIndex(1, 11);  triangles[8].SetVertexIndex(2, 9);
    triangles[9].SetVertexIndex(0, 10);  triangles[9].SetVertexIndex(1, 9);  triangles[9].SetVertexIndex(2, 8);
    ForegroundTextureIndexBuffers.Unlock();

    ForegroundTextureVertexBuffers.Uninitialise();
    ForegroundTextureVertexBuffers.Initialise((EVertexType)0, 14, sizeof(CTVertexColTex1), g_pResourceManager->GetVertexBufferUsage(),
                                              (EVertexBufferPool)1);
    CTVertexColTex1* v = (CTVertexColTex1*)ForegroundTextureVertexBuffers.Lock(0, 0, ForegroundTextureVertexBuffers.GetSize());
    v[0].SetPosition(C3DVector(-1.0f, 1.0f, 1.0f));  v[0].SetU1V1(0.0f, 0.0f);  v[0].SetDiffuseColour(0xffffffff);
    v[1].SetPosition(C3DVector(1.0f, 1.0f, 1.0f));  v[1].SetU1V1(1.0f, 0.0f);  v[1].SetDiffuseColour(0xffffffff);
    v[2].SetPosition(C3DVector(1.0f, -1.0f, 1.0f));  v[2].SetU1V1(1.0f, 1.0f);  v[2].SetDiffuseColour(0xffffffff);
    v[3].SetPosition(C3DVector(-1.0f, -1.0f, 1.0f));  v[3].SetU1V1(0.0f, 1.0f);  v[3].SetDiffuseColour(0xffffffff);
    v[4].SetPosition(C3DVector(-1.0f, 0.0f, 1.0f));  v[4].SetU1V1(0.0f, 0.0f);  v[4].SetDiffuseColour(0x00ffffff);
    v[5].SetPosition(C3DVector(1.0f, 0.0f, 1.0f));  v[5].SetU1V1(1.0f, 0.0f);  v[5].SetDiffuseColour(0x00ffffff);
    v[6].SetPosition(C3DVector(1.0f, -0.1f, 1.0f));  v[6].SetU1V1(1.0f, 0.1f);  v[6].SetDiffuseColour(0xffffffff);
    v[7].SetPosition(C3DVector(-1.0f, -0.1f, 1.0f));  v[7].SetU1V1(0.0f, 0.1f);  v[7].SetDiffuseColour(0xffffffff);
    v[8].SetPosition(C3DVector(-1.0f, -1.0f, 1.0f));  v[8].SetU1V1(0.0f, 1.0f);  v[8].SetDiffuseColour(0xffffffff);
    v[9].SetPosition(C3DVector(1.0f, -1.0f, 1.0f));  v[9].SetU1V1(1.0f, 1.0f);  v[9].SetDiffuseColour(0xffffffff);
    v[10].SetPosition(C3DVector(-1.0f, 1.0f, 1.0f));  v[10].SetU1V1(0.0f, 0.0f);  v[10].SetDiffuseColour(0xffffffff);
    v[11].SetPosition(C3DVector(1.0f, 1.0f, 1.0f));  v[11].SetU1V1(1.0f, 0.0f);  v[11].SetDiffuseColour(0xffffffff);
    v[12].SetPosition(C3DVector(1.0f, -0.1f, 1.0f));  v[12].SetU1V1(1.0f, 1.0f);  v[12].SetDiffuseColour(0xffffffff);
    v[13].SetPosition(C3DVector(-1.0f, -0.1f, 1.0f));  v[13].SetU1V1(0.0f, 1.0f);  v[13].SetDiffuseColour(0xffffffff);
    ForegroundTextureVertexBuffers.Unlock();
}
