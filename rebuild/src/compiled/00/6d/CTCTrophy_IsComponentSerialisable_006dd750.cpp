// CTCTrophy::IsComponentSerialisable() const -- returns true
// Leaf: ?IsComponentSerialisable@CTCTrophy@@UBE_NXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x006DD750, thiscall, `ret`.
// Identity: slot 23 of CTCTrophy's vtable 0x0125FC54 (defined here: the slot holds this body in 1 vtable(s), all of CTCTrophy or derived); the Xbox 360 Test PDB virtual order (third source) aligned on 16 TLC-named slots of that vtable puts IsComponentSerialisable(void) at this slot; Ego_r/FableWin name set: ?IsComponentSerialisable@CTCTrophy@@UBE_NXZ; `ret` matches the signature.
class CTCTrophy {
public:
    virtual bool IsComponentSerialisable() const;
};
bool CTCTrophy::IsComponentSerialisable() const
{
    return true;
}
