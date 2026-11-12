// CTCElectrocutedEffect::HasFrameUpdate() const -- returns true
// Leaf: ?HasFrameUpdate@CTCElectrocutedEffect@@UBE_NXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x007CED30, thiscall, `ret`.
// Identity: slot 8 of CTCElectrocutedEffect's vtable 0x0126F044 (defined here: the slot holds this body in 1 vtable(s), all of CTCElectrocutedEffect or derived); the Xbox 360 Test PDB virtual order (third source) aligned on 16 TLC-named slots of that vtable puts HasFrameUpdate(void) at this slot; Ego_r/FableWin name set: ?HasFrameUpdate@CTCElectrocutedEffect@@UBE_NXZ; `ret` matches the signature.
class CTCElectrocutedEffect {
public:
    virtual bool HasFrameUpdate() const;
};
bool CTCElectrocutedEffect::HasFrameUpdate() const
{
    return true;
}
