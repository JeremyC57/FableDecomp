// CTCBed::HasFrameUpdate() const -- returns true
// Leaf: ?HasFrameUpdate@CTCBed@@UBE_NXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00746C10, thiscall, `ret`.
// Identity: slot 8 of CTCBed's vtable 0x01268964 (defined here: the slot holds this body in 1 vtable(s), all of CTCBed or derived); the Xbox 360 Test PDB virtual order (third source) aligned on 16 TLC-named slots of that vtable puts HasFrameUpdate(void) at this slot; Ego_r/FableWin name set: ?HasFrameUpdate@CTCBed@@UBE_NXZ; `ret` matches the signature.
class CTCBed {
public:
    virtual bool HasFrameUpdate() const;
};
bool CTCBed::HasFrameUpdate() const
{
    return true;
}
