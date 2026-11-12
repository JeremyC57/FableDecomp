// CTCUndeadSoul::HasFrameUpdate() const -- returns true
// Leaf: ?HasFrameUpdate@CTCUndeadSoul@@UBE_NXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x004D2E07, thiscall, `ret`.
// Identity: slot 8 of CTCUndeadSoul's vtable 0x0123A364 (defined here: the slot holds this body in 1 vtable(s), all of CTCUndeadSoul or derived); the Xbox 360 Test PDB virtual order (third source) aligned on 16 TLC-named slots of that vtable puts HasFrameUpdate(void) at this slot; Ego_r/FableWin name set: ?HasFrameUpdate@CTCUndeadSoul@@UBE_NXZ; `ret` matches the signature.
class CTCUndeadSoul {
public:
    virtual bool HasFrameUpdate() const;
};
bool CTCUndeadSoul::HasFrameUpdate() const
{
    return true;
}
