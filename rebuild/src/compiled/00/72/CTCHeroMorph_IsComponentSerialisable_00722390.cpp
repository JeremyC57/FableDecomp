// CTCHeroMorph::IsComponentSerialisable() const -- returns true
// Leaf: ?IsComponentSerialisable@CTCHeroMorph@@UBE_NXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00722390, thiscall, `ret`.
// Identity: slot 23 of CTCHeroMorph's vtable 0x01265CF4 (defined here: the slot holds this body in 1 vtable(s), all of CTCHeroMorph or derived); the Xbox 360 Test PDB virtual order (third source) aligned on 16 TLC-named slots of that vtable puts IsComponentSerialisable(void) at this slot; Ego_r/FableWin name set: ?IsComponentSerialisable@CTCHeroMorph@@UBE_NXZ; `ret` matches the signature.
class CTCHeroMorph {
public:
    virtual bool IsComponentSerialisable() const;
};
bool CTCHeroMorph::IsComponentSerialisable() const
{
    return true;
}
