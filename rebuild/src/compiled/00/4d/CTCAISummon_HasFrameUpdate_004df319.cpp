// CTCAISummon::HasFrameUpdate() const -- returns true
// Leaf: ?HasFrameUpdate@CTCAISummon@@UBE_NXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x004DF319, thiscall, `ret`.
// Identity: slot 8 of CTCAISummon's vtable 0x01241C8C (defined here: the slot holds this body in 1 vtable(s), all of CTCAISummon or derived); the Xbox 360 Test PDB virtual order (third source) aligned on 16 TLC-named slots of that vtable puts HasFrameUpdate(void) at this slot; Ego_r/FableWin name set: ?HasFrameUpdate@CTCAISummon@@UBE_NXZ; `ret` matches the signature.
class CTCAISummon {
public:
    virtual bool HasFrameUpdate() const;
};
bool CTCAISummon::HasFrameUpdate() const
{
    return true;
}
