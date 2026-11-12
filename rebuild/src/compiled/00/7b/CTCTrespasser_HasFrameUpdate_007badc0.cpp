// CTCTrespasser::HasFrameUpdate() const -- returns true
// Leaf: ?HasFrameUpdate@CTCTrespasser@@UBE_NXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x007BADC0, thiscall, `ret`.
// Identity: slot 8 of CTCTrespasser's vtable 0x0126DE14 (defined here: the slot holds this body in 1 vtable(s), all of CTCTrespasser or derived); the Xbox 360 Test PDB virtual order (third source) aligned on 17 TLC-named slots of that vtable puts HasFrameUpdate(void) at this slot; Ego_r/FableWin name set: ?HasFrameUpdate@CTCTrespasser@@UBE_NXZ; `ret` matches the signature.
class CTCTrespasser {
public:
    virtual bool HasFrameUpdate() const;
};
bool CTCTrespasser::HasFrameUpdate() const
{
    return true;
}
