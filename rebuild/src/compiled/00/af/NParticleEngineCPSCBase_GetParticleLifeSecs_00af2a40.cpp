// NParticleEngine::CPSCBase::GetParticleLifeSecs(float&) -- returns false
// Leaf: ?GetParticleLifeSecs@CPSCBase@NParticleEngine@@UAE_NAAM@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00AF2A40, thiscall, `ret 4`.
// Identity: slot 21 of NParticleEngine::CPSCSplineBase's vtable 0x012A0B14 (defined here: the slot holds this body in 11 vtable(s), all of NParticleEngine::CPSCBase or derived); the Xbox 360 Test PDB virtual order (third source) aligned on 11 TLC-named slots of that vtable puts GetParticleLifeSecs(float &) at this slot; Ego_r/FableWin name set: ?GetParticleLifeSecs@CPSCBase@NParticleEngine@@UAE_NAAM@Z; `ret` matches the signature.
namespace NParticleEngine {
class CPSCBase {
public:
    virtual bool GetParticleLifeSecs(float&);
};
bool CPSCBase::GetParticleLifeSecs(float&)
{
    return false;
}
}
