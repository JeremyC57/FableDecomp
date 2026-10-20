// CLandscapeTesselatorBase::TesselateEdgeGeneric(LANDSCAPE_EDGE_STRIP_SIDE, CPatchTesselationEdgeStrip*, CMovableResource*) -- rebuild one edge strip of the current patch and its bridge buffers
// Grade: DIFFER(ours v retail) -- register assignment: ours spills x and keeps y in ebx (retail spills y, keeps x in ebx), tests the side with `test edi,edi` (retail `cmp edi,ebp` with the zeroed direction register), and loads the index buffer straight into ecx (retail eax + `mov ecx,eax`)
enum LANDSCAPE_EDGE_STRIP_SIDE { EDGE_SIDE_0, EDGE_SIDE_1, EDGE_SIDE_2, EDGE_SIDE_3 };
struct CPatchTesselationEdgeStrip;
struct CMovableResource;
struct CIndexBuffer {
    unsigned char* Lock(unsigned long offset, unsigned long size, unsigned long flags);   // 0x00AC1FB0
    void Unlock();                                 // 0x00AC1F00
};
struct CVertexBufferWin32;
struct CEngineMap;
struct CTesselationData {
    void*                       __vftable;
    long                        PatchWorldX;       // +0x04
    long                        PatchWorldY;       // +0x08
    long                        Width;             // +0x0c
    long                        Height;            // +0x10
    CEngineMap*                 EngineMap;         // +0x14
    CIndexBuffer*               IndexBuffer;       // +0x18
    CPatchTesselationEdgeStrip* EdgeStrips[4];     // +0x1c
    CVertexBufferWin32*         BridgeVertexBuffers[4];   // +0x2c
    CIndexBuffer*               BridgeIndexBuffers[4];    // +0x3c
    long                        BridgePolyCount[4];       // +0x4c
};
struct CBufferManager {
    void ReleaseIndexBuffer(CIndexBuffer* buffer);         // 0x00BDA090
    void ReleaseVertexBuffer(CVertexBufferWin32* buffer);  // 0x00BDA0D0
};
struct CEngine {
    unsigned char   _pad000[0x6b0];
    CBufferManager* BufferManager;                 // +0x6b0
};
extern CEngine* g_pEngine;                         // 0x01436EA8
template <class T> struct vector {                 // STLport vector, 12 bytes
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    void resize(unsigned long count, const T& value);   // 0x00517900
    void resize(unsigned long count) { resize(count, T()); }
};
class CLandscapeTesselatorBase {
public:
    virtual void TesselateStrip(CPatchTesselationEdgeStrip* strip, long x, long y, bool alongX, bool reversed,
                                long direction, unsigned char* indices);   // slot 0
    virtual void v01(); virtual void v02(); virtual void v03();
    virtual void FinishEdge(long flags);           // slot 4 (+0x10)
    vector<unsigned long> BridgeIndexBuffer;       // +0x04
    CTesselationData*     Data;                    // +0x10
    bool BuildBridgeVertexAndIndexBuffers(LANDSCAPE_EDGE_STRIP_SIDE side, CMovableResource* resource);   // 0x00BE3660
    bool TesselateEdgeGeneric(LANDSCAPE_EDGE_STRIP_SIDE side, CPatchTesselationEdgeStrip* strip, CMovableResource* resource);
};

bool CLandscapeTesselatorBase::TesselateEdgeGeneric(LANDSCAPE_EDGE_STRIP_SIDE side, CPatchTesselationEdgeStrip* strip, CMovableResource* resource)
{
    if (Data->BridgeIndexBuffers[side]) {
        g_pEngine->BufferManager->ReleaseIndexBuffer(Data->BridgeIndexBuffers[side]);
        Data->BridgeIndexBuffers[side] = 0;
    }
    if (Data->BridgeVertexBuffers[side]) {
        g_pEngine->BufferManager->ReleaseVertexBuffer(Data->BridgeVertexBuffers[side]);
        Data->BridgeVertexBuffers[side] = 0;
    }
    CTesselationData* data = Data;
    long y = data->PatchWorldY;
    long x = data->PatchWorldX;
    bool alongX = false;
    bool reversed = false;
    long direction = 0;
    if (side == EDGE_SIDE_0) {
        direction = 1;
        alongX = true;
    } else if (side == EDGE_SIDE_1) {
        x += data->Width;
        alongX = true;
        reversed = true;
    } else if (side == EDGE_SIDE_2) {
        direction = 3;
        reversed = true;
    } else {
        y += data->Height;
        direction = 2;
    }
    unsigned char* indices = data->IndexBuffer->Lock(0, 0, 0);
    TesselateStrip(strip, x, y, alongX, reversed, direction, indices);
    Data->IndexBuffer->Unlock();
    bool built = BuildBridgeVertexAndIndexBuffers(side, resource);
    BridgeIndexBuffer.resize(0);
    FinishEdge(0);
    return built;
}
