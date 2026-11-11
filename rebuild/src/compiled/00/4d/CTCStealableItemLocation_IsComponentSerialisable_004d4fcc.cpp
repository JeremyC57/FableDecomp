// CTCStealableItemLocation::IsComponentSerialisable() const -- returns true
// Leaf: ?IsComponentSerialisable@CTCStealableItemLocation@@UBE_NXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x004D4FCC, thiscall, `ret`.
// Identity: slot 23 of CTCStealableItemLocation's vtable 0x0123C65C (defined here: the slot holds this body in 1 vtable(s), all of CTCStealableItemLocation or derived); the Xbox 360 Test PDB virtual order (third source) aligned on 16 TLC-named slots of that vtable puts IsComponentSerialisable(void) at this slot; Ego_r/FableWin name set: ?IsComponentSerialisable@CTCStealableItemLocation@@UBE_NXZ; `ret` matches the signature.
class CTCStealableItemLocation {
public:
    virtual bool IsComponentSerialisable() const;
};
bool CTCStealableItemLocation::IsComponentSerialisable() const
{
    return true;
}
