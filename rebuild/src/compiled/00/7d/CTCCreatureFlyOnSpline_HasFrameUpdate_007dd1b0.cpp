// CTCCreatureFlyOnSpline::HasFrameUpdate() const -- returns true
// Leaf: ?HasFrameUpdate@CTCCreatureFlyOnSpline@@UBE_NXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x007DD1B0, thiscall, `ret`.
// Identity: slot 8 of CTCCreatureFlyOnSpline's vtable 0x0126F9F4 (defined here: the slot holds this body in 1 vtable(s), all of CTCCreatureFlyOnSpline or derived); the Xbox 360 Test PDB virtual order (third source) aligned on 16 TLC-named slots of that vtable puts HasFrameUpdate(void) at this slot; Ego_r/FableWin name set: ?HasFrameUpdate@CTCCreatureFlyOnSpline@@UBE_NXZ; `ret` matches the signature.
class CTCCreatureFlyOnSpline {
public:
    virtual bool HasFrameUpdate() const;
};
bool CTCCreatureFlyOnSpline::HasFrameUpdate() const
{
    return true;
}
