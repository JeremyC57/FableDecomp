// CTCExplodingObject::HasFrameUpdate() const -- returns true
// Leaf: ?HasFrameUpdate@CTCExplodingObject@@UBE_NXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00751290, thiscall, `ret`.
// Identity: slot 8 of CTCExplodingObject's vtable 0x01269674 (defined here: the slot holds this body in 1 vtable(s), all of CTCExplodingObject or derived); the Xbox 360 Test PDB virtual order (third source) aligned on 17 TLC-named slots of that vtable puts HasFrameUpdate(void) at this slot; Ego_r/FableWin name set: ?HasFrameUpdate@CTCExplodingObject@@UBE_NXZ; `ret` matches the signature.
class CTCExplodingObject {
public:
    virtual bool HasFrameUpdate() const;
};
bool CTCExplodingObject::HasFrameUpdate() const
{
    return true;
}
