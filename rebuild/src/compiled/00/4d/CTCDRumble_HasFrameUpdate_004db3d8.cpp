// CTCDRumble::HasFrameUpdate() const -- returns true
// Leaf: ?HasFrameUpdate@CTCDRumble@@UBE_NXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x004DB3D8, thiscall, `ret`.
// Identity: slot 8 of CTCDRumble's vtable 0x012408BC (defined here: the slot holds this body in 1 vtable(s), all of CTCDRumble or derived); the Xbox 360 Test PDB virtual order (third source) aligned on 16 TLC-named slots of that vtable puts HasFrameUpdate(void) at this slot; Ego_r/FableWin name set: ?HasFrameUpdate@CTCDRumble@@UBE_NXZ; `ret` matches the signature.
class CTCDRumble {
public:
    virtual bool HasFrameUpdate() const;
};
bool CTCDRumble::HasFrameUpdate() const
{
    return true;
}
