// CTCDParticleEmitter::IsComponentSerialisable() const -- only an independent emitter is serialised (IndependantObject, bit 2 of the +0x44 flags)
// Leaf: ?IsComponentSerialisable@CTCDParticleEmitter@@UBE_NXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x006E1D50, thiscall, `ret`.
// Identity (agentFAM03 proof, re-verified by agentFAM05): slot 23 of CTCDParticleEmitter's vtable 0x0125FEEC (the slot holds this body in 1 vtable(s), owner CTCDParticleEmitter); the Xbox 360 Test PDB virtual order (third source) aligned on 17 TLC-named slots of that vtable puts IsComponentSerialisable(void) at this slot [gap kind GS]; Ego_r/FableWin name set: ?IsComponentSerialisable@CTCDParticleEmitter@@UBE_NXZ; `ret` matches the signature.
// Was: CTCDParticleEmitter::vfunc_23 (rebuild/src/compiled/00/6e/CTCDParticleEmitter_vfunc23_006e1d50.cpp).
#define FABLE_RETAIL_CTCDPARTICLEEMITTER_FULL
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#include "engine/ETCInterfaceType.h"
#include "engine/CTCDParticleEmitter.h"
bool CTCDParticleEmitter::IsComponentSerialisable() const
{
    return IndependantObject;
}
