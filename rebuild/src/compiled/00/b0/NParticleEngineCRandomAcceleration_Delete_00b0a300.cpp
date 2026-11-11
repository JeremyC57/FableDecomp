// NParticleEngine::CRandomAcceleration::Delete() -- deletes the object (the virtual scalar deleting destructor, slot 0)
// Leaf: ?Delete@CRandomAcceleration@NParticleEngine@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00B0A300, thiscall, `ret`.
// Identity (agentFAM03 proof, re-verified by agentFAM05): slot 1 of NParticleEngine::CRandomAcceleration's vtable 0x0129FC08 (the slot holds this body in 1 vtable(s), owner NParticleEngine::CRandomAcceleration); the Xbox 360 Test PDB virtual order (third source) aligned on 0 TLC-named slots of that vtable puts Delete(void) at this slot [gap kind GS]; Ego_r/FableWin name set: ?Delete@CRandomAcceleration@NParticleEngine@@UAEXXZ; `ret` matches the signature.
// Was: NParticleEngine::CRandomAcceleration::vfunc_1 (rebuild/src/compiled/00/b0/gapscan_sub00b0a300_00b0a300.cpp).
// No shared header for NParticleEngine::CRandomAcceleration: minimal local definition on its RTTI base CBaseClassNonCopyable.
#include "engine/CBaseClassNonCopyable.h"
namespace NParticleEngine {
class CRandomAcceleration : public CBaseClassNonCopyable {
public:
    virtual void Delete();                                   // slot 1
};
void CRandomAcceleration::Delete()
{
    delete this;
}
}
