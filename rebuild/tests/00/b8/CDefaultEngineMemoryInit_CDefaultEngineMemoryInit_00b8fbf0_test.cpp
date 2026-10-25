// Behaviour test for 0x00B8FBF0 CDefaultEngineMemoryInit::CDefaultEngineMemoryInit().
#include <cstdio>
#include <cstring>

static bool Near(float a, float b) { float d = a - b; return d < 0.0001f && d > -0.0001f; }
struct CEngineMemoryInit {
    long  LandscapePoolSize;
    float LandscapePhysicalMemoryRatio;
    long  LandscapeVirtualMemoryMinSize;
    long  MeshPoolSize;
    long  MeshStatsPoolSize;
    float MeshPhysicalMemoryRatio;
    long  HiresTextureMemory;
    long  PhysicsMeshPoolSize;
    long  AnimationPoolSize;
    long  ClothPoolSize;
};
struct CDefaultEngineMemoryInit : CEngineMemoryInit {
    CDefaultEngineMemoryInit()
    {
        LandscapePoolSize = 0x1A00; LandscapePhysicalMemoryRatio = 0.6f; LandscapeVirtualMemoryMinSize = 0x200;
        MeshPoolSize = 0x22E0; MeshStatsPoolSize = 0x100; MeshPhysicalMemoryRatio = 0.75f; PhysicsMeshPoolSize = 0x400;
        AnimationPoolSize = 0x1800; ClothPoolSize = 0x200; HiresTextureMemory = 0x578;
    }
};

int main()
{
    CDefaultEngineMemoryInit m;
    if (!(m.LandscapePoolSize == 6656 && Near(m.LandscapePhysicalMemoryRatio, 0.6f) && m.LandscapeVirtualMemoryMinSize == 512 && m.MeshPoolSize == 8928 && m.MeshStatsPoolSize == 256 && Near(m.MeshPhysicalMemoryRatio, 0.75f) && m.HiresTextureMemory == 1400 && m.PhysicsMeshPoolSize == 1024 && m.AnimationPoolSize == 6144 && m.ClothPoolSize == 512)) {
        std::printf("00b8fbf0_TEST FAIL\n");
        return 1;
    }
    std::printf("00b8fbf0_TEST PASS\n");
    return 0;
}