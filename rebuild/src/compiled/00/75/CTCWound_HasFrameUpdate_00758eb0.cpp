// CTCWound::HasFrameUpdate() const -- returns true
// Leaf: ?HasFrameUpdate@CTCWound@@UBE_NXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00758EB0, thiscall, `ret`.
// Identity: slot 8 of CTCWound's vtable 0x012698B4 (defined here: the slot holds this body in 1 vtable(s), all of CTCWound or derived); the Xbox 360 Test PDB virtual order (third source) aligned on 17 TLC-named slots of that vtable puts HasFrameUpdate(void) at this slot; Ego_r/FableWin name set: ?HasFrameUpdate@CTCWound@@UBE_NXZ; `ret` matches the signature.
class CTCWound {
public:
    virtual bool HasFrameUpdate() const;
};
bool CTCWound::HasFrameUpdate() const
{
    return true;
}
