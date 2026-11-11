// CTCFishing::HasFrameUpdate() const -- returns true
// Leaf: ?HasFrameUpdate@CTCFishing@@UBE_NXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x007B26F0, thiscall, `ret`.
// Identity: slot 8 of CTCFishing's vtable 0x0126D5CC (defined here: the slot holds this body in 1 vtable(s), all of CTCFishing or derived); the Xbox 360 Test PDB virtual order (third source) aligned on 16 TLC-named slots of that vtable puts HasFrameUpdate(void) at this slot; Ego_r/FableWin name set: ?HasFrameUpdate@CTCFishing@@UBE_NXZ; `ret` matches the signature.
class CTCFishing {
public:
    virtual bool HasFrameUpdate() const;
};
bool CTCFishing::HasFrameUpdate() const
{
    return true;
}
