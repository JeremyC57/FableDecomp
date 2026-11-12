// NCameraModes::CRotateAround::OnDie() -- does nothing
// Leaf: ?OnDie@CRotateAround@NCameraModes@@MAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x0069E130, thiscall, `ret`.
// Identity: slot 22 of NCameraModes::CRotateAround's vtable 0x0125C674 (defined here: the slot holds this body in 1 vtable(s), all of NCameraModes::CRotateAround or derived); the Xbox 360 Test PDB virtual order (third source) aligned on 6 TLC-named slots of that vtable puts OnDie(void) at this slot; Ego_r/FableWin name set: ?OnDie@CRotateAround@NCameraModes@@MAEXXZ; `ret` matches the signature.
namespace NCameraModes {
class CRotateAround {
public:
protected:
    virtual void OnDie();
};
void CRotateAround::OnDie()
{
}
}
