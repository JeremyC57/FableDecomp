#include <cstdio>
#include <cstring>
#ifndef _MSC_VER
#define __fastcall
#define __stdcall
#define __forceinline inline
#endif
#define COM_SLOT(n) virtual void __stdcall slot##n() {}

struct IDirect3DBaseTexture9;
struct IDirect3DTexture9;
struct IDirect3DIndexBuffer9;

struct IDirect3DDevice9 {                          // only the slots used are typed
    COM_SLOT(0);
    COM_SLOT(1);
    COM_SLOT(2);
    COM_SLOT(3);
    COM_SLOT(4);
    COM_SLOT(5);
    COM_SLOT(6);
    COM_SLOT(7);
    COM_SLOT(8);
    COM_SLOT(9);
    COM_SLOT(10);
    COM_SLOT(11);
    COM_SLOT(12);
    COM_SLOT(13);
    COM_SLOT(14);
    COM_SLOT(15);
    COM_SLOT(16);
    COM_SLOT(17);
    COM_SLOT(18);
    COM_SLOT(19);
    COM_SLOT(20);
    COM_SLOT(21);
    COM_SLOT(22);
    COM_SLOT(23);
    COM_SLOT(24);
    COM_SLOT(25);
    COM_SLOT(26);
    COM_SLOT(27);
    COM_SLOT(28);
    COM_SLOT(29);
    COM_SLOT(30);
    COM_SLOT(31);
    COM_SLOT(32);
    COM_SLOT(33);
    COM_SLOT(34);
    COM_SLOT(35);
    COM_SLOT(36);
    COM_SLOT(37);
    COM_SLOT(38);
    COM_SLOT(39);
    COM_SLOT(40);
    COM_SLOT(41);
    COM_SLOT(42);
    COM_SLOT(43);
    COM_SLOT(44);
    COM_SLOT(45);
    COM_SLOT(46);
    COM_SLOT(47);
    COM_SLOT(48);
    COM_SLOT(49);
    COM_SLOT(50);
    COM_SLOT(51);
    COM_SLOT(52);
    COM_SLOT(53);
    COM_SLOT(54);
    COM_SLOT(55);
    COM_SLOT(56);
    COM_SLOT(57);
    COM_SLOT(58);
    COM_SLOT(59);
    COM_SLOT(60);
    COM_SLOT(61);
    COM_SLOT(62);
    COM_SLOT(63);
    COM_SLOT(64);
    virtual long __stdcall SetTexture(unsigned long stage, IDirect3DBaseTexture9* texture) { return 0; }   // slot 65 (+0x104)
    COM_SLOT(66); COM_SLOT(67); COM_SLOT(68); COM_SLOT(69); COM_SLOT(70); COM_SLOT(71); COM_SLOT(72); COM_SLOT(73); COM_SLOT(74); COM_SLOT(75);
    COM_SLOT(76); COM_SLOT(77); COM_SLOT(78); COM_SLOT(79); COM_SLOT(80); COM_SLOT(81);
    virtual long __stdcall DrawIndexedPrimitive(unsigned long type, long baseVertexIndex, unsigned long minVertexIndex,
                                                unsigned long numVertices, unsigned long startIndex,
                                                unsigned long primCount) { return 0; }    // slot 82 (+0x148)
    COM_SLOT(83); COM_SLOT(84); COM_SLOT(85); COM_SLOT(86); COM_SLOT(87); COM_SLOT(88); COM_SLOT(89); COM_SLOT(90); COM_SLOT(91); COM_SLOT(92);
    COM_SLOT(93); COM_SLOT(94); COM_SLOT(95); COM_SLOT(96); COM_SLOT(97); COM_SLOT(98); COM_SLOT(99); COM_SLOT(100); COM_SLOT(101);
    COM_SLOT(102); COM_SLOT(103);
    virtual long __stdcall SetIndices(IDirect3DIndexBuffer9* indexData) { return 0; }                     // slot 104 (+0x1a0)
};

enum EPrimitiveType { PRIMITIVE_TYPE_DEFAULT = -1, PrimitiveTriangleList = 4, PrimitiveTriangleStrip = 5 };

class CIndexBuffer {                               // Ego_r.pdb, 0x14 bytes
public:
    void*                  __vftable;              // +0x00
    IDirect3DIndexBuffer9* PD3DIndexBuffer;        // +0x04
    unsigned long          ByteLength;             // +0x08
    EPrimitiveType         PrimitiveType;          // +0x0c
    void*                  PAllocatedBuffer;       // +0x10
};

class CTexture {                                   // Ego_r.pdb, 8 bytes
public:
    IDirect3DTexture9* PD3DTexture;                // +0x00
    unsigned long      Bits;                       // +0x04 ByteLength:28, AllocationSource:4
};

class CRenderStateManager {
public:
    void RealiseRenderState();                     // 0x00A058C0
};

class CRenderManagerCore : public CRenderStateManager {   // retail offsets as the landed SetupFirstTextures 0x00B9D670
public:
    unsigned char          _pad0[0x3cb8];
    long                   NumTextureStagesUsed;   // +0x3cb8
    unsigned char          _pad3cbc[0x3cf0 - 0x3cbc];
    IDirect3DDevice9*      PD3DDevice;             // +0x3cf0
    void*                  CurrentIndices;         // +0x3cf4
    unsigned char          _pad3cf8[8];
    IDirect3DBaseTexture9* CurrentTextures[4];     // +0x3d00
    void AttachTextureToStage(long stage, const CTexture* texture)
    {
        AttachD3DTextureToStage(stage, texture ? (IDirect3DBaseTexture9*)texture->PD3DTexture : 0);
    }
private:
    void AttachD3DTextureToStage(long stage, IDirect3DBaseTexture9* texture)
    {
        if (CurrentTextures[stage] != texture) {
            CurrentTextures[stage] = texture;
            PD3DDevice->SetTexture(stage, texture);
        }
        if (NumTextureStagesUsed < stage + 1)
            NumTextureStagesUsed = stage + 1;
    }
};
extern CRenderManagerCore* g_pRenderManagerCore;   // 0x01436E18

class CRenderManager : public CRenderManagerCore { };

class CShaderRenderManager {
public:
    unsigned char     _pad000[0x1d0];
    CRenderManager*   RenderManager;               // +0x1d0
    IDirect3DDevice9* D3DDevice;                   // +0x1d4
    __forceinline void DrawBuffersIndexed(unsigned long minVertexIndex, unsigned long numVertices, CIndexBuffer& indexBuffer,
                            unsigned long startIndex, unsigned long primitiveCount, EPrimitiveType primitiveType)
    {
        D3DDevice->SetIndices(indexBuffer.PD3DIndexBuffer);
        if (primitiveType == PRIMITIVE_TYPE_DEFAULT)
            primitiveType = indexBuffer.PrimitiveType;
        RenderManager->RealiseRenderState();
        D3DDevice->DrawIndexedPrimitive(primitiveType, 0, minVertexIndex, numVertices, startIndex, primitiveCount);
    }
};
extern CShaderRenderManager* g_pShaderRenderManager;   // 0x01436E14

template <class T> class scoped_ptr {              // boost::scoped_ptr (one pointer)
public:
    T& operator*() const { return *ptr; }
    T* operator->() const { return ptr; }
    T* get() const { return ptr; }
private:
    T* ptr;
};

namespace N3DPrimitive {
class CBaseBlock {                                 // Ego_r.pdb, 12 bytes
public:
    unsigned long PrimitiveCount;                  // +0x00
    unsigned long StartIndex;                      // +0x04
    bool          IsStrip;                         // +0x08
    bool          DegenerateTriangles;             // +0x09
    unsigned char ChangeFlags;                     // +0x0a
};
class CStaticBlock : public CBaseBlock {           // Ego_r.pdb, 32 bytes
public:
    unsigned long DiffuseTextureFixupOffset;       // +0x0c
    unsigned long BumpTextureFixupOffset;          // +0x10
    CTexture*     DiffuseTexture;                  // +0x14
    CTexture*     BumpTexture;                     // +0x18
    long          MaterialIndex;                   // +0x1c
};
}

class C3DPrimitive2 {
public:
    unsigned char             _pad00[0x34];
    scoped_ptr<CIndexBuffer>  IndexBuffer;         // +0x34
    unsigned long             VertexBufferStatus;  // +0x38
    unsigned long             IndexBufferStatus;   // +0x3c
    unsigned long             StaticBlockCount;    // +0x40
    N3DPrimitive::CStaticBlock* StaticBlocks;      // +0x44
    unsigned long             AnimatedBlockCount;  // +0x48
    void*                     AnimatedBlocks;      // +0x4c
    unsigned long             AnimatedBlocksStatus;// +0x50
    unsigned long             RepeatedBlocksStatus;// +0x54
    unsigned long             VertexCount;         // +0x58
};

class C3DTextureStages {                           // Ego_r.pdb, 12 bytes
public:
    long DiffuseTextureStage;                      // +0x00
    long BumpTextureStage;                         // +0x04
    bool UseBlankTextures;                         // +0x08
};

class CEnginePrimitiveManagerStaticMeshesBase {
public:
    static void __fastcall RenderMeshPrimitiveBlocksDegenerateTriangles(C3DPrimitive2& primitive, const C3DTextureStages& textureStages);
};

void __fastcall CEnginePrimitiveManagerStaticMeshesBase::RenderMeshPrimitiveBlocksDegenerateTriangles(C3DPrimitive2& primitive,
                                                                                                      const C3DTextureStages& textureStages)
{
    if (textureStages.DiffuseTextureStage != -1)
        g_pRenderManagerCore->AttachTextureToStage(textureStages.DiffuseTextureStage, 0);
    if (textureStages.BumpTextureStage != -1)
        g_pRenderManagerCore->AttachTextureToStage(textureStages.BumpTextureStage, 0);

    const unsigned long blockCount = primitive.StaticBlockCount;
    for (unsigned long i = 0; i < blockCount; ++i) {
        N3DPrimitive::CStaticBlock& block = primitive.StaticBlocks[i];
        if (block.DegenerateTriangles) {
            const unsigned long primitiveCount = block.PrimitiveCount;
            const unsigned long startIndex = block.StartIndex;
            const unsigned long vertexCount = primitive.VertexCount;
            CIndexBuffer& indexBuffer = *primitive.IndexBuffer;
            g_pShaderRenderManager->DrawBuffersIndexed(0, vertexCount, indexBuffer, startIndex, primitiveCount,
                                                       block.IsStrip ? PrimitiveTriangleStrip : PrimitiveTriangleList);
        }
    }
}


static int g_realise = 0;
void CRenderStateManager::RealiseRenderState() { ++g_realise; }

struct Call { int kind; unsigned long a, b, c, d, e, f; };
static Call g_calls[64];
static int g_ncalls = 0;

struct MockDevice : IDirect3DDevice9 {
    virtual long __stdcall SetTexture(unsigned long stage, IDirect3DBaseTexture9* texture)
    { Call c = { 1, stage, (unsigned long)(texture != 0), 0, 0, 0, 0 }; g_calls[g_ncalls++] = c; return 0; }
    virtual long __stdcall DrawIndexedPrimitive(unsigned long type, long base, unsigned long minV, unsigned long numV,
                                                unsigned long start, unsigned long count)
    { Call c = { 2, type, (unsigned long)base, minV, numV, start, count }; g_calls[g_ncalls++] = c; return 0; }
    virtual long __stdcall SetIndices(IDirect3DIndexBuffer9* ib)
    { Call c = { 3, (unsigned long)(ib != 0), 0, 0, 0, 0, 0 }; g_calls[g_ncalls++] = c; return 0; }
};

static CRenderManager s_core;
static CShaderRenderManager s_shader;
CRenderManagerCore* g_pRenderManagerCore = &s_core;
CShaderRenderManager* g_pShaderRenderManager = &s_shader;

static bool fail(const char* why) { std::printf("00b9e750_TEST FAIL %s\n", why); return false; }

static bool run()
{
    MockDevice dev;
    static int fakeTex, fakeIb;
    s_core.PD3DDevice = &dev;
    s_core.NumTextureStagesUsed = 1;
    s_core.CurrentTextures[0] = (IDirect3DBaseTexture9*)&fakeTex;   // diffuse stage bound -> SetTexture(0, 0)
    s_core.CurrentTextures[2] = 0;                                  // bump stage already empty -> no device call
    s_shader.RenderManager = &s_core;
    s_shader.D3DDevice = &dev;

    CIndexBuffer ib;
    std::memset(&ib, 0, sizeof ib);
    ib.PD3DIndexBuffer = (IDirect3DIndexBuffer9*)&fakeIb;
    ib.PrimitiveType = PrimitiveTriangleList;
    CIndexBuffer* ibp = &ib;

    N3DPrimitive::CStaticBlock blocks[4];
    std::memset(blocks, 0, sizeof blocks);
    blocks[0].PrimitiveCount = 10; blocks[0].StartIndex = 0;   blocks[0].IsStrip = false; blocks[0].DegenerateTriangles = true;
    blocks[1].PrimitiveCount = 20; blocks[1].StartIndex = 30;  blocks[1].IsStrip = true;  blocks[1].DegenerateTriangles = false;
    blocks[2].PrimitiveCount = 30; blocks[2].StartIndex = 90;  blocks[2].IsStrip = true;  blocks[2].DegenerateTriangles = true;
    blocks[3].PrimitiveCount = 5;  blocks[3].StartIndex = 200; blocks[3].IsStrip = false; blocks[3].DegenerateTriangles = true;

    C3DPrimitive2 prim;
    std::memset(&prim, 0, sizeof prim);
    std::memcpy(&prim.IndexBuffer, &ibp, sizeof ibp);
    prim.StaticBlockCount = 4;
    prim.StaticBlocks = blocks;
    prim.VertexCount = 77;

    C3DTextureStages stages = { 0, 2, false };
    CEnginePrimitiveManagerStaticMeshesBase::RenderMeshPrimitiveBlocksDegenerateTriangles(prim, stages);

    if (g_ncalls != 1 + 3 * 2) return fail("call count");
    if (g_calls[0].kind != 1 || g_calls[0].a != 0 || g_calls[0].b != 0) return fail("diffuse unbind");
    if (s_core.CurrentTextures[0] != 0) return fail("texture cache");
    if (s_core.NumTextureStagesUsed != 3) return fail("stages used");
    const unsigned long want[3][3] = { { 4, 0, 10 }, { 5, 90, 30 }, { 4, 200, 5 } };
    for (int i = 0; i < 3; ++i) {
        const Call& si = g_calls[1 + 2 * i];
        const Call& dr = g_calls[2 + 2 * i];
        if (si.kind != 3 || si.a != 1) return fail("set indices");
        if (dr.kind != 2 || dr.a != want[i][0] || dr.b != 0 || dr.c != 0 || dr.d != 77 || dr.e != want[i][1] || dr.f != want[i][2])
            return fail("draw args");
    }
    if (g_realise != 3) return fail("realise");

    // no texture stages, no blocks: nothing happens
    g_ncalls = 0; g_realise = 0;
    C3DTextureStages none = { -1, -1, false };
    prim.StaticBlockCount = 0;
    CEnginePrimitiveManagerStaticMeshesBase::RenderMeshPrimitiveBlocksDegenerateTriangles(prim, none);
    if (g_ncalls != 0 || g_realise != 0) return fail("empty");
    return true;
}

int main()
{
    if (!run()) return 1;
    std::printf("00b9e750_TEST PASS\n");
    return 0;
}