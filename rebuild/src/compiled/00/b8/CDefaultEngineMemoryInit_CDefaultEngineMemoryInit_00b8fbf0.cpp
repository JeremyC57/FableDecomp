// CDefaultEngineMemoryInit::CDefaultEngineMemoryInit() -- the engine's default memory budget: landscape, mesh, mesh-stats, hi-res texture, physics-mesh, animation and cloth pool sizes and the two physical-memory ratios
// Leaf: ??0CDefaultEngineMemoryInit@@QAE@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00B8FBF0, 69 bytes, __fastcall `ret`, returns `this`. No callees.
// Identity (label correction): the catalog's "pair<EHeroMorphType,CParticleMorphs::CEntry>" is propagated. The only caller,
//   CEngine::Initialise (0x00B26519), builds a frame object with it and immediately calls
//   CDefaultEngineMemoryInit::ApplyCustomSettings (landed 0x00B8FC40) on it; FableWin.pdb ??0CDefaultEngineMemoryInit@@QAE@XZ.
//   Layout from Ego_r.pdb CEngineMemoryInit (40 bytes, no vptr), as in the landed ApplyCustomSettings model.
// Model: engine/CDefaultEngineMemoryInit.h is not used: it flattens the base into a `__vftable` at +0 plus 0x24 opaque
//   bytes, while retail stores LandscapePoolSize at +0 (no vptr) -- the named CEngineMemoryInit members are needed.
// Shape: body assignments to the base record's members (they cannot sit in this class's initialiser list); VC7.1 keeps
//   the shared 0x200 in ecx and stores HiresTextureMemory last, so the source sets it last.
struct CEngineMemoryInit {
    long  LandscapePoolSize;                            // +0x00
    float LandscapePhysicalMemoryRatio;                 // +0x04
    long  LandscapeVirtualMemoryMinSize;                // +0x08
    long  MeshPoolSize;                                 // +0x0C
    long  MeshStatsPoolSize;                            // +0x10
    float MeshPhysicalMemoryRatio;                      // +0x14
    long  HiresTextureMemory;                           // +0x18
    long  PhysicsMeshPoolSize;                          // +0x1C
    long  AnimationPoolSize;                            // +0x20
    long  ClothPoolSize;                                // +0x24
};

struct CDefaultEngineMemoryInit : CEngineMemoryInit {
    CDefaultEngineMemoryInit();
};

CDefaultEngineMemoryInit::CDefaultEngineMemoryInit()
{
    LandscapePoolSize = 0x1A00;
    LandscapePhysicalMemoryRatio = 0.6f;
    LandscapeVirtualMemoryMinSize = 0x200;
    MeshPoolSize = 0x22E0;
    MeshStatsPoolSize = 0x100;
    MeshPhysicalMemoryRatio = 0.75f;
    PhysicsMeshPoolSize = 0x400;
    AnimationPoolSize = 0x1800;
    ClothPoolSize = 0x200;
    HiresTextureMemory = 0x578;
}