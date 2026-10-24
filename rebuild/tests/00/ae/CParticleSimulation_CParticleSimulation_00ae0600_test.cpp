// Behaviour test for 0x00AE0600 CParticleSimulation::CParticleSimulation (null render data, handle -1, pool 0, then Clear()).
#include <cstdio>

static int g_clears = 0; static long g_handleAtClear = 0;
struct SCounted { void* Data; void* Info; SCounted() : Data(0), Info(0) { } };
struct SMovable { SMovable() { } virtual ~SMovable() { } };
struct SParticleSimulation : SMovable {
    SCounted RenderData; float Timestep; bool TimestepChanged; long MemoryHandle; void* MemoryPool;
    void Clear() { ++g_clears; g_handleAtClear = MemoryHandle; Timestep = 1.0f / 15.0f; TimestepChanged = false; }   // 0x00ADF1E0 (part)
    // reconstructed body
    SParticleSimulation() : MemoryHandle(-1), MemoryPool(0) { Clear(); }
};

int main()
{
    SParticleSimulation s;
    if (s.RenderData.Data != 0 || s.RenderData.Info != 0 || s.MemoryHandle != -1 || s.MemoryPool != 0) { std::printf("00ae0600_TEST FAIL\n"); return 1; }
    if (g_clears != 1 || g_handleAtClear != -1 || s.TimestepChanged || s.Timestep < 0.066f || s.Timestep > 0.067f) { std::printf("00ae0600_TEST FAIL\n"); return 1; }
    std::printf("00ae0600_TEST PASS\n");
    return 0;
}