// CTCHeroRememberCrimes::IsComponentSerialisable() const -- returns true
// Leaf: ?IsComponentSerialisable@CTCHeroRememberCrimes@@UBE_NXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00756BD0, thiscall, `ret`.
// Identity: slot 23 of CTCHeroRememberCrimes's vtable 0x0126980C (defined here: the slot holds this body in 1 vtable(s), all of CTCHeroRememberCrimes or derived); the Xbox 360 Test PDB virtual order (third source) aligned on 15 TLC-named slots of that vtable puts IsComponentSerialisable(void) at this slot; Ego_r/FableWin name set: ?IsComponentSerialisable@CTCHeroRememberCrimes@@UBE_NXZ; `ret` matches the signature.
class CTCHeroRememberCrimes {
public:
    virtual bool IsComponentSerialisable() const;
};
bool CTCHeroRememberCrimes::IsComponentSerialisable() const
{
    return true;
}
