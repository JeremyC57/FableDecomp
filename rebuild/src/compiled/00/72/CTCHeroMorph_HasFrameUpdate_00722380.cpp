// CTCHeroMorph::HasFrameUpdate() const -- returns true
// Leaf: ?HasFrameUpdate@CTCHeroMorph@@UBE_NXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00722380, thiscall, `ret`.
// Identity: slot 8 of CTCHeroMorph's vtable 0x01265CF4 (defined here: the slot holds this body in 1 vtable(s), all of CTCHeroMorph or derived); the Xbox 360 Test PDB virtual order (third source) aligned on 16 TLC-named slots of that vtable puts HasFrameUpdate(void) at this slot; Ego_r/FableWin name set: ?HasFrameUpdate@CTCHeroMorph@@UBE_NXZ; `ret` matches the signature.
class CTCHeroMorph {
public:
    virtual bool HasFrameUpdate() const;
};
bool CTCHeroMorph::HasFrameUpdate() const
{
    return true;
}
