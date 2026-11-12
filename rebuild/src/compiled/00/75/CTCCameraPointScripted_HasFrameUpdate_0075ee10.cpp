// CTCCameraPointScripted::HasFrameUpdate() const -- returns false
// Leaf: ?HasFrameUpdate@CTCCameraPointScripted@@UBE_NXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x0075EE10, thiscall, `ret`.
// Identity: slot 8 of CTCCameraPointScripted's vtable 0x01269C84 (defined here: the slot holds this body in 1 vtable(s), all of CTCCameraPointScripted or derived); the Xbox 360 Test PDB virtual order (third source) aligned on 19 TLC-named slots of that vtable puts HasFrameUpdate(void) at this slot; Ego_r/FableWin name set: ?HasFrameUpdate@CTCCameraPointScripted@@UBE_NXZ; `ret` matches the signature.
class CTCCameraPointScripted {
public:
    virtual bool HasFrameUpdate() const;
};
bool CTCCameraPointScripted::HasFrameUpdate() const
{
    return false;
}
