// NParticleEngine::CPSCBase::GetNumberOfDecals(float&, float) -- returns false
// Leaf: ?GetNumberOfDecals@CPSCBase@NParticleEngine@@UAE_NAAMM@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00AF2A70, thiscall, `ret 8`.
// Identity: slot 24 of NParticleEngine::CPSCSplineBase's vtable 0x012A0B14 (defined here: the slot holds this body in 11 vtable(s), all of NParticleEngine::CPSCBase or derived); the Xbox 360 Test PDB virtual order (third source) aligned on 11 TLC-named slots of that vtable puts GetNumberOfDecals(float &, float const) at this slot; Ego_r/FableWin name set: ?GetNumberOfDecals@CPSCBase@NParticleEngine@@UAE_NAAMM@Z; `ret` matches the signature.
namespace NParticleEngine {
class CPSCBase {
public:
    virtual bool GetNumberOfDecals(float&, float);
};
bool CPSCBase::GetNumberOfDecals(float&, float)
{
    return false;
}
}
