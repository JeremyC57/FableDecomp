// CTCActivationTrigger::OnHitByShot() -- does nothing
// Leaf: ?OnHitByShot@CTCActivationTrigger@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x004D43F4, thiscall, `ret`.
// Identity: slot 30 of CTCActivationTrigger's vtable 0x0123BBAC (defined here: the slot holds this body in 1 vtable(s), all of CTCActivationTrigger or derived); the Xbox 360 Test PDB virtual order (third source) aligned on 15 TLC-named slots of that vtable puts OnHitByShot(void) at this slot; Ego_r/FableWin name set: ?OnHitByShot@CTCActivationTrigger@@UAEXXZ; `ret` matches the signature.
class CTCActivationTrigger {
public:
    virtual void OnHitByShot();
};
void CTCActivationTrigger::OnHitByShot()
{
}
