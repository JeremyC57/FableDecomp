// CEngineWaterRenderer::BuildConstantBuffers() -- build the shared water index/vertex buffers: a 16x16-quad patch triangulated in 2x2 blocks with alternating diagonals, stripified into the foreground index buffer, plus the fixed 30-index / 14-vertex texture-quad buffers
// Leaf: ?BuildConstantBuffers@CEngineWaterRenderer@@AAEXXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B6EDF0, length 3144 bytes, __thiscall, `ret`.
// Identity: Ego_r.pdb / FableWin.pdb ?BuildConstantBuffers@CEngineWaterRenderer@@AAEXXZ; its only caller is the CEngineWaterRenderer
//   ctor 0x00B73760, and it fills exactly the three buffers DeleteConstantBuffers (0x00B6D4B0) releases: ForegroundIndexBuffers +0x0C,
//   ForegroundTextureIndexBuffers +0x20, ForegroundTextureVertexBuffers +0x34, plus WaterForegroundPolyCount +0xB8. The catalog
//   label "GenerateWaterMesh" is not a PDB name.
// Body: a 17x17-vertex patch as 2x2 blocks of quads with alternating diagonals (STLport push_back on CArray<unsigned short>, the
//   POD _M_insert_overflow inlined: malloc / memmove / free), CTriangleStripifier2 (RTTI : CTriangleStripifierBase :
//   CBaseClassNonCopyable; ctor 0x0099A2F0 + vtable 0x012A331C, implicit dtor -> ~CBaseClassNonCopyable 0x0099A300)
//   ::GenerateTriangleListAndStrip(CArray<unsigned short>&, CArray<unsigned short>&, CArray<unsigned short>&) 0x00F3EA50 (FableWin),
//   the strip copied into a D3DFMT_INDEX16 (0x65) triangle-strip (5) index buffer (CIndexBuffer::Uninitialise 0x00AC1EE0 /
//   Initialise 0x00AC1F70 / Lock 0x00AC1FB0 / Unlock 0x00AC1F00 / GetSize 0x00AC1F10; CEngineResourceManager::GetIndexBufferUsage
//   0x00B3B060 / GetIndexBufferMemPool 0x00B3B0A0 / GetVertexBufferUsage 0x00B3B030), 10 C3DTriangle2 (SetVertexIndex, inline) of a
//   triangle-list (4) buffer, and 14 CTVertexColTex1 (CGenericVertex<CTVertexColTex1Base>::SetPosition / SetU1V1 /
//   SetDiffuseColour, inline) through CVertexBufferWin32::Initialise 0x00A63150 / Uninitialise 0x00A63200 and CVertexBufferBase::Lock
//   0x00AC20A0 / Unlock 0x00AC20B0.
// Vertex block: every vertex is written through one reassigned pointer (`vertex = &v[k]`). Measured on the c2 scheduler DAG: retail
//   stores of one vertex are mutually independent and depend on every store of the earlier vertices (v0, v6, v12 and v13 show it: U/V
//   before position in v0, v6.Y / v12.Y after the UVs, v13 Z/V before `mov ecx,esi`); setters on `v[k]` give per-setter `this` temps
//   (cross-setter edges inside a vertex) and plain `v[k].X` stores give no cross-vertex edges -- neither reproduces retail, the
//   per-vertex pointer reproduces all fourteen vertices.
// FITTED (chosen by grading, ordinary spellings): (1) rowBase declared and zeroed before the y loop (retail 0x00B6EE27 / 0x00B6EE40:
//   x = 0 stored before rowBase = y * 17 and x loaded first); (2) v[1] written Y, X, Z by member stores (retail 0x00B6F855 stores
//   Y before X; the stores of one vertex have equal priority, so the order is the statement order); (3) v[5] colour first (retail
//   0x00B6F8A2 stores v5.DiffuseColour right after v4's).
// EVIDENCED: C3DTriangle2::SetVertexIndex per index (retail in-order word stores), setters position / UV / colour (lazy constant
//   loads); FableWin.pdb names CGenericVertex<CTVertexColTex1Base>::SetPosition / SetU1V1 / SetDiffuseColour (inline here).
#define FABLE_RETAIL_CENGINEWATERRENDERER
#define FABLE_RETAIL_CENGINERESOURCEMANAGER
#define FABLE_RETAIL_CTRIANGLESTRIPIFIER2
#define FABLE_RETAIL_C3DTRIANGLE2_SETVERTEXINDEX
#define FABLE_RETAIL_CTVERTEXCOLTEX1_SETTERS
#include "engine/CEngineWaterRenderer.h"
#include "engine/CEngineResourceManager.h"
#include "engine/CTriangleStripifier2.h"
#include "engine/C3DTriangle2.h"
#include "engine/C3DVector.h"
#include "engine/CTVertexColTex1.h"

namespace NEngineInternal { extern CEngineResourceManager* GPResourceManager; }   // 0x01436E98 (?GPResourceManager@NEngineInternal@@3PAVCEngineResourceManager@@A, landed 0x00BB5040)


void CEngineWaterRenderer::BuildConstantBuffers()
{
    CArray<unsigned short> polys;
    long rowBase = 0;   // FITTED: declared (and zeroed) before the y loop so x = 0 is stored before rowBase = y * 17 AND x is loaded first in x + rowBase (retail 0x00B6EE27 / 0x00B6EE40)
    for (long y = 0; y < 16; y += 2) {
        long x = 0;
        rowBase = y * 17;
        for (; x < 16; x += 2) {
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
    ForegroundIndexBuffers.Initialise(stripLength * 2, NEngineInternal::GPResourceManager->GetIndexBufferUsage(), (EIndexBufferFormat)0x65,
                                      NEngineInternal::GPResourceManager->GetIndexBufferMemPool(), (EPrimitiveType)5);
    unsigned char* stripData = ForegroundIndexBuffers.Lock(0, 0, 0);
    memcpy(stripData, strip.GetAsCArray(), stripLength * 2);
    ForegroundIndexBuffers.Unlock();
    WaterForegroundPolyCount = stripLength - 2;

    ForegroundTextureIndexBuffers.Uninitialise();
    ForegroundTextureIndexBuffers.Initialise(0x3c, NEngineInternal::GPResourceManager->GetIndexBufferUsage(), (EIndexBufferFormat)0x65,
                                             NEngineInternal::GPResourceManager->GetIndexBufferMemPool(), (EPrimitiveType)4);
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
    ForegroundTextureVertexBuffers.Initialise((EVertexType)0, 14, sizeof(CTVertexColTex1), NEngineInternal::GPResourceManager->GetVertexBufferUsage(),
                                              (EVertexBufferPool)1);
    CTVertexColTex1* v = (CTVertexColTex1*)ForegroundTextureVertexBuffers.Lock(0, 0, ForegroundTextureVertexBuffers.GetSize());
    CTVertexColTex1* vertex = &v[0];
    vertex->SetPosition(C3DVector(-1.0f, 1.0f, 1.0f));  vertex->SetU1V1(0.0f, 0.0f);  vertex->SetDiffuseColour(0xffffffff);
    vertex = &v[1];
    vertex->Y = 1.0f;  vertex->X = 1.0f;  vertex->Z = 1.0f;  vertex->SetU1V1(1.0f, 0.0f);  vertex->SetDiffuseColour(0xffffffff);
    vertex = &v[2];
    vertex->SetPosition(C3DVector(1.0f, -1.0f, 1.0f));  vertex->SetU1V1(1.0f, 1.0f);  vertex->SetDiffuseColour(0xffffffff);
    vertex = &v[3];
    vertex->SetPosition(C3DVector(-1.0f, -1.0f, 1.0f));  vertex->SetU1V1(0.0f, 1.0f);  vertex->SetDiffuseColour(0xffffffff);
    vertex = &v[4];
    vertex->SetPosition(C3DVector(-1.0f, 0.0f, 1.0f));  vertex->SetU1V1(0.0f, 0.0f);  vertex->SetDiffuseColour(0x00ffffff);
    vertex = &v[5];
    vertex->SetDiffuseColour(0x00ffffff);  vertex->SetPosition(C3DVector(1.0f, 0.0f, 1.0f));  vertex->SetU1V1(1.0f, 0.0f);
    vertex = &v[6];
    vertex->SetPosition(C3DVector(1.0f, -0.1f, 1.0f));  vertex->SetU1V1(1.0f, 0.1f);  vertex->SetDiffuseColour(0xffffffff);
    vertex = &v[7];
    vertex->SetPosition(C3DVector(-1.0f, -0.1f, 1.0f));  vertex->SetU1V1(0.0f, 0.1f);  vertex->SetDiffuseColour(0xffffffff);
    vertex = &v[8];
    vertex->SetPosition(C3DVector(-1.0f, -1.0f, 1.0f));  vertex->SetU1V1(0.0f, 1.0f);  vertex->SetDiffuseColour(0xffffffff);
    vertex = &v[9];
    vertex->SetPosition(C3DVector(1.0f, -1.0f, 1.0f));  vertex->SetU1V1(1.0f, 1.0f);  vertex->SetDiffuseColour(0xffffffff);
    vertex = &v[10];
    vertex->SetPosition(C3DVector(-1.0f, 1.0f, 1.0f));  vertex->SetU1V1(0.0f, 0.0f);  vertex->SetDiffuseColour(0xffffffff);
    vertex = &v[11];
    vertex->SetPosition(C3DVector(1.0f, 1.0f, 1.0f));  vertex->SetU1V1(1.0f, 0.0f);  vertex->SetDiffuseColour(0xffffffff);
    vertex = &v[12];
    vertex->SetPosition(C3DVector(1.0f, -0.1f, 1.0f));  vertex->SetU1V1(1.0f, 1.0f);  vertex->SetDiffuseColour(0xffffffff);
    vertex = &v[13];
    vertex->SetPosition(C3DVector(-1.0f, -0.1f, 1.0f));  vertex->SetU1V1(0.0f, 1.0f);  vertex->SetDiffuseColour(0xffffffff);
    ForegroundTextureVertexBuffers.Unlock();
}
