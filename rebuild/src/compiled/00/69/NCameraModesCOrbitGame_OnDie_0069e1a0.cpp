// NCameraModes::COrbitGame::OnDie() -- does nothing
// Leaf: ?OnDie@COrbitGame@NCameraModes@@MAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x0069E1A0, thiscall, `ret`.
// Identity: slot 22 of NCameraModes::COrbitGame's vtable 0x0125C6DC (defined here: the slot holds this body in 1 vtable(s), all of NCameraModes::COrbitGame or derived); the Xbox 360 Test PDB virtual order (third source) aligned on 7 TLC-named slots of that vtable puts OnDie(void) at this slot; Ego_r/FableWin name set: ?OnDie@COrbitGame@NCameraModes@@MAEXXZ; `ret` matches the signature.
namespace NCameraModes {
class COrbitGame {
public:
protected:
    virtual void OnDie();
};
void COrbitGame::OnDie()
{
}
}
