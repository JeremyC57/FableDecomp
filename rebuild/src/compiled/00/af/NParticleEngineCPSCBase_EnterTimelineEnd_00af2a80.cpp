// NParticleEngine::CPSCBase::EnterTimelineEnd() -- does nothing
// Leaf: ?EnterTimelineEnd@CPSCBase@NParticleEngine@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00AF2A80, thiscall, `ret`.
// Identity: slot 25 of NParticleEngine::CPSCSplineBase's vtable 0x012A0B14 (defined here: the slot holds this body in 9 vtable(s), all of NParticleEngine::CPSCBase or derived); the Xbox 360 Test PDB virtual order (third source) aligned on 11 TLC-named slots of that vtable puts EnterTimelineEnd(void) at this slot; Ego_r/FableWin name set: ?EnterTimelineEnd@CPSCBase@NParticleEngine@@UAEXXZ; `ret` matches the signature.
namespace NParticleEngine {
class CPSCBase {
public:
    virtual void EnterTimelineEnd();
};
void CPSCBase::EnterTimelineEnd()
{
}
}
