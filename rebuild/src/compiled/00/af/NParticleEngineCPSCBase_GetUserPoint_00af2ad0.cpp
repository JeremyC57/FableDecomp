// NParticleEngine::CPSCBase::GetUserPoint(C3DVector&) const -- returns false
// Leaf: ?GetUserPoint@CPSCBase@NParticleEngine@@UBE_NAAVC3DVector@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00AF2AD0, thiscall, `ret 4`.
// Identity: slot 30 of NParticleEngine::CPSCLight's vtable 0x012A0624 (defined here: the slot holds this body in 12 vtable(s), all of NParticleEngine::CPSCBase or derived); the Xbox 360 Test PDB virtual order (third source) aligned on 16 TLC-named slots of that vtable puts GetUserPoint(C3DVector &) at this slot; Ego_r/FableWin name set: ?GetUserPoint@CPSCBase@NParticleEngine@@UBE_NAAVC3DVector@@@Z; `ret` matches the signature.
class C3DVector;
namespace NParticleEngine {
class CPSCBase {
public:
    virtual bool GetUserPoint(C3DVector&) const;
};
bool CPSCBase::GetUserPoint(C3DVector&) const
{
    return false;
}
}
