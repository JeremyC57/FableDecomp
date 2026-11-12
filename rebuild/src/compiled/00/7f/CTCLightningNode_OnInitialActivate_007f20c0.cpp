// CTCLightningNode::OnInitialActivate() -- does nothing
// Leaf: ?OnInitialActivate@CTCLightningNode@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x007F20C0, thiscall, `ret`.
// Identity: slot 6 of CTCLightningNode's vtable 0x0124337C (defined here: the slot holds this body in 1 vtable(s), all of CTCLightningNode or derived); the Xbox 360 Test PDB virtual order (third source) aligned on 13 TLC-named slots of that vtable puts OnInitialActivate(void) at this slot; Ego_r/FableWin name set: ?OnInitialActivate@CTCLightningNode@@UAEXXZ; `ret` matches the signature.
class CTCLightningNode {
public:
    virtual void OnInitialActivate();
};
void CTCLightningNode::OnInitialActivate()
{
}
