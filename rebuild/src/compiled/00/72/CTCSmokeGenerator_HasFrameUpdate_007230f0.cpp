// CTCSmokeGenerator::HasFrameUpdate() const -- returns true
// Leaf: ?HasFrameUpdate@CTCSmokeGenerator@@UBE_NXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x007230F0, thiscall, `ret`.
// Identity: slot 8 of CTCSmokeGenerator's vtable 0x01265D74 (defined here: the slot holds this body in 1 vtable(s), all of CTCSmokeGenerator or derived); the Xbox 360 Test PDB virtual order (third source) aligned on 16 TLC-named slots of that vtable puts HasFrameUpdate(void) at this slot; Ego_r/FableWin name set: ?HasFrameUpdate@CTCSmokeGenerator@@UBE_NXZ; `ret` matches the signature.
class CTCSmokeGenerator {
public:
    virtual bool HasFrameUpdate() const;
};
bool CTCSmokeGenerator::HasFrameUpdate() const
{
    return true;
}
