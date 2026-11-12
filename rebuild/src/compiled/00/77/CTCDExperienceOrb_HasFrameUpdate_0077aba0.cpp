// CTCDExperienceOrb::HasFrameUpdate() const -- returns true
// Leaf: ?HasFrameUpdate@CTCDExperienceOrb@@UBE_NXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x0077ABA0, thiscall, `ret`.
// Identity: slot 8 of CTCDExperienceOrb's vtable 0x0126ABFC (defined here: the slot holds this body in 1 vtable(s), all of CTCDExperienceOrb or derived); the Xbox 360 Test PDB virtual order (third source) aligned on 15 TLC-named slots of that vtable puts HasFrameUpdate(void) at this slot; Ego_r/FableWin name set: ?HasFrameUpdate@CTCDExperienceOrb@@UBE_NXZ; `ret` matches the signature.
class CTCDExperienceOrb {
public:
    virtual bool HasFrameUpdate() const;
};
bool CTCDExperienceOrb::HasFrameUpdate() const
{
    return true;
}
