#ifndef FABLE_ENGINE_C3DCLOTHPRIMITIVE_H
#define FABLE_ENGINE_C3DCLOTHPRIMITIVE_H
// NEW header (agentC6; rows 0x00BB3E80 RenderClothPrimitive / 0x00BB30A0 Render2DClothPrimitive -- the model chunk 2060 / agentC2 kept
// local in the row). Ego_r.pdb C3DClothPrimitive, 0x14 bytes, polymorphic (vfptr +0x00): ParticleProgram +0x04
// (CCountedPointer<CParticleProgram>), PrimitiveIndex +0x0C, MaterialIndex +0x10 (struct_layouts_egor.tsv; retail reads +0x04 / +0x10
// at 0x00BB3E9x and 0x00BB30AB / 0x00BB3BC2). Inline accessors: FableWin.pdb ?GetParticleProgram@C3DClothPrimitive@@QBEPAVCParticleProgram@@XZ
// and ?GetMaterialIndex@C3DClothPrimitive@@QBEKXZ. The vtable is not modelled (no virtual is called through it here).
#include "rebuild_abi.h"
#include "engine/CCountedPointer.h"
class CParticleProgram;
class C3DClothPrimitive {
public:
#ifdef FABLE_RETAIL_C3DCLOTHPRIMITIVE_GETPARTICLEPROGRAM_GET
    // Opt-in (agentC8, rows 0x00BB30A0 / 0x00BB3E80): FableWin.pdb's debug ?GetParticleProgram@C3DClothPrimitive@@QBEPAVCParticleProgram@@XZ
    // is 10 instructions / 22 bytes = a call of the smart pointer's Get() (?Get@?$CCountedPointer@VCParticleProgram@@@@QBEPAVCParticleProgram@@XZ,
    // also in FableWin.pdb); the member read is 9 / 17. Same release bytes.
    CParticleProgram* GetParticleProgram() const { return ParticleProgram.Get(); }
#else
    CParticleProgram* GetParticleProgram() const { return ParticleProgram.Data; }   // FableWin ?GetParticleProgram@C3DClothPrimitive@@QBEPAVCParticleProgram@@XZ
#endif
    unsigned long GetMaterialIndex() const { return MaterialIndex; }                // FableWin ?GetMaterialIndex@C3DClothPrimitive@@QBEKXZ
    void* _vftable;
    CCountedPointer<CParticleProgram> ParticleProgram;                              // +0x04
    unsigned long PrimitiveIndex;                                                   // +0x0C
    unsigned long MaterialIndex;                                                    // +0x10
};
FABLE_STATIC_ASSERT_AT(20, sizeof(C3DClothPrimitive) == 0x14);
#endif
