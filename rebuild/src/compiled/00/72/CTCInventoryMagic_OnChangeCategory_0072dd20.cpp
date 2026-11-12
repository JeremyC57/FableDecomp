// CTCInventoryMagic::OnChangeCategory() -- does nothing
// Leaf: ?OnChangeCategory@CTCInventoryMagic@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x0072DD20, thiscall, `ret`.
// Identity: slot 37 of CTCInventoryMagic's vtable 0x012668EC (defined here: the slot holds this body in 1 vtable(s), all of CTCInventoryMagic or derived); the Xbox 360 Test PDB virtual order (third source) aligned on 34 TLC-named slots of that vtable puts OnChangeCategory(void) at this slot; Ego_r/FableWin name set: ?OnChangeCategory@CTCInventoryMagic@@UAEXXZ; `ret` matches the signature.
class CTCInventoryMagic {
public:
    virtual void OnChangeCategory();
};
void CTCInventoryMagic::OnChangeCategory()
{
}
