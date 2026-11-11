// CTCCreatureTintEffect::HasFrameUpdate() const -- returns true
// Leaf: ?HasFrameUpdate@CTCCreatureTintEffect@@UBE_NXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x007CF380, thiscall, `ret`.
// Identity: slot 8 of CTCCreatureTintEffect's vtable 0x0126F13C (defined here: the slot holds this body in 1 vtable(s), all of CTCCreatureTintEffect or derived); the Xbox 360 Test PDB virtual order (third source) aligned on 14 TLC-named slots of that vtable puts HasFrameUpdate(void) at this slot; Ego_r/FableWin name set: ?HasFrameUpdate@CTCCreatureTintEffect@@UBE_NXZ; `ret` matches the signature.
class CTCCreatureTintEffect {
public:
    virtual bool HasFrameUpdate() const;
};
bool CTCCreatureTintEffect::HasFrameUpdate() const
{
    return true;
}
