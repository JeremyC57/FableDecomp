// CTCFireRangedWeaponOnLeavingStateGroup::SetNotAbleToFire() -- sets QuitOut to true
// Leaf: ?SetNotAbleToFire@CTCFireRangedWeaponOnLeavingStateGroup@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x004D65A4, thiscall, `ret`.
// Identity: slot 28 of CTCFireRangedWeaponOnLeavingStateGroup's vtable 0x0123E464 (defined here: the slot holds this body in 1 vtable(s), all of CTCFireRangedWeaponOnLeavingStateGroup or derived); the Xbox 360 Test PDB virtual order (third source) aligned on 15 TLC-named slots of that vtable puts SetNotAbleToFire(void) at this slot; Ego_r/FableWin name set: ?SetNotAbleToFire@CTCFireRangedWeaponOnLeavingStateGroup@@UAEXXZ; `ret` matches the signature.
// Layout: Ego_r.pdb CTCFireRangedWeaponOnLeavingStateGroup has bool QuitOut at +0xC, the offset retail uses.
class CTCFireRangedWeaponOnLeavingStateGroup {
public:
    virtual void SetNotAbleToFire();
private:
    unsigned char _base_0x04[0x8];   // +0x04 base class and earlier members (no shared header)
    bool QuitOut;   // +0xC (Ego_r.pdb CTCFireRangedWeaponOnLeavingStateGroup)
};
void CTCFireRangedWeaponOnLeavingStateGroup::SetNotAbleToFire()
{
    QuitOut = true;
}
