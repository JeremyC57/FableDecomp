// CTCNymph::HasFrameUpdate() const -- returns true
// Leaf: ?HasFrameUpdate@CTCNymph@@UBE_NXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x004E339C, thiscall, `ret`.
// Identity: slot 8 of CTCNymph's vtable 0x01242AD4 (defined here: the slot holds this body in 1 vtable(s), all of CTCNymph or derived); the Xbox 360 Test PDB virtual order (third source) aligned on 16 TLC-named slots of that vtable puts HasFrameUpdate(void) at this slot; Ego_r/FableWin name set: ?HasFrameUpdate@CTCNymph@@UBE_NXZ; `ret` matches the signature.
class CTCNymph {
public:
    virtual bool HasFrameUpdate() const;
};
bool CTCNymph::HasFrameUpdate() const
{
    return true;
}
