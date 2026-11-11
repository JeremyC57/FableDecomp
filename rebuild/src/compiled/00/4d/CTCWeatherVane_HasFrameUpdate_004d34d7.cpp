// CTCWeatherVane::HasFrameUpdate() const -- returns true
// Leaf: ?HasFrameUpdate@CTCWeatherVane@@UBE_NXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x004D34D7, thiscall, `ret`.
// Identity: slot 8 of CTCWeatherVane's vtable 0x0123A984 (defined here: the slot holds this body in 1 vtable(s), all of CTCWeatherVane or derived); the Xbox 360 Test PDB virtual order (third source) aligned on 15 TLC-named slots of that vtable puts HasFrameUpdate(void) at this slot; Ego_r/FableWin name set: ?HasFrameUpdate@CTCWeatherVane@@UBE_NXZ; `ret` matches the signature.
class CTCWeatherVane {
public:
    virtual bool HasFrameUpdate() const;
};
bool CTCWeatherVane::HasFrameUpdate() const
{
    return true;
}
