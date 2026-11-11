// NParticleEngine::CParticle::Delete() -- deletes the object (the virtual scalar deleting destructor, slot 0)
// Leaf: ?Delete@CParticle@NParticleEngine@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00AF7AF0, thiscall, `ret`.
// Identity (agentFAM03 proof, re-verified by agentFAM05): slot 1 of NParticleEngine::CParticle's vtable 0x0129F4F0 (the slot holds this body in 1 vtable(s), owner NParticleEngine::CParticle); the Xbox 360 Test PDB virtual order (third source) aligned on 0 TLC-named slots of that vtable puts Delete(void) at this slot [gap kind GS]; Ego_r/FableWin name set: ?Delete@CParticle@NParticleEngine@@UAEXXZ; `ret` matches the signature.
// Was: NParticleEngine::CParticle::vfunc_1 (rebuild/src/compiled/00/af/gapscan_sub00af7af0_00af7af0.cpp).
// No shared header for NParticleEngine::CParticle: minimal local definition on its RTTI base CBaseClassNonCopyable.
#include "engine/CBaseClassNonCopyable.h"
namespace NParticleEngine {
class CParticle : public CBaseClassNonCopyable {
public:
    virtual void Delete();                                   // slot 1
};
void CParticle::Delete()
{
    delete this;
}
}
