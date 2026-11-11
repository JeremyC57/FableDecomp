// CTCLightningNode::OnKill() -- does nothing
// Leaf: ?OnKill@CTCLightningNode@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x007F20E0, thiscall, `ret`.
// Identity: slot 14 of CTCLightningNode's vtable 0x0124337C (defined here: the slot holds this body in 1 vtable(s), all of CTCLightningNode or derived); the Xbox 360 Test PDB virtual order (third source) aligned on 13 TLC-named slots of that vtable puts OnKill(void) at this slot; Ego_r/FableWin name set: ?OnKill@CTCLightningNode@@UAEXXZ; `ret` matches the signature.
class CTCLightningNode {
public:
    virtual void OnKill();
};
void CTCLightningNode::OnKill()
{
}
