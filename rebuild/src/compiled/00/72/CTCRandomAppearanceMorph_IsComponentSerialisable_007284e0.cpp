// CTCRandomAppearanceMorph::IsComponentSerialisable() const -- returns true
// Leaf: ?IsComponentSerialisable@CTCRandomAppearanceMorph@@UBE_NXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x007284E0, thiscall, `ret`.
// Identity: slot 23 of CTCRandomAppearanceMorph's vtable 0x0126653C (defined here: the slot holds this body in 1 vtable(s), all of CTCRandomAppearanceMorph or derived); the Xbox 360 Test PDB virtual order (third source) aligned on 14 TLC-named slots of that vtable puts IsComponentSerialisable(void) at this slot; Ego_r/FableWin name set: ?IsComponentSerialisable@CTCRandomAppearanceMorph@@UBE_NXZ; `ret` matches the signature.
class CTCRandomAppearanceMorph {
public:
    virtual bool IsComponentSerialisable() const;
};
bool CTCRandomAppearanceMorph::IsComponentSerialisable() const
{
    return true;
}
