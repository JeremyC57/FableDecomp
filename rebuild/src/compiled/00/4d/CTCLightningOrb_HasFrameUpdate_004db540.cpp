// CTCLightningOrb::HasFrameUpdate() const -- returns true
// Leaf: ?HasFrameUpdate@CTCLightningOrb@@UBE_NXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x004DB540, thiscall, `ret`.
// Identity: slot 8 of CTCLightningOrb's vtable 0x01240A2C (defined here: the slot holds this body in 1 vtable(s), all of CTCLightningOrb or derived); the Xbox 360 Test PDB virtual order (third source) aligned on 15 TLC-named slots of that vtable puts HasFrameUpdate(void) at this slot; Ego_r/FableWin name set: ?HasFrameUpdate@CTCLightningOrb@@UBE_NXZ; `ret` matches the signature.
class CTCLightningOrb {
public:
    virtual bool HasFrameUpdate() const;
};
bool CTCLightningOrb::HasFrameUpdate() const
{
    return true;
}
