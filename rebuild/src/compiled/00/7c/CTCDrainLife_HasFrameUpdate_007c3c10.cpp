// CTCDrainLife::HasFrameUpdate() const -- returns true
// Leaf: ?HasFrameUpdate@CTCDrainLife@@UBE_NXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x007C3C10, thiscall, `ret`.
// Identity: slot 8 of CTCDrainLife's vtable 0x0126E284 (defined here: the slot holds this body in 1 vtable(s), all of CTCDrainLife or derived); the Xbox 360 Test PDB virtual order (third source) aligned on 14 TLC-named slots of that vtable puts HasFrameUpdate(void) at this slot; Ego_r/FableWin name set: ?HasFrameUpdate@CTCDrainLife@@UBE_NXZ; `ret` matches the signature.
class CTCDrainLife {
public:
    virtual bool HasFrameUpdate() const;
};
bool CTCDrainLife::HasFrameUpdate() const
{
    return true;
}
