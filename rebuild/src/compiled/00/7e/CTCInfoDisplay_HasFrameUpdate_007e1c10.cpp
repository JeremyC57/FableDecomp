// CTCInfoDisplay::HasFrameUpdate() const -- returns true
// Leaf: ?HasFrameUpdate@CTCInfoDisplay@@UBE_NXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x007E1C10, thiscall, `ret`.
// Identity: slot 8 of CTCInfoDisplay's vtable 0x01270044 (defined here: the slot holds this body in 1 vtable(s), all of CTCInfoDisplay or derived); the Xbox 360 Test PDB virtual order (third source) aligned on 16 TLC-named slots of that vtable puts HasFrameUpdate(void) at this slot; Ego_r/FableWin name set: ?HasFrameUpdate@CTCInfoDisplay@@UBE_NXZ; `ret` matches the signature.
class CTCInfoDisplay {
public:
    virtual bool HasFrameUpdate() const;
};
bool CTCInfoDisplay::HasFrameUpdate() const
{
    return true;
}
