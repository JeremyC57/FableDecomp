// NParticleEngine::CPSCBase::GetDecalCount() -- returns 0
// Leaf: ?GetDecalCount@CPSCBase@NParticleEngine@@UAEJXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00AF2AA0, thiscall, `ret`.
// Identity: slot 27 of NParticleEngine::CPSCSplineBase's vtable 0x012A0B14 (defined here: the slot holds this body in 11 vtable(s), all of NParticleEngine::CPSCBase or derived); the Xbox 360 Test PDB virtual order (third source) aligned on 11 TLC-named slots of that vtable puts GetDecalCount(void) at this slot; Ego_r/FableWin name set: ?GetDecalCount@CPSCBase@NParticleEngine@@UAEJXZ; `ret` matches the signature.
namespace NParticleEngine {
class CPSCBase {
public:
    virtual long GetDecalCount();
};
long CPSCBase::GetDecalCount()
{
    return 0;
}
}
