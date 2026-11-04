#ifndef FABLE_ENGINE_NUISYSTEM_CPARTICLEEFFECT_H
#define FABLE_ENGINE_NUISYSTEM_CPARTICLEEFFECT_H
// NUISystem::CParticleEffect (new, chunk 765): RTTI .?AVCParticleEffect@NUISystem@@, single direct base NUISystem::CChangingStateComponent at +0 (ghidra_out/rtti_bases.tsv).
// Vtables 0x0124735C (+0, slot 0 the ??_G 0x005440B0), 0x01247338 (+4, CObserver) and 0x01247330 (+0x18, CActionParam) (rtti-vtables.tsv).
// Name set: Ego_r.pdb / FableWin.pdb ??0CParticleEffect@NUISystem@@QAE@J@Z, ??0CParticleEffect@NUISystem@@QAE@ABV01@@Z, ??1CParticleEffect@NUISystem@@UAE@XZ.
// Ego_r.pdb members PHandle (CParticleEmitter*) +396, EffectIndex +400 (struct_layouts_egor.tsv) sit 0x30 lower in retail
// (retail CChangingStateComponent is 0x15C): the constructor 0x00544010 clears +0x15C before GetDataFromDef 0x00543FA0.
// Overrides are not declared (no row calls them).
#include "engine/NUISystem_CChangingStateComponent.h"
class CParticleEmitter;
namespace NUISystem {
class CParticleEffect : public CChangingStateComponent {
public:
    CParticleEffect(long);
    CParticleEffect(const CParticleEffect&);
    virtual ~CParticleEffect();
    void GetDataFromDef();                                          // 0x00543FA0
protected:
    CParticleEmitter* PHandle;                                      // +0x15c
    unsigned long EffectIndex;                                      // +0x160
};
}
FABLE_STATIC_ASSERT(sizeof(NUISystem::CParticleEffect) == 0x164);
#endif
