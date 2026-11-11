// CTCOverheadDisplay::HasFrameUpdate() const -- returns true
// Leaf: ?HasFrameUpdate@CTCOverheadDisplay@@UBE_NXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x007B8080, thiscall, `ret`.
// Identity: slot 8 of CTCOverheadDisplay's vtable 0x0126DA94 (defined here: the slot holds this body in 1 vtable(s), all of CTCOverheadDisplay or derived); the Xbox 360 Test PDB virtual order (third source) aligned on 15 TLC-named slots of that vtable puts HasFrameUpdate(void) at this slot; Ego_r/FableWin name set: ?HasFrameUpdate@CTCOverheadDisplay@@UBE_NXZ; `ret` matches the signature.
class CTCOverheadDisplay {
public:
    virtual bool HasFrameUpdate() const;
};
bool CTCOverheadDisplay::HasFrameUpdate() const
{
    return true;
}
