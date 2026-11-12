// CTCWobble::HasFrameUpdate() const -- returns true
// Leaf: ?HasFrameUpdate@CTCWobble@@UBE_NXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x004D6206, thiscall, `ret`.
// Identity: slot 8 of CTCWobble's vtable 0x0123DF34 (defined here: the slot holds this body in 1 vtable(s), all of CTCWobble or derived); the Xbox 360 Test PDB virtual order (third source) aligned on 16 TLC-named slots of that vtable puts HasFrameUpdate(void) at this slot; Ego_r/FableWin name set: ?HasFrameUpdate@CTCWobble@@UBE_NXZ; `ret` matches the signature.
class CTCWobble {
public:
    virtual bool HasFrameUpdate() const;
};
bool CTCWobble::HasFrameUpdate() const
{
    return true;
}
