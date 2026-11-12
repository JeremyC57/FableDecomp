// NParticleEngine::CPSCBase::OnCreate() -- does nothing
// Leaf: ?OnCreate@CPSCBase@NParticleEngine@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00AF29B0, thiscall, `ret`.
// Identity: slot 7 of NParticleEngine::CPSCSplineBase's vtable 0x012A0B14 (defined here: the slot holds this body in 12 vtable(s), all of NParticleEngine::CPSCBase or derived); the Xbox 360 Test PDB virtual order (third source) aligned on 11 TLC-named slots of that vtable puts OnCreate(void) at this slot; Ego_r/FableWin name set: ?OnCreate@CPSCBase@NParticleEngine@@UAEXXZ; `ret` matches the signature.
namespace NParticleEngine {
class CPSCBase {
public:
    virtual void OnCreate();
};
void CPSCBase::OnCreate()
{
}
}
