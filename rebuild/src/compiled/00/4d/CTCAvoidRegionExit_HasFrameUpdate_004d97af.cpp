// CTCAvoidRegionExit::HasFrameUpdate() const -- returns true
// Leaf: ?HasFrameUpdate@CTCAvoidRegionExit@@UBE_NXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x004D97AF, thiscall, `ret`.
// Identity: slot 8 of CTCAvoidRegionExit's vtable 0x0123EFAC (defined here: the slot holds this body in 1 vtable(s), all of CTCAvoidRegionExit or derived); the Xbox 360 Test PDB virtual order (third source) aligned on 15 TLC-named slots of that vtable puts HasFrameUpdate(void) at this slot; Ego_r/FableWin name set: ?HasFrameUpdate@CTCAvoidRegionExit@@UBE_NXZ; `ret` matches the signature.
class CTCAvoidRegionExit {
public:
    virtual bool HasFrameUpdate() const;
};
bool CTCAvoidRegionExit::HasFrameUpdate() const
{
    return true;
}
