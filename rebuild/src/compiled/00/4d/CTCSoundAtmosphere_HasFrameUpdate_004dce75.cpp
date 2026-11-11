// CTCSoundAtmosphere::HasFrameUpdate() const -- returns true
// Leaf: ?HasFrameUpdate@CTCSoundAtmosphere@@UBE_NXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x004DCE75, thiscall, `ret`.
// Identity: slot 8 of CTCSoundAtmosphere's vtable 0x01271DFC (defined here: the slot holds this body in 2 vtable(s), all of CTCSoundAtmosphere or derived); the Xbox 360 Test PDB virtual order (third source) aligned on 15 TLC-named slots of that vtable puts HasFrameUpdate(void) at this slot; Ego_r/FableWin name set: ?HasFrameUpdate@CTCSoundAtmosphere@@UBE_NXZ; `ret` matches the signature.
class CTCSoundAtmosphere {
public:
    virtual bool HasFrameUpdate() const;
};
bool CTCSoundAtmosphere::HasFrameUpdate() const
{
    return true;
}
