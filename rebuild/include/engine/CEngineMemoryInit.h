#ifndef FABLE_ENGINE_CENGINEMEMORYINIT_H
#define FABLE_ENGINE_CENGINEMEMORYINIT_H
// New header (chunk 731, 2026-11-03; one model, no class macro): Ego_r.pdb CEngineMemoryInit (0x28 bytes, no vptr; the PDB
// names ??0CEngineMemoryInit@@QAE@XZ and the ABVCEngineMemoryInit@@ parameters of CEngineResourceManager /
// CEngineLandscapeRenderer / CDefaultEngineMemoryInit::ApplyCustomSettings). Retail CEngineInit holds it at +0x34
// (CEngine::Initialise 0x00B26360 passes `lea eax,[edi+34h]`).
#include "rebuild_abi.h"
class CEngineMemoryInit {
public:
    CEngineMemoryInit();                                                 // ??0CEngineMemoryInit@@QAE@XZ
    long  LandscapePoolSize;                                             // +0x00
    float LandscapePhysicalMemoryRatio;                                  // +0x04
    long  LandscapeVirtualMemoryMinSize;                                 // +0x08
    long  MeshPoolSize;                                                  // +0x0c
    long  MeshStatsPoolSize;                                             // +0x10
    float MeshPhysicalMemoryRatio;                                       // +0x14
    long  HiresTextureMemory;                                            // +0x18
    long  PhysicsMeshPoolSize;                                           // +0x1c
    long  AnimationPoolSize;                                             // +0x20
    long  ClothPoolSize;                                                 // +0x24
};
FABLE_STATIC_ASSERT(sizeof(CEngineMemoryInit) == 0x28);
#endif // FABLE_ENGINE_CENGINEMEMORYINIT_H
