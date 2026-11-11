// CTCCameraPointScriptedSpline::HasFrameUpdate() const -- returns false
// Leaf: ?HasFrameUpdate@CTCCameraPointScriptedSpline@@UBE_NXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x0075FC00, thiscall, `ret`.
// Identity: slot 8 of CTCCameraPointScriptedSpline's vtable 0x0126A0BC (defined here: the slot holds this body in 1 vtable(s), all of CTCCameraPointScriptedSpline or derived); the Xbox 360 Test PDB virtual order (third source) aligned on 18 TLC-named slots of that vtable puts HasFrameUpdate(void) at this slot; Ego_r/FableWin name set: ?HasFrameUpdate@CTCCameraPointScriptedSpline@@UBE_NXZ; `ret` matches the signature.
class CTCCameraPointScriptedSpline {
public:
    virtual bool HasFrameUpdate() const;
};
bool CTCCameraPointScriptedSpline::HasFrameUpdate() const
{
    return false;
}
