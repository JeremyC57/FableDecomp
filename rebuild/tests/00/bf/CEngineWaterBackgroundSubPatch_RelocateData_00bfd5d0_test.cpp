// Behaviour test for 0x00BFD5D0 CEngineWaterBackgroundSubPatch::RelocateData: only the matching pointer moves; index buffers notified.
#include <cstdio>

static int g_notified = 0;
class CIndexBuffer { public: void OnRelocated() { ++g_notified; } };
class CVertexBufferWin32 { public: int x; };
template <class T> inline void RelocatePointer(T*& p, void* oldBase, void* newBase)
{
    if (p) p = (T*)((char*)p - (char*)oldBase + (long)newBase);
}
class CEngineWaterBackgroundSubPatch { public:
    CIndexBuffer* IndexBuffer; CVertexBufferWin32* VertexBuffer;
    CIndexBuffer* BridgeIndexBuffers[4]; CVertexBufferWin32* BridgeVertexBuffers[4];
    void RelocateData(long, void* oldBase, void* newBase, long)
    {
        for (long i = 0; i < 4; ++i) {
            if (oldBase == BridgeIndexBuffers[i]) { RelocatePointer(BridgeIndexBuffers[i], oldBase, newBase); BridgeIndexBuffers[i]->OnRelocated(); return; }
            if (oldBase == BridgeVertexBuffers[i]) { RelocatePointer(BridgeVertexBuffers[i], oldBase, newBase); return; }
        }
        if (oldBase == IndexBuffer) { RelocatePointer(IndexBuffer, oldBase, newBase); IndexBuffer->OnRelocated(); return; }
        if (oldBase == VertexBuffer) RelocatePointer(VertexBuffer, oldBase, newBase);
    }
};

int main()
{
    static CIndexBuffer ib[8]; static CVertexBufferWin32 vb[8];
    CEngineWaterBackgroundSubPatch p;
    p.IndexBuffer = &ib[0]; p.VertexBuffer = &vb[0];
    for (int i = 0; i < 4; ++i) { p.BridgeIndexBuffers[i] = &ib[1 + i]; p.BridgeVertexBuffers[i] = &vb[1 + i]; }
    p.RelocateData(0, &ib[2], &ib[6], 0);
    if (p.BridgeIndexBuffers[1] != &ib[6] || g_notified != 1 || p.BridgeIndexBuffers[0] != &ib[1]) { std::printf("00bfd5d0_TEST FAIL\n"); return 1; }
    p.RelocateData(0, &vb[3], &vb[7], 0);
    if (p.BridgeVertexBuffers[2] != &vb[7] || g_notified != 1) { std::printf("00bfd5d0_TEST FAIL\n"); return 1; }
    p.RelocateData(0, &ib[0], &ib[7], 0);
    if (p.IndexBuffer != &ib[7] || g_notified != 2) { std::printf("00bfd5d0_TEST FAIL\n"); return 1; }
    p.RelocateData(0, &vb[0], &vb[6], 0);
    if (p.VertexBuffer != &vb[6] || g_notified != 2) { std::printf("00bfd5d0_TEST FAIL\n"); return 1; }
    std::printf("00bfd5d0_TEST PASS\n");
    return 0;
}