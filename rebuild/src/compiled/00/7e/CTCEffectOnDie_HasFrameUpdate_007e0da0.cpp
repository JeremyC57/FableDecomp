// CTCEffectOnDie::HasFrameUpdate() const -- returns true
// Leaf: ?HasFrameUpdate@CTCEffectOnDie@@UBE_NXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x007E0DA0, thiscall, `ret`.
// Identity: slot 8 of CTCEffectOnDie's vtable 0x0126FEB4 (defined here: the slot holds this body in 1 vtable(s), all of CTCEffectOnDie or derived); the Xbox 360 Test PDB virtual order (third source) aligned on 16 TLC-named slots of that vtable puts HasFrameUpdate(void) at this slot; Ego_r/FableWin name set: ?HasFrameUpdate@CTCEffectOnDie@@UBE_NXZ; `ret` matches the signature.
class CTCEffectOnDie {
public:
    virtual bool HasFrameUpdate() const;
};
bool CTCEffectOnDie::HasFrameUpdate() const
{
    return true;
}
