// CTCDrainLife::OnKill() -- does nothing
// Leaf: ?OnKill@CTCDrainLife@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x007C3020, thiscall, `ret`.
// Identity: slot 14 of CTCDrainLife's vtable 0x0126E284 (defined here: the slot holds this body in 1 vtable(s), all of CTCDrainLife or derived); the Xbox 360 Test PDB virtual order (third source) aligned on 14 TLC-named slots of that vtable puts OnKill(void) at this slot; Ego_r/FableWin name set: ?OnKill@CTCDrainLife@@UAEXXZ; `ret` matches the signature.
class CTCDrainLife {
public:
    virtual void OnKill();
};
void CTCDrainLife::OnKill()
{
}
