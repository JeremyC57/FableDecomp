// CTCLightningNode::HasFrameUpdate() const -- returns true
// Leaf: ?HasFrameUpdate@CTCLightningNode@@UBE_NXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x004E6D87, thiscall, `ret`.
// Identity: slot 8 of CTCLightningNode's vtable 0x0124337C (defined here: the slot holds this body in 1 vtable(s), all of CTCLightningNode or derived); the Xbox 360 Test PDB virtual order (third source) aligned on 13 TLC-named slots of that vtable puts HasFrameUpdate(void) at this slot; Ego_r/FableWin name set: ?HasFrameUpdate@CTCLightningNode@@UBE_NXZ; `ret` matches the signature.
class CTCLightningNode {
public:
    virtual bool HasFrameUpdate() const;
};
bool CTCLightningNode::HasFrameUpdate() const
{
    return true;
}
