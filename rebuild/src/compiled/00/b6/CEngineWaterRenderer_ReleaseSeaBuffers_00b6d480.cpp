// CEngineWaterRenderer::ReleaseSeaBuffers() -- uninitialise the sea background vertex buffer and index buffer, zero their counts and drop the HaveBackgroundSea flag
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B6D480, length 48 bytes.
// Callees: CVertexBufferWin32::Uninitialise @ 0x00A63200 (on SeaBackgroundVertexBuffer +0x7c), CIndexBuffer::Uninitialise @ 0x00AC1EE0 (on
// SeaBackgroundIndexBuffer +0x68; the manifest label "CVolume::ReleaseVolume" is propagated). Statement order: vertex buffer, vertex count,
// index buffer, poly count, flag; the vertex-count zero is scheduled between the index buffer's `lea` and its call. The engine header
// (CEngineWaterRenderer.h) carries the Ego PDB layout whose HaveBackgroundSea sits at +0x2aa; retail keeps it at +0x276 (the retail
// CArray is 12 bytes, so everything after the render lists is 0x34 lower), so the retail layout is modelled locally.
struct CIndexBuffer_00b6d480 {
    unsigned char _body[0x14];
    void Uninitialise();                           // 0x00AC1EE0
};

struct CVertexBufferWin32_00b6d480 {
    unsigned char _body[0x34];
    void Uninitialise();                           // 0x00A63200
};

class CEngineWaterRenderer { public:
    unsigned char               _pad0[0x68];
    CIndexBuffer_00b6d480       SeaBackgroundIndexBuffer;     // +0x68
    CVertexBufferWin32_00b6d480 SeaBackgroundVertexBuffer;    // +0x7c
    long                        SeaBackgroundVertexCount;     // +0xb0
    long                        SeaBackgroundPolyCount;       // +0xb4
    unsigned char               _pad1[0x276 - 0xb8];
    bool                        HaveBackgroundSea;            // +0x276
    void ReleaseSeaBuffers();
};

void CEngineWaterRenderer::ReleaseSeaBuffers()
{
    SeaBackgroundVertexBuffer.Uninitialise();
    SeaBackgroundVertexCount = 0;
    SeaBackgroundIndexBuffer.Uninitialise();
    SeaBackgroundPolyCount = 0;
    HaveBackgroundSea = false;
}