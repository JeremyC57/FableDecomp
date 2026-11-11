// CTCDrunkenness::HasFrameUpdate() const -- returns true
// Leaf: ?HasFrameUpdate@CTCDrunkenness@@UBE_NXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x007BBA10, thiscall, `ret`.
// Identity: slot 8 of CTCDrunkenness's vtable 0x0126DEB4 (defined here: the slot holds this body in 1 vtable(s), all of CTCDrunkenness or derived); the Xbox 360 Test PDB virtual order (third source) aligned on 15 TLC-named slots of that vtable puts HasFrameUpdate(void) at this slot; Ego_r/FableWin name set: ?HasFrameUpdate@CTCDrunkenness@@UBE_NXZ; `ret` matches the signature.
class CTCDrunkenness {
public:
    virtual bool HasFrameUpdate() const;
};
bool CTCDrunkenness::HasFrameUpdate() const
{
    return true;
}
