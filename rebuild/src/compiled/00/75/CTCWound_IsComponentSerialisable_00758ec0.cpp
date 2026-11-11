// CTCWound::IsComponentSerialisable() const -- returns true
// Leaf: ?IsComponentSerialisable@CTCWound@@UBE_NXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00758EC0, thiscall, `ret`.
// Identity: slot 23 of CTCWound's vtable 0x012698B4 (defined here: the slot holds this body in 1 vtable(s), all of CTCWound or derived); the Xbox 360 Test PDB virtual order (third source) aligned on 17 TLC-named slots of that vtable puts IsComponentSerialisable(void) at this slot; Ego_r/FableWin name set: ?IsComponentSerialisable@CTCWound@@UBE_NXZ; `ret` matches the signature.
class CTCWound {
public:
    virtual bool IsComponentSerialisable() const;
};
bool CTCWound::IsComponentSerialisable() const
{
    return true;
}
