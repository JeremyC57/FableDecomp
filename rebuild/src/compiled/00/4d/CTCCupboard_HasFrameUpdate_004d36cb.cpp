// CTCCupboard::HasFrameUpdate() const -- returns true
// Leaf: ?HasFrameUpdate@CTCCupboard@@UBE_NXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x004D36CB, thiscall, `ret`.
// Identity: slot 8 of CTCCupboard's vtable 0x0123AB9C (defined here: the slot holds this body in 1 vtable(s), all of CTCCupboard or derived); the Xbox 360 Test PDB virtual order (third source) aligned on 15 TLC-named slots of that vtable puts HasFrameUpdate(void) at this slot; Ego_r/FableWin name set: ?HasFrameUpdate@CTCCupboard@@UBE_NXZ; `ret` matches the signature.
class CTCCupboard {
public:
    virtual bool HasFrameUpdate() const;
};
bool CTCCupboard::HasFrameUpdate() const
{
    return true;
}
