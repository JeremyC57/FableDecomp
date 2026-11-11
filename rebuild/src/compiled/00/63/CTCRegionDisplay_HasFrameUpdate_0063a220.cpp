// CTCRegionDisplay::HasFrameUpdate() const -- returns true
// Leaf: ?HasFrameUpdate@CTCRegionDisplay@@UBE_NXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x0063A220, thiscall, `ret`.
// Identity: slot 8 of CTCRegionDisplay's vtable 0x0125A0EC (defined here: the slot holds this body in 1 vtable(s), all of CTCRegionDisplay or derived); the Xbox 360 Test PDB virtual order (third source) aligned on 15 TLC-named slots of that vtable puts HasFrameUpdate(void) at this slot; Ego_r/FableWin name set: ?HasFrameUpdate@CTCRegionDisplay@@UBE_NXZ; `ret` matches the signature.
class CTCRegionDisplay {
public:
    virtual bool HasFrameUpdate() const;
};
bool CTCRegionDisplay::HasFrameUpdate() const
{
    return true;
}
