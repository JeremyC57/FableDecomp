// CTCAICreatureWillPowerIndicator::HasFrameUpdate() const -- returns true
// Leaf: ?HasFrameUpdate@CTCAICreatureWillPowerIndicator@@UBE_NXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x007E1170, thiscall, `ret`.
// Identity: slot 8 of CTCAICreatureWillPowerIndicator's vtable 0x0126FF2C (defined here: the slot holds this body in 1 vtable(s), all of CTCAICreatureWillPowerIndicator or derived); the Xbox 360 Test PDB virtual order (third source) aligned on 16 TLC-named slots of that vtable puts HasFrameUpdate(void) at this slot; Ego_r/FableWin name set: ?HasFrameUpdate@CTCAICreatureWillPowerIndicator@@UBE_NXZ; `ret` matches the signature.
class CTCAICreatureWillPowerIndicator {
public:
    virtual bool HasFrameUpdate() const;
};
bool CTCAICreatureWillPowerIndicator::HasFrameUpdate() const
{
    return true;
}
