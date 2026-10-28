// Behaviour test for CEngineParticleSimulationRenderData's scalar deleting destructor (0x00BB6520): both vertex buffers destroyed (last first), then the base; freed only on flag bit 0.
#include <cstdio>
#include <new>
static int g_log[4]; static int g_n; static int g_frees;
struct Base { virtual ~Base() { g_log[g_n++] = 9; } };
struct VB { int id; VB() : id(0) {} virtual ~VB() { g_log[g_n++] = id; } };
struct RenderData : Base { VB VertexBuffers[2]; void* CurrentVB; RenderData() : CurrentVB(0) { VertexBuffers[0].id = 1; VertexBuffers[1].id = 2; }
    void* ScalarDelete(unsigned int flags) { this->~RenderData(); if (flags & 1) ++g_frees; return this; } };
int main()
{
    unsigned char storage[sizeof(RenderData)]; RenderData* r = new (storage) RenderData;
    g_n = 0; r->ScalarDelete(1);
    if (g_n != 3 || g_log[0] != 2 || g_log[1] != 1 || g_log[2] != 9 || g_frees != 1) { std::printf("00bb6520_TEST FAIL\n"); return 1; }
    r = new (storage) RenderData; g_n = 0; r->ScalarDelete(0);
    if (g_n != 3 || g_frees != 1) { std::printf("00bb6520_TEST FAIL\n"); return 1; }
    std::printf("00bb6520_TEST PASS\n");
    return 0;
}