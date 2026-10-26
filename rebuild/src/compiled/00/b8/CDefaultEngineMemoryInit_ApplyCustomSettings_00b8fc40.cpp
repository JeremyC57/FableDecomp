// CDefaultEngineMemoryInit::ApplyCustomSettings(const CEngineMemoryInit& custom) -- override each memory setting with the custom value when that value is non-negative
// Leaf: ApplyCustomSettings
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B8FC40, length 131 bytes.
// Layout: Ego_r.pdb CEngineMemoryInit (40 bytes, no vptr): LandscapePoolSize +0, LandscapePhysicalMemoryRatio (float) +4,
// LandscapeVirtualMemoryMinSize +8, MeshPoolSize +0xc, MeshStatsPoolSize +0x10, MeshPhysicalMemoryRatio (float) +0x14,
// HiresTextureMemory +0x18, PhysicsMeshPoolSize +0x1c, AnimationPoolSize +0x20, ClothPoolSize +0x24. `this` stores
// LandscapePoolSize at +0, so CDefaultEngineMemoryInit is the non-polymorphic settings record itself (engine/
// CDefaultEngineMemoryInit.h only carries a flattened 0x28-byte blob); modelled locally as a derived struct.
// Shape: ten calls of an inline `ApplyIfSet(setting, custom)` helper taking the custom value BY VALUE, in member order:
// the float value is loaded once and kept on the FPU for the store (`fcom [0.0f]`, not `fcomp` and a reload) (`test ah,1 ; jne` is `>=`; the dropped arm pops with `fstp st(0)`).
class CEngineMemoryInit { public:
    long  LandscapePoolSize;                // +0x00
    float LandscapePhysicalMemoryRatio;     // +0x04
    long  LandscapeVirtualMemoryMinSize;    // +0x08
    long  MeshPoolSize;                     // +0x0c
    long  MeshStatsPoolSize;                // +0x10
    float MeshPhysicalMemoryRatio;          // +0x14
    long  HiresTextureMemory;               // +0x18
    long  PhysicsMeshPoolSize;              // +0x1c
    long  AnimationPoolSize;                // +0x20
    long  ClothPoolSize;                    // +0x24
};

template <class T> static inline void ApplyIfSet(T& setting, T custom)
{
    if (custom >= 0)
        setting = custom;
}

struct CDefaultEngineMemoryInit : CEngineMemoryInit {
    void ApplyCustomSettings(const CEngineMemoryInit& custom);
};

void CDefaultEngineMemoryInit::ApplyCustomSettings(const CEngineMemoryInit& custom)
{
    ApplyIfSet(LandscapePoolSize, custom.LandscapePoolSize);
    ApplyIfSet(LandscapePhysicalMemoryRatio, custom.LandscapePhysicalMemoryRatio);
    ApplyIfSet(LandscapeVirtualMemoryMinSize, custom.LandscapeVirtualMemoryMinSize);
    ApplyIfSet(MeshPoolSize, custom.MeshPoolSize);
    ApplyIfSet(MeshStatsPoolSize, custom.MeshStatsPoolSize);
    ApplyIfSet(MeshPhysicalMemoryRatio, custom.MeshPhysicalMemoryRatio);
    ApplyIfSet(HiresTextureMemory, custom.HiresTextureMemory);
    ApplyIfSet(PhysicsMeshPoolSize, custom.PhysicsMeshPoolSize);
    ApplyIfSet(AnimationPoolSize, custom.AnimationPoolSize);
    ApplyIfSet(ClothPoolSize, custom.ClothPoolSize);
}