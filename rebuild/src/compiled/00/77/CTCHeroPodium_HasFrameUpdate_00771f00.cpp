// CTCHeroPodium::HasFrameUpdate() const -- returns true
// Leaf: ?HasFrameUpdate@CTCHeroPodium@@UBE_NXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00771F00, thiscall, `ret`.
// Identity: slot 8 of CTCHeroPodium's vtable 0x0126A89C (defined here: the slot holds this body in 1 vtable(s), all of CTCHeroPodium or derived); the Xbox 360 Test PDB virtual order (third source) aligned on 16 TLC-named slots of that vtable puts HasFrameUpdate(void) at this slot; Ego_r/FableWin name set: ?HasFrameUpdate@CTCHeroPodium@@UBE_NXZ; `ret` matches the signature.
class CTCHeroPodium {
public:
    virtual bool HasFrameUpdate() const;
};
bool CTCHeroPodium::HasFrameUpdate() const
{
    return true;
}
