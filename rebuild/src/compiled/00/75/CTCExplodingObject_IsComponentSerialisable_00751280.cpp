// CTCExplodingObject::IsComponentSerialisable() const -- returns true
// Leaf: ?IsComponentSerialisable@CTCExplodingObject@@UBE_NXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00751280, thiscall, `ret`.
// Identity: slot 23 of CTCExplodingObject's vtable 0x01269674 (defined here: the slot holds this body in 1 vtable(s), all of CTCExplodingObject or derived); the Xbox 360 Test PDB virtual order (third source) aligned on 17 TLC-named slots of that vtable puts IsComponentSerialisable(void) at this slot; Ego_r/FableWin name set: ?IsComponentSerialisable@CTCExplodingObject@@UBE_NXZ; `ret` matches the signature.
class CTCExplodingObject {
public:
    virtual bool IsComponentSerialisable() const;
};
bool CTCExplodingObject::IsComponentSerialisable() const
{
    return true;
}
