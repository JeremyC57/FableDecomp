// CTCHeroRememberCrimes::HasFrameUpdate() const -- returns true
// Leaf: ?HasFrameUpdate@CTCHeroRememberCrimes@@UBE_NXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00756BC0, thiscall, `ret`.
// Identity: slot 8 of CTCHeroRememberCrimes's vtable 0x0126980C (defined here: the slot holds this body in 1 vtable(s), all of CTCHeroRememberCrimes or derived); the Xbox 360 Test PDB virtual order (third source) aligned on 15 TLC-named slots of that vtable puts HasFrameUpdate(void) at this slot; Ego_r/FableWin name set: ?HasFrameUpdate@CTCHeroRememberCrimes@@UBE_NXZ; `ret` matches the signature.
class CTCHeroRememberCrimes {
public:
    virtual bool HasFrameUpdate() const;
};
bool CTCHeroRememberCrimes::HasFrameUpdate() const
{
    return true;
}
