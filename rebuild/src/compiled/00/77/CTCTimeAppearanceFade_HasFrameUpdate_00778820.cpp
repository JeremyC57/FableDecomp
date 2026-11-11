// CTCTimeAppearanceFade::HasFrameUpdate() const -- returns true
// Leaf: ?HasFrameUpdate@CTCTimeAppearanceFade@@UBE_NXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00778820, thiscall, `ret`.
// Identity: slot 8 of CTCTimeAppearanceFade's vtable 0x0126AA64 (defined here: the slot holds this body in 1 vtable(s), all of CTCTimeAppearanceFade or derived); the Xbox 360 Test PDB virtual order (third source) aligned on 16 TLC-named slots of that vtable puts HasFrameUpdate(void) at this slot; Ego_r/FableWin name set: ?HasFrameUpdate@CTCTimeAppearanceFade@@UBE_NXZ; `ret` matches the signature.
class CTCTimeAppearanceFade {
public:
    virtual bool HasFrameUpdate() const;
};
bool CTCTimeAppearanceFade::HasFrameUpdate() const
{
    return true;
}
