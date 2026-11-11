// CTCWillEnflame::OnKill() -- does nothing
// Leaf: ?OnKill@CTCWillEnflame@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x007A42F0, thiscall, `ret`.
// Identity: slot 14 of CTCWillEnflame's vtable 0x0126C824 (defined here: the slot holds this body in 1 vtable(s), all of CTCWillEnflame or derived); the Xbox 360 Test PDB virtual order (third source) aligned on 13 TLC-named slots of that vtable puts OnKill(void) at this slot; Ego_r/FableWin name set: ?OnKill@CTCWillEnflame@@UAEXXZ; `ret` matches the signature.
class CTCWillEnflame {
public:
    virtual void OnKill();
};
void CTCWillEnflame::OnKill()
{
}
