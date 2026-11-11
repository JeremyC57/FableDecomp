// CTCRotate::HasFrameUpdate() const -- returns true
// Leaf: ?HasFrameUpdate@CTCRotate@@UBE_NXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x004D2B77, thiscall, `ret`.
// Identity: slot 8 of CTCRotate's vtable 0x0123A024 (defined here: the slot holds this body in 1 vtable(s), all of CTCRotate or derived); the Xbox 360 Test PDB virtual order (third source) aligned on 16 TLC-named slots of that vtable puts HasFrameUpdate(void) at this slot; Ego_r/FableWin name set: ?HasFrameUpdate@CTCRotate@@UBE_NXZ; `ret` matches the signature.
class CTCRotate {
public:
    virtual bool HasFrameUpdate() const;
};
bool CTCRotate::HasFrameUpdate() const
{
    return true;
}
