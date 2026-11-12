// CTCWaterWader::HasFrameUpdate() const -- returns true
// Leaf: ?HasFrameUpdate@CTCWaterWader@@UBE_NXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x007B62D0, thiscall, `ret`.
// Identity: slot 8 of CTCWaterWader's vtable 0x0126D95C (defined here: the slot holds this body in 1 vtable(s), all of CTCWaterWader or derived); the Xbox 360 Test PDB virtual order (third source) aligned on 16 TLC-named slots of that vtable puts HasFrameUpdate(void) at this slot; Ego_r/FableWin name set: ?HasFrameUpdate@CTCWaterWader@@UBE_NXZ; `ret` matches the signature.
class CTCWaterWader {
public:
    virtual bool HasFrameUpdate() const;
};
bool CTCWaterWader::HasFrameUpdate() const
{
    return true;
}
