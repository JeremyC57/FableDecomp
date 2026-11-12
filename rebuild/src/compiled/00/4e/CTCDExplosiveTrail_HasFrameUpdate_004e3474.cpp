// CTCDExplosiveTrail::HasFrameUpdate() const -- returns true
// Leaf: ?HasFrameUpdate@CTCDExplosiveTrail@@UBE_NXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x004E3474, thiscall, `ret`.
// Identity: slot 8 of CTCDExplosiveTrail's vtable 0x01242B4C (defined here: the slot holds this body in 1 vtable(s), all of CTCDExplosiveTrail or derived); the Xbox 360 Test PDB virtual order (third source) aligned on 16 TLC-named slots of that vtable puts HasFrameUpdate(void) at this slot; Ego_r/FableWin name set: ?HasFrameUpdate@CTCDExplosiveTrail@@UBE_NXZ; `ret` matches the signature.
class CTCDExplosiveTrail {
public:
    virtual bool HasFrameUpdate() const;
};
bool CTCDExplosiveTrail::HasFrameUpdate() const
{
    return true;
}
