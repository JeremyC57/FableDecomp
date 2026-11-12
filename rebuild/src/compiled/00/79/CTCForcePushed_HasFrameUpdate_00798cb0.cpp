// CTCForcePushed::HasFrameUpdate() const -- returns true
// Leaf: ?HasFrameUpdate@CTCForcePushed@@UBE_NXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00798CB0, thiscall, `ret`.
// Identity: slot 8 of CTCForcePushed's vtable 0x0126C09C (defined here: the slot holds this body in 1 vtable(s), all of CTCForcePushed or derived); the Xbox 360 Test PDB virtual order (third source) aligned on 16 TLC-named slots of that vtable puts HasFrameUpdate(void) at this slot; Ego_r/FableWin name set: ?HasFrameUpdate@CTCForcePushed@@UBE_NXZ; `ret` matches the signature.
class CTCForcePushed {
public:
    virtual bool HasFrameUpdate() const;
};
bool CTCForcePushed::HasFrameUpdate() const
{
    return true;
}
