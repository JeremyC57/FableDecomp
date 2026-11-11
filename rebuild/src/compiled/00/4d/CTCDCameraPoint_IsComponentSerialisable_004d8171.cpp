// CTCDCameraPoint::IsComponentSerialisable() const -- returns true
// Leaf: ?IsComponentSerialisable@CTCDCameraPoint@@UBE_NXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x004D8171, thiscall, `ret`.
// Identity: slot 23 of CTCDCameraPoint's vtable 0x0123EC3C (defined here: the slot holds this body in 1 vtable(s), all of CTCDCameraPoint or derived); the Xbox 360 Test PDB virtual order (third source) aligned on 15 TLC-named slots of that vtable puts IsComponentSerialisable(void) at this slot; Ego_r/FableWin name set: ?IsComponentSerialisable@CTCDCameraPoint@@UBE_NXZ; `ret` matches the signature.
class CTCDCameraPoint {
public:
    virtual bool IsComponentSerialisable() const;
};
bool CTCDCameraPoint::IsComponentSerialisable() const
{
    return true;
}
