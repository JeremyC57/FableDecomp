// CTCTrophy::HasFrameUpdate() const -- returns true
// Leaf: ?HasFrameUpdate@CTCTrophy@@UBE_NXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x006DFA40, thiscall, `ret`.
// Identity: slot 8 of CTCTrophy's vtable 0x0125FC54 (defined here: the slot holds this body in 1 vtable(s), all of CTCTrophy or derived); the Xbox 360 Test PDB virtual order (third source) aligned on 16 TLC-named slots of that vtable puts HasFrameUpdate(void) at this slot; Ego_r/FableWin name set: ?HasFrameUpdate@CTCTrophy@@UBE_NXZ; `ret` matches the signature.
class CTCTrophy {
public:
    virtual bool HasFrameUpdate() const;
};
bool CTCTrophy::HasFrameUpdate() const
{
    return true;
}
