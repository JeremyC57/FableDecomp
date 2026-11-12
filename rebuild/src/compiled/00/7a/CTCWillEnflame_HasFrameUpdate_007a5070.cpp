// CTCWillEnflame::HasFrameUpdate() const -- returns true
// Leaf: ?HasFrameUpdate@CTCWillEnflame@@UBE_NXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x007A5070, thiscall, `ret`.
// Identity: slot 8 of CTCWillEnflame's vtable 0x0126C824 (defined here: the slot holds this body in 1 vtable(s), all of CTCWillEnflame or derived); the Xbox 360 Test PDB virtual order (third source) aligned on 13 TLC-named slots of that vtable puts HasFrameUpdate(void) at this slot; Ego_r/FableWin name set: ?HasFrameUpdate@CTCWillEnflame@@UBE_NXZ; `ret` matches the signature.
class CTCWillEnflame {
public:
    virtual bool HasFrameUpdate() const;
};
bool CTCWillEnflame::HasFrameUpdate() const
{
    return true;
}
