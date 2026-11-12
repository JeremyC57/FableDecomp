// CTCFineDialogue::HasFrameUpdate() const -- returns true
// Leaf: ?HasFrameUpdate@CTCFineDialogue@@UBE_NXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x007E2D60, thiscall, `ret`.
// Identity: slot 8 of CTCFineDialogue's vtable 0x01270274 (defined here: the slot holds this body in 1 vtable(s), all of CTCFineDialogue or derived); the Xbox 360 Test PDB virtual order (third source) aligned on 15 TLC-named slots of that vtable puts HasFrameUpdate(void) at this slot; Ego_r/FableWin name set: ?HasFrameUpdate@CTCFineDialogue@@UBE_NXZ; `ret` matches the signature.
class CTCFineDialogue {
public:
    virtual bool HasFrameUpdate() const;
};
bool CTCFineDialogue::HasFrameUpdate() const
{
    return true;
}
