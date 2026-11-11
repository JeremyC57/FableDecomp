// NParticleEngine::CAEnvironmentInteraction::IsWindActive() const -- returns true
// Leaf: ?IsWindActive@CAEnvironmentInteraction@NParticleEngine@@UBE_NXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00415CC0, thiscall, `ret`.
// Identity: slot 2 of CParticleEnvironmentInteraction's vtable 0x0122ED8C (defined here: the slot holds this body in 2 vtable(s), all of NParticleEngine::CAEnvironmentInteraction or derived); the Xbox 360 Test PDB virtual order (third source) aligned on 5 TLC-named slots of that vtable puts IsWindActive(void) at this slot; Ego_r/FableWin name set: ?IsWindActive@CAEnvironmentInteraction@NParticleEngine@@UBE_NXZ; `ret` matches the signature.
namespace NParticleEngine {
class CAEnvironmentInteraction {
public:
    virtual bool IsWindActive() const;
};
bool CAEnvironmentInteraction::IsWindActive() const
{
    return true;
}
}
