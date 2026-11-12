// CTCInfoDisplay::FrameUpdate() -- does nothing
// Leaf: ?FrameUpdate@CTCInfoDisplay@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x007E1B00, thiscall, `ret`.
// Identity: slot 9 of CTCInfoDisplay's vtable 0x01270044 (defined here: the slot holds this body in 1 vtable(s), all of CTCInfoDisplay or derived); the Xbox 360 Test PDB virtual order (third source) aligned on 16 TLC-named slots of that vtable puts FrameUpdate(void) at this slot; Ego_r/FableWin name set: ?FrameUpdate@CTCInfoDisplay@@UAEXXZ; `ret` matches the signature.
class CTCInfoDisplay {
public:
    virtual void FrameUpdate();
};
void CTCInfoDisplay::FrameUpdate()
{
}
