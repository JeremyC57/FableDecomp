// CTCRockTrollShield::HasFrameUpdate() const -- returns true
// Leaf: ?HasFrameUpdate@CTCRockTrollShield@@UBE_NXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x007AF860, thiscall, `ret`.
// Identity: slot 8 of CTCRockTrollShield's vtable 0x0126D41C (defined here: the slot holds this body in 1 vtable(s), all of CTCRockTrollShield or derived); the Xbox 360 Test PDB virtual order (third source) aligned on 15 TLC-named slots of that vtable puts HasFrameUpdate(void) at this slot; Ego_r/FableWin name set: ?HasFrameUpdate@CTCRockTrollShield@@UBE_NXZ; `ret` matches the signature.
class CTCRockTrollShield {
public:
    virtual bool HasFrameUpdate() const;
};
bool CTCRockTrollShield::HasFrameUpdate() const
{
    return true;
}
