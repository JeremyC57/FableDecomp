// Behaviour test for 0x00B8FC40 CDefaultEngineMemoryInit::ApplyCustomSettings: non-negative custom values override the
// defaults, negative ones (the "unset" marker) leave them.
#include <cstdio>
class CEngineMemoryInit { public:
    int LandscapePoolSize; float LandscapePhysicalMemoryRatio; int LandscapeVirtualMemoryMinSize, MeshPoolSize, MeshStatsPoolSize;
    float MeshPhysicalMemoryRatio; int HiresTextureMemory, PhysicsMeshPoolSize, AnimationPoolSize, ClothPoolSize;
};
template <class T> static void ApplyIfSet(T& setting, T custom) { if (custom >= 0) setting = custom; }
struct CDefaultEngineMemoryInit : CEngineMemoryInit {
    void ApplyCustomSettings(const CEngineMemoryInit& c)
    {
        ApplyIfSet(LandscapePoolSize, c.LandscapePoolSize); ApplyIfSet(LandscapePhysicalMemoryRatio, c.LandscapePhysicalMemoryRatio);
        ApplyIfSet(LandscapeVirtualMemoryMinSize, c.LandscapeVirtualMemoryMinSize); ApplyIfSet(MeshPoolSize, c.MeshPoolSize);
        ApplyIfSet(MeshStatsPoolSize, c.MeshStatsPoolSize); ApplyIfSet(MeshPhysicalMemoryRatio, c.MeshPhysicalMemoryRatio);
        ApplyIfSet(HiresTextureMemory, c.HiresTextureMemory); ApplyIfSet(PhysicsMeshPoolSize, c.PhysicsMeshPoolSize);
        ApplyIfSet(AnimationPoolSize, c.AnimationPoolSize); ApplyIfSet(ClothPoolSize, c.ClothPoolSize);
    }
};
int main()
{
    CDefaultEngineMemoryInit d;
    d.LandscapePoolSize = 1; d.LandscapePhysicalMemoryRatio = 0.5f; d.LandscapeVirtualMemoryMinSize = 3; d.MeshPoolSize = 4; d.MeshStatsPoolSize = 5;
    d.MeshPhysicalMemoryRatio = 0.25f; d.HiresTextureMemory = 7; d.PhysicsMeshPoolSize = 8; d.AnimationPoolSize = 9; d.ClothPoolSize = 10;
    CEngineMemoryInit c;
    c.LandscapePoolSize = 100; c.LandscapePhysicalMemoryRatio = -1.0f; c.LandscapeVirtualMemoryMinSize = -1; c.MeshPoolSize = 0; c.MeshStatsPoolSize = -5;
    c.MeshPhysicalMemoryRatio = 0.0f; c.HiresTextureMemory = 70; c.PhysicsMeshPoolSize = -1; c.AnimationPoolSize = 90; c.ClothPoolSize = -2;
    d.ApplyCustomSettings(c);
    if (d.LandscapePoolSize != 100 || d.LandscapePhysicalMemoryRatio != 0.5f || d.LandscapeVirtualMemoryMinSize != 3 || d.MeshPoolSize != 0
        || d.MeshStatsPoolSize != 5 || d.MeshPhysicalMemoryRatio != 0.0f || d.HiresTextureMemory != 70 || d.PhysicsMeshPoolSize != 8
        || d.AnimationPoolSize != 90 || d.ClothPoolSize != 10) { std::printf("00b8fc40_TEST FAIL\n"); return 1; }
    std::printf("00b8fc40_TEST PASS\n");
    return 0;
}