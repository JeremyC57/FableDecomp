// CTCCoopSpirit::HasFrameUpdate() const -- returns true
// Leaf: ?HasFrameUpdate@CTCCoopSpirit@@UBE_NXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00670B80, thiscall, `ret`.
// Identity: slot 8 of CTCCoopSpirit's vtable 0x0125B264 (defined here: the slot holds this body in 1 vtable(s), all of CTCCoopSpirit or derived); the Xbox 360 Test PDB virtual order (third source) aligned on 15 TLC-named slots of that vtable puts HasFrameUpdate(void) at this slot; Ego_r/FableWin name set: ?HasFrameUpdate@CTCCoopSpirit@@UBE_NXZ; `ret` matches the signature.
class CTCCoopSpirit {
public:
    virtual bool HasFrameUpdate() const;
};
bool CTCCoopSpirit::HasFrameUpdate() const
{
    return true;
}
