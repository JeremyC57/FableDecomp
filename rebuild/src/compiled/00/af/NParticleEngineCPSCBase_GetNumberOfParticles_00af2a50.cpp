// NParticleEngine::CPSCBase::GetNumberOfParticles(float&, float) -- returns false
// Leaf: ?GetNumberOfParticles@CPSCBase@NParticleEngine@@UAE_NAAMM@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00AF2A50, thiscall, `ret 8`.
// Identity: slot 22 of NParticleEngine::CPSCSplineBase's vtable 0x012A0B14 (defined here: the slot holds this body in 11 vtable(s), all of NParticleEngine::CPSCBase or derived); the Xbox 360 Test PDB virtual order (third source) aligned on 11 TLC-named slots of that vtable puts GetNumberOfParticles(float &, float const) at this slot; Ego_r/FableWin name set: ?GetNumberOfParticles@CPSCBase@NParticleEngine@@UAE_NAAMM@Z; `ret` matches the signature.
namespace NParticleEngine {
class CPSCBase {
public:
    virtual bool GetNumberOfParticles(float&, float);
};
bool CPSCBase::GetNumberOfParticles(float&, float)
{
    return false;
}
}
