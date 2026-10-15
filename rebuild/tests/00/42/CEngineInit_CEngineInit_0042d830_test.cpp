// Behaviour test for CEngineInit::CEngineInit @ 0x0042D830
#include <cstdio>
static int g_wideStrings = 0;
class CWideString {
public:
    CWideString() : PStringData(0) { ++g_wideStrings; }
    void* PStringData;
};
struct CEngineMemoryInit {
    long LandscapePoolSize; float LandscapePhysicalMemoryRatio; long LandscapeVirtualMemoryMinSize, MeshPoolSize, MeshStatsPoolSize;
    float MeshPhysicalMemoryRatio; long HiresTextureMemory, PhysicsMeshPoolSize, AnimationPoolSize, ClothPoolSize;
    CEngineMemoryInit()
        : LandscapePoolSize(-1), LandscapePhysicalMemoryRatio(-1.0f), LandscapeVirtualMemoryMinSize(-1), MeshPoolSize(-1),
          MeshStatsPoolSize(-1), MeshPhysicalMemoryRatio(-1.0f), HiresTextureMemory(-1), PhysicsMeshPoolSize(-1),
          AnimationPoolSize(-1), ClothPoolSize(-1) {}
};
struct CEngineInit {
    void* DefinitionManager; void* EngineDef; void* RenderManager; void* DebugString; void* World; void* Font;
    void* MainGraphicBank; void* TerrainGraphicBank;
    CWideString MeshResourcePath, GraphicsResourcePath, ShaderResourcePath, EngineCacheResourcePath, WriteableEngineCacheResourcePath;
    CEngineMemoryInit MemoryInit;
    long StaticMapQuality, InitFlags;
    CEngineInit()
        : DefinitionManager(0), EngineDef(0), RenderManager(0), DebugString(0), World(0), Font(0),
          MainGraphicBank(0), TerrainGraphicBank(0), StaticMapQuality(0x7fffffff), InitFlags(0) {}
};
int main()
{
    CEngineInit e;
    if (g_wideStrings != 5 || e.DefinitionManager || e.TerrainGraphicBank || e.MeshResourcePath.PStringData ||
        e.MemoryInit.LandscapePoolSize != -1 || e.MemoryInit.LandscapePhysicalMemoryRatio != -1.0f ||
        e.MemoryInit.MeshPhysicalMemoryRatio != -1.0f || e.MemoryInit.ClothPoolSize != -1 ||
        e.StaticMapQuality != 0x7fffffff || e.InitFlags != 0) {
        std::printf("0042d830_TEST FAIL\n"); return 1;
    }
    std::printf("0042d830_TEST PASS\n");
    return 0;
}