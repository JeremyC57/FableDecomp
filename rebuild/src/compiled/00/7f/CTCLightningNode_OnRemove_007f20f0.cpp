// CTCLightningNode::OnRemove() -- does nothing
// Leaf: ?OnRemove@CTCLightningNode@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x007F20F0, thiscall, `ret`.
// Identity: slot 2 of CTCLightningNode's vtable 0x0124337C (defined here: the slot holds this body in 1 vtable(s), all of CTCLightningNode or derived); the Xbox 360 Test PDB virtual order (third source) aligned on 13 TLC-named slots of that vtable puts OnRemove(void) at this slot; Ego_r/FableWin name set: ?OnRemove@CTCLightningNode@@UAEXXZ; `ret` matches the signature.
class CTCLightningNode {
public:
    virtual void OnRemove();
};
void CTCLightningNode::OnRemove()
{
}
