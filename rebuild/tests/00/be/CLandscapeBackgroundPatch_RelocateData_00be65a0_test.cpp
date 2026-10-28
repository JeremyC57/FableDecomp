// Behaviour test for CLandscapeBackgroundPatch::RelocateData @ 0x00BE65A0 (copy of the model with stub notifications).
#include <cstdio>
static int g_indexNotes, g_waterNotes, g_stripCalls;
class CIndexBuffer { public: void OnPostRelocation() { ++g_indexNotes; } };
class CEngineWaterBackgroundSubPatch { public: void OnPostRelocation() { ++g_waterNotes; } };
struct CVertexBuffer {};
class CTexture { public:};
class CPatchTesselationEdgeStrip { public:
    char* data;
    bool RelocateData(void* oldBase, void* newBase) { ++g_stripCalls; if (data != oldBase) return false; data = (char*)newBase; return true; }
};
template <class T> inline void RelocatePointer(T*& p, void* oldBase, void* newBase) { if (p) p = (T*)((char*)p - (char*)oldBase + (long)newBase); }
class CLandscapeBackgroundPatch { public:
    CPatchTesselationEdgeStrip EdgeStrips[4]; CTexture* Texture; CIndexBuffer* IndexBuffer; CVertexBuffer* VertexBuffer;
    CIndexBuffer* BridgeIndexBuffers[4]; CVertexBuffer* BridgeVertexBuffers[4]; CEngineWaterBackgroundSubPatch* WaterSubPatch; bool SharedIndexBuffer;
    void RelocateData(long, void* oldBase, void* newBase, long)
    {
        if (WaterSubPatch == oldBase) { RelocatePointer(WaterSubPatch, oldBase, newBase); WaterSubPatch->OnPostRelocation(); return; }
        for (long i = 0; i < 4; ++i) {
            if (oldBase == BridgeIndexBuffers[i]) { RelocatePointer(BridgeIndexBuffers[i], oldBase, newBase); BridgeIndexBuffers[i]->OnPostRelocation(); return; }
            if (oldBase == BridgeVertexBuffers[i]) { RelocatePointer(BridgeVertexBuffers[i], oldBase, newBase); return; }
            if (EdgeStrips[i].RelocateData(oldBase, newBase)) return;
        }
        if (oldBase == Texture) { RelocatePointer(Texture, oldBase, newBase); return; }
        if (oldBase == IndexBuffer && !SharedIndexBuffer) { RelocatePointer(IndexBuffer, oldBase, newBase); IndexBuffer->OnPostRelocation(); return; }
        if (oldBase == VertexBuffer) RelocatePointer(VertexBuffer, oldBase, newBase);
    }
};
static char g_old[64], g_new[64];
int main()
{
    int bad = 0; CLandscapeBackgroundPatch p;
    for (int i = 0; i < 4; ++i) { p.EdgeStrips[i].data = 0; p.BridgeIndexBuffers[i] = 0; p.BridgeVertexBuffers[i] = 0; }
    p.Texture = 0; p.IndexBuffer = 0; p.VertexBuffer = 0; p.WaterSubPatch = 0; p.SharedIndexBuffer = false;
    p.WaterSubPatch = (CEngineWaterBackgroundSubPatch*)g_old; p.RelocateData(0, g_old, g_new, 0);
    if ((char*)p.WaterSubPatch != g_new || g_waterNotes != 1 || g_stripCalls) bad++;
    p.BridgeIndexBuffers[2] = (CIndexBuffer*)g_old; p.RelocateData(0, g_old, g_new, 0);
    if ((char*)p.BridgeIndexBuffers[2] != g_new || g_indexNotes != 1 || g_stripCalls != 2) bad++;
    p.BridgeVertexBuffers[1] = (CVertexBuffer*)(g_old + 8); p.RelocateData(0, g_old + 8, g_new + 8, 0);
    if ((char*)p.BridgeVertexBuffers[1] != g_new + 8) bad++;
    p.EdgeStrips[3].data = g_old + 16; p.RelocateData(0, g_old + 16, g_new + 16, 0); if (p.EdgeStrips[3].data != g_new + 16) bad++;
    p.Texture = (CTexture*)(g_old + 24); p.RelocateData(0, g_old + 24, g_new + 24, 0); if ((char*)p.Texture != g_new + 24) bad++;
    p.IndexBuffer = (CIndexBuffer*)(g_old + 32); p.SharedIndexBuffer = true; p.RelocateData(0, g_old + 32, g_new + 32, 0);
    if ((char*)p.IndexBuffer != g_old + 32 || g_indexNotes != 1) bad++;
    p.SharedIndexBuffer = false; p.RelocateData(0, g_old + 32, g_new + 32, 0); if ((char*)p.IndexBuffer != g_new + 32 || g_indexNotes != 2) bad++;
    p.VertexBuffer = (CVertexBuffer*)(g_old + 40); p.RelocateData(0, g_old + 40, g_new + 40, 0); if ((char*)p.VertexBuffer != g_new + 40) bad++;
    if (bad) { std::printf("00be65a0_TEST FAIL\n"); return 1; }
    std::printf("00be65a0_TEST PASS\n");
    return 0;
}