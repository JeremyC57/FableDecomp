// CTCBurntEffect::HasFrameUpdate() const -- returns true
// Leaf: ?HasFrameUpdate@CTCBurntEffect@@UBE_NXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x007CE1A0, thiscall, `ret`.
// Identity: slot 8 of CTCBurntEffect's vtable 0x0126EFBC (defined here: the slot holds this body in 1 vtable(s), all of CTCBurntEffect or derived); the Xbox 360 Test PDB virtual order (third source) aligned on 14 TLC-named slots of that vtable puts HasFrameUpdate(void) at this slot; Ego_r/FableWin name set: ?HasFrameUpdate@CTCBurntEffect@@UBE_NXZ; `ret` matches the signature.
class CTCBurntEffect {
public:
    virtual bool HasFrameUpdate() const;
};
bool CTCBurntEffect::HasFrameUpdate() const
{
    return true;
}
