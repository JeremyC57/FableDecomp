// CEngineInit::CEngineInit() -- default constructor of the engine start-up description: null manager/bank pointers, five empty resource-path strings, memory-pool sizes "unset" (-1 / -1.0f), unlimited static-map quality, no init flags
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0042D830, length 122 bytes.
// Callees: CWideString::CWideString() @ 0x0099AED0 (x5, out of line). Layout: the PDB CEngineInit (0x68 bytes) has nine pointers before the
// strings; the retail object has eight (+0x00..+0x1c), so every later member sits 4 bytes lower: the five CWideString paths +0x20..+0x30,
// CEngineMemoryInit +0x34 (0x28 bytes), StaticMapQuality +0x5c, InitFlags +0x60. The dropped pointer is assumed to be MeshBank (the PDB's
// last pointer); the names of the first eight follow the PDB order.
// Shape: the eight pointer zeroes from one zero register (init list) precede the string constructors; the memory description's
// constructor stores every long -1 from eax and both float ratios as -1.0f (0xBF800000) from ecx, then InitFlags = 0 and
// StaticMapQuality = 0x7fffffff (the scheduler groups the stores by value register).
class CWideString {
public:
    CWideString();                           // 0x0099AED0
    struct CWideStringData* PStringData;
};

struct CEngineMemoryInit {
    long  LandscapePoolSize;                 // +0x00
    float LandscapePhysicalMemoryRatio;      // +0x04
    long  LandscapeVirtualMemoryMinSize;     // +0x08
    long  MeshPoolSize;                      // +0x0c
    long  MeshStatsPoolSize;                 // +0x10
    float MeshPhysicalMemoryRatio;           // +0x14
    long  HiresTextureMemory;                // +0x18
    long  PhysicsMeshPoolSize;               // +0x1c
    long  AnimationPoolSize;                 // +0x20
    long  ClothPoolSize;                     // +0x24
    CEngineMemoryInit()
        : LandscapePoolSize(-1), LandscapePhysicalMemoryRatio(-1.0f), LandscapeVirtualMemoryMinSize(-1), MeshPoolSize(-1),
          MeshStatsPoolSize(-1), MeshPhysicalMemoryRatio(-1.0f), HiresTextureMemory(-1), PhysicsMeshPoolSize(-1),
          AnimationPoolSize(-1), ClothPoolSize(-1)
    {
    }
};

struct CEngineInit {
    struct CDefinitionManager* DefinitionManager;   // +0x00
    struct CEngineDef*         EngineDef;           // +0x04
    struct CRenderManager*     RenderManager;       // +0x08
    struct CDebugString*       DebugString;         // +0x0c
    struct CIDrawWorld*        World;               // +0x10
    struct CFontBank*          Font;                // +0x14
    struct CGraphicDataBank*   MainGraphicBank;     // +0x18
    struct CGraphicDataBank*   TerrainGraphicBank;  // +0x1c
    CWideString       MeshResourcePath;             // +0x20
    CWideString       GraphicsResourcePath;         // +0x24
    CWideString       ShaderResourcePath;           // +0x28
    CWideString       EngineCacheResourcePath;      // +0x2c
    CWideString       WriteableEngineCacheResourcePath; // +0x30
    CEngineMemoryInit MemoryInit;                   // +0x34
    long              StaticMapQuality;             // +0x5c
    long              InitFlags;                    // +0x60
    CEngineInit();
};

CEngineInit::CEngineInit()
    : DefinitionManager(0), EngineDef(0), RenderManager(0), DebugString(0), World(0), Font(0),
      MainGraphicBank(0), TerrainGraphicBank(0), StaticMapQuality(0x7fffffff), InitFlags(0)
{
}