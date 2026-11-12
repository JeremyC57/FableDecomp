// CTCBoastingArea::HasFrameUpdate() const -- returns true
// Leaf: ?HasFrameUpdate@CTCBoastingArea@@UBE_NXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00782B00, thiscall, `ret`.
// Identity: slot 8 of CTCBoastingArea's vtable 0x0126B314 (defined here: the slot holds this body in 1 vtable(s), all of CTCBoastingArea or derived); the Xbox 360 Test PDB virtual order (third source) aligned on 16 TLC-named slots of that vtable puts HasFrameUpdate(void) at this slot; Ego_r/FableWin name set: ?HasFrameUpdate@CTCBoastingArea@@UBE_NXZ; `ret` matches the signature.
class CTCBoastingArea {
public:
    virtual bool HasFrameUpdate() const;
};
bool CTCBoastingArea::HasFrameUpdate() const
{
    return true;
}
