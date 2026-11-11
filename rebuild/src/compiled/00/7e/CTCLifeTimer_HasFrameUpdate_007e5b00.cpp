// CTCLifeTimer::HasFrameUpdate() const -- returns true
// Leaf: ?HasFrameUpdate@CTCLifeTimer@@UBE_NXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x007E5B00, thiscall, `ret`.
// Identity: slot 8 of CTCLifeTimer's vtable 0x012706A4 (defined here: the slot holds this body in 1 vtable(s), all of CTCLifeTimer or derived); the Xbox 360 Test PDB virtual order (third source) aligned on 16 TLC-named slots of that vtable puts HasFrameUpdate(void) at this slot; Ego_r/FableWin name set: ?HasFrameUpdate@CTCLifeTimer@@UBE_NXZ; `ret` matches the signature.
class CTCLifeTimer {
public:
    virtual bool HasFrameUpdate() const;
};
bool CTCLifeTimer::HasFrameUpdate() const
{
    return true;
}
