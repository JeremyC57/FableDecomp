// NCameraModes::CPCMouseLook::OnDie() -- does nothing
// Leaf: ?OnDie@CPCMouseLook@NCameraModes@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00873BB0, thiscall, `ret`.
// Identity: slot 22 of NCameraModes::CPCMouseLook's vtable 0x01277784 (defined here: the slot holds this body in 1 vtable(s), all of NCameraModes::CPCMouseLook or derived); the Xbox 360 Test PDB virtual order (third source) aligned on 9 TLC-named slots of that vtable puts OnDie(void) at this slot; Ego_r/FableWin name set: ?OnDie@CPCMouseLook@NCameraModes@@UAEXXZ; `ret` matches the signature.
namespace NCameraModes {
class CPCMouseLook {
public:
    virtual void OnDie();
};
void CPCMouseLook::OnDie()
{
}
}
