// CTCCreatureFlyInCircles::HasFrameUpdate() const -- returns true
// Leaf: ?HasFrameUpdate@CTCCreatureFlyInCircles@@UBE_NXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x007DE410, thiscall, `ret`.
// Identity: slot 8 of CTCCreatureFlyInCircles's vtable 0x0126FB04 (defined here: the slot holds this body in 1 vtable(s), all of CTCCreatureFlyInCircles or derived); the Xbox 360 Test PDB virtual order (third source) aligned on 16 TLC-named slots of that vtable puts HasFrameUpdate(void) at this slot; Ego_r/FableWin name set: ?HasFrameUpdate@CTCCreatureFlyInCircles@@UBE_NXZ; `ret` matches the signature.
class CTCCreatureFlyInCircles {
public:
    virtual bool HasFrameUpdate() const;
};
bool CTCCreatureFlyInCircles::HasFrameUpdate() const
{
    return true;
}
