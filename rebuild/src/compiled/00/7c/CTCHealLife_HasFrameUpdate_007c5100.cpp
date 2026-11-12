// CTCHealLife::HasFrameUpdate() const -- returns true
// Leaf: ?HasFrameUpdate@CTCHealLife@@UBE_NXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x007C5100, thiscall, `ret`.
// Identity: slot 8 of CTCHealLife's vtable 0x0126E36C (defined here: the slot holds this body in 1 vtable(s), all of CTCHealLife or derived); the Xbox 360 Test PDB virtual order (third source) aligned on 14 TLC-named slots of that vtable puts HasFrameUpdate(void) at this slot; Ego_r/FableWin name set: ?HasFrameUpdate@CTCHealLife@@UBE_NXZ; `ret` matches the signature.
class CTCHealLife {
public:
    virtual bool HasFrameUpdate() const;
};
bool CTCHealLife::HasFrameUpdate() const
{
    return true;
}
