// CTCTargetingSpirit::HasFrameUpdate() const -- returns true
// Leaf: ?HasFrameUpdate@CTCTargetingSpirit@@UBE_NXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x004DEC19, thiscall, `ret`.
// Identity: slot 8 of CTCTargetingSpirit's vtable 0x012419A4 (defined here: the slot holds this body in 1 vtable(s), all of CTCTargetingSpirit or derived); the Xbox 360 Test PDB virtual order (third source) aligned on 19 TLC-named slots of that vtable puts HasFrameUpdate(void) at this slot; Ego_r/FableWin name set: ?HasFrameUpdate@CTCTargetingSpirit@@UBE_NXZ; `ret` matches the signature.
class CTCTargetingSpirit {
public:
    virtual bool HasFrameUpdate() const;
};
bool CTCTargetingSpirit::HasFrameUpdate() const
{
    return true;
}
