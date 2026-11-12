// CTCFadeOutAndIn::HasFrameUpdate() const -- returns true
// Leaf: ?HasFrameUpdate@CTCFadeOutAndIn@@UBE_NXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x006C4420, thiscall, `ret`.
// Identity: slot 8 of CTCFadeOutAndIn's vtable 0x0125DA0C (defined here: the slot holds this body in 1 vtable(s), all of CTCFadeOutAndIn or derived); the Xbox 360 Test PDB virtual order (third source) aligned on 15 TLC-named slots of that vtable puts HasFrameUpdate(void) at this slot; Ego_r/FableWin name set: ?HasFrameUpdate@CTCFadeOutAndIn@@UBE_NXZ; `ret` matches the signature.
class CTCFadeOutAndIn {
public:
    virtual bool HasFrameUpdate() const;
};
bool CTCFadeOutAndIn::HasFrameUpdate() const
{
    return true;
}
