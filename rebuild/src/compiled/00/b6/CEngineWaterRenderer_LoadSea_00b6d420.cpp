// CEngineWaterRenderer::LoadSea(CMemoryDataInputStream* stream) -- build the sea background buffers from a stream through a stack CWaterSeaGenerator: vertex buffer, vertex count, index buffer and a local poly count come back from LoadSea, the poly count less two is kept (strip -> triangle count) and HaveBackgroundSea is raised
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B6D420, length 91 bytes (the oracle row carries 5 bytes of int3 padding).
// Callees: CWaterSeaGenerator::CWaterSeaGenerator @ 0x00BE9140 (4-byte object at [esp+8], an empty constructor returning `this`),
// CWaterSeaGenerator::LoadSea(stream, CVertexBufferWin32*, long* vertexCount, CIndexBuffer*, long* polyCount) @ 0x00BE91E0,
// CWaterSeaGenerator::~CWaterSeaGenerator @ 0x00BE9150 (empty). The `long polyCount = 0` store is scheduled after the argument pushes
// (`mov dword ptr [esp+18h],0`), the result is read back, decremented by two (`add edx,-2`) and stored before the flag byte. The engine
// header's HaveBackgroundSea (+0x2aa, Ego PDB) sits at +0x276 in retail (12-byte CArrays), so the retail layout is modelled locally.
class CMemoryDataInputStream;

struct CIndexBuffer_00b6d420 { unsigned char _body[0x14]; };
struct CVertexBufferWin32_00b6d420 { unsigned char _body[0x34]; };

struct CWaterSeaGenerator_00b6d420 {              // 4 bytes, built and destroyed out of line
    long m_state;
    CWaterSeaGenerator_00b6d420();                 // 0x00BE9140
    ~CWaterSeaGenerator_00b6d420();                // 0x00BE9150
    void LoadSea(CMemoryDataInputStream* stream, CVertexBufferWin32_00b6d420* vertexBuffer, long* vertexCount,
                 CIndexBuffer_00b6d420* indexBuffer, long* polyCount);   // 0x00BE91E0
};

class CEngineWaterRenderer { public:
    unsigned char               _pad0[0x68];
    CIndexBuffer_00b6d420       SeaBackgroundIndexBuffer;     // +0x68
    CVertexBufferWin32_00b6d420 SeaBackgroundVertexBuffer;    // +0x7c
    long                        SeaBackgroundVertexCount;     // +0xb0
    long                        SeaBackgroundPolyCount;       // +0xb4
    unsigned char               _pad1[0x276 - 0xb8];
    bool                        HaveBackgroundSea;            // +0x276
    void LoadSea(CMemoryDataInputStream* stream);
};

void CEngineWaterRenderer::LoadSea(CMemoryDataInputStream* stream)
{
    CWaterSeaGenerator_00b6d420 generator;
    long polyCount = 0;
    generator.LoadSea(stream, &SeaBackgroundVertexBuffer, &SeaBackgroundVertexCount, &SeaBackgroundIndexBuffer, &polyCount);
    SeaBackgroundPolyCount = polyCount - 2;
    HaveBackgroundSea = true;
}