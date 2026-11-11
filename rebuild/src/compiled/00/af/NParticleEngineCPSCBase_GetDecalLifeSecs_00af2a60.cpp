// NParticleEngine::CPSCBase::GetDecalLifeSecs(float&) -- returns false
// Leaf: ?GetDecalLifeSecs@CPSCBase@NParticleEngine@@UAE_NAAM@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00AF2A60, thiscall, `ret 4`.
// Identity: slot 23 of NParticleEngine::CPSCSplineBase's vtable 0x012A0B14 (defined here: the slot holds this body in 11 vtable(s), all of NParticleEngine::CPSCBase or derived); the Xbox 360 Test PDB virtual order (third source) aligned on 11 TLC-named slots of that vtable puts GetDecalLifeSecs(float &) at this slot; Ego_r/FableWin name set: ?GetDecalLifeSecs@CPSCBase@NParticleEngine@@UAE_NAAM@Z; `ret` matches the signature.
namespace NParticleEngine {
class CPSCBase {
public:
    virtual bool GetDecalLifeSecs(float&);
};
bool CPSCBase::GetDecalLifeSecs(float&)
{
    return false;
}
}
