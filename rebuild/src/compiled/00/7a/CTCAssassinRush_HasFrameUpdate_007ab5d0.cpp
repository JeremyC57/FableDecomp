// CTCAssassinRush::HasFrameUpdate() const -- returns true
// Leaf: ?HasFrameUpdate@CTCAssassinRush@@UBE_NXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x007AB5D0, thiscall, `ret`.
// Identity: slot 8 of CTCAssassinRush's vtable 0x0126CF2C (defined here: the slot holds this body in 1 vtable(s), all of CTCAssassinRush or derived); the Xbox 360 Test PDB virtual order (third source) aligned on 16 TLC-named slots of that vtable puts HasFrameUpdate(void) at this slot; Ego_r/FableWin name set: ?HasFrameUpdate@CTCAssassinRush@@UBE_NXZ; `ret` matches the signature.
class CTCAssassinRush {
public:
    virtual bool HasFrameUpdate() const;
};
bool CTCAssassinRush::HasFrameUpdate() const
{
    return true;
}
