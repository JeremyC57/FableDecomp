// Behaviour test for CLandscapeTesselatorBase::BuildBridgeVertexAndIndexBuffers @ 0x00BE3660 (model copy over stub buffers).
#include <cstdio>
#include <cstring>
static unsigned char g_ib[64], g_vb[64]; static bool g_failIndex, g_failVertex; static int g_unlocks;
struct CMovableResource {};
struct CIndexBuffer { unsigned char* Lock(unsigned long, unsigned long, unsigned long) { return g_ib; } void Unlock() { ++g_unlocks; } };
struct CVertexBufferWin32 { unsigned char* Lock(unsigned long, unsigned long, unsigned long) { return g_vb; } void Unlock() { ++g_unlocks; } };
static CIndexBuffer g_index; static CVertexBufferWin32 g_vertex;
struct CTesselationData { CVertexBufferWin32* BridgeVertexBuffers[4]; CIndexBuffer* BridgeIndexBuffers[4]; long BridgePolyCounts[4]; };
struct CArrayU16 {
    unsigned short* _M_start; unsigned short* _M_finish;
    unsigned long size() const { return (unsigned long)(_M_finish - _M_start); }
    const unsigned short* GetData() const { return size() > 0 ? _M_start : (const unsigned short*)this; }
};
struct CLandscapeTesselatorBase {
    long vcount; unsigned char vdata[24];
    long GetVertexCount() { return vcount; } const void* GetVertexData() { return vdata; } long GetVertexSize() { return 4; }
    CArrayU16 BridgeIndexBuffer; CTesselationData* Data;
    bool BuildBridgeVertexAndIndexBuffers(long bridge, CMovableResource*)
    {
        if (BridgeIndexBuffer.size() != 0) {
            long size = (long)(BridgeIndexBuffer.size() * sizeof(unsigned short));
            CIndexBuffer* indexBuffer = g_failIndex ? 0 : &g_index;
            if (!indexBuffer) return false;
            unsigned char* indices = indexBuffer->Lock(0, 0, 0);
            std::memcpy(indices, BridgeIndexBuffer.GetData(), size);
            indexBuffer->Unlock();
            Data->BridgeIndexBuffers[bridge] = indexBuffer;
            Data->BridgePolyCounts[bridge] = (long)BridgeIndexBuffer.size() / 3;
        }
        if (GetVertexCount()) {
            CVertexBufferWin32* vertexBuffer = g_failVertex ? 0 : &g_vertex;
            if (!vertexBuffer) return false;
            unsigned char* vertices = vertexBuffer->Lock(0, 0, 0);
            std::memcpy(vertices, GetVertexData(), GetVertexSize() * GetVertexCount());
            vertexBuffer->Unlock();
            Data->BridgeVertexBuffers[bridge] = vertexBuffer;
        }
        return true;
    }
};
int main()
{
    int bad = 0; unsigned short idx[6] = { 1, 2, 3, 4, 5, 6 }; CTesselationData data; std::memset(&data, 0, sizeof(data));
    CLandscapeTesselatorBase t; t.vcount = 3; for (int i = 0; i < 12; ++i) t.vdata[i] = (unsigned char)(i + 1);
    t.BridgeIndexBuffer._M_start = idx; t.BridgeIndexBuffer._M_finish = idx + 6; t.Data = &data;
    g_failIndex = g_failVertex = false;
    if (!t.BuildBridgeVertexAndIndexBuffers(2, 0) || data.BridgeIndexBuffers[2] != &g_index || data.BridgePolyCounts[2] != 2 || data.BridgeVertexBuffers[2] != &g_vertex) bad++;
    if (((unsigned short*)g_ib)[5] != 6 || g_vb[11] != 12 || g_unlocks != 2) bad++;
    g_failVertex = true; if (t.BuildBridgeVertexAndIndexBuffers(1, 0)) bad++;
    g_failIndex = true; t.vcount = 0; if (t.BuildBridgeVertexAndIndexBuffers(0, 0)) bad++;
    t.BridgeIndexBuffer._M_finish = idx; if (!t.BuildBridgeVertexAndIndexBuffers(0, 0)) bad++;   // nothing to build
    if (bad) { std::printf("00be3660_TEST FAIL\n"); return 1; }
    std::printf("00be3660_TEST PASS\n");
    return 0;
}