// CTCActionUseBed::IsComponentSerialisable() const -- returns true
// Leaf: ?IsComponentSerialisable@CTCActionUseBed@@UBE_NXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x004DAC17, thiscall, `ret`.
// Identity: slot 23 of CTCActionUseBed's vtable 0x0123F784 (defined here: the slot holds this body in 1 vtable(s), all of CTCActionUseBed or derived); the Xbox 360 Test PDB virtual order (third source) aligned on 15 TLC-named slots of that vtable puts IsComponentSerialisable(void) at this slot; Ego_r/FableWin name set: ?IsComponentSerialisable@CTCActionUseBed@@UBE_NXZ; `ret` matches the signature.
class CTCActionUseBed {
public:
    virtual bool IsComponentSerialisable() const;
};
bool CTCActionUseBed::IsComponentSerialisable() const
{
    return true;
}
