// CTCFireRangedWeaponOnLeavingStateGroup::HasFrameUpdate() const -- returns true
// Leaf: ?HasFrameUpdate@CTCFireRangedWeaponOnLeavingStateGroup@@UBE_NXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x004D65A1, thiscall, `ret`.
// Identity: slot 8 of CTCFireRangedWeaponOnLeavingStateGroup's vtable 0x0123E464 (defined here: the slot holds this body in 1 vtable(s), all of CTCFireRangedWeaponOnLeavingStateGroup or derived); the Xbox 360 Test PDB virtual order (third source) aligned on 15 TLC-named slots of that vtable puts HasFrameUpdate(void) at this slot; Ego_r/FableWin name set: ?HasFrameUpdate@CTCFireRangedWeaponOnLeavingStateGroup@@UBE_NXZ; `ret` matches the signature.
class CTCFireRangedWeaponOnLeavingStateGroup {
public:
    virtual bool HasFrameUpdate() const;
};
bool CTCFireRangedWeaponOnLeavingStateGroup::HasFrameUpdate() const
{
    return true;
}
