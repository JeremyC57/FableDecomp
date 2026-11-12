// CTCCreditsUI::HasFrameUpdate() const -- returns true
// Leaf: ?HasFrameUpdate@CTCCreditsUI@@UBE_NXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x007E4C40, thiscall, `ret`.
// Identity: slot 8 of CTCCreditsUI's vtable 0x012705AC (defined here: the slot holds this body in 1 vtable(s), all of CTCCreditsUI or derived); the Xbox 360 Test PDB virtual order (third source) aligned on 16 TLC-named slots of that vtable puts HasFrameUpdate(void) at this slot; Ego_r/FableWin name set: ?HasFrameUpdate@CTCCreditsUI@@UBE_NXZ; `ret` matches the signature.
class CTCCreditsUI {
public:
    virtual bool HasFrameUpdate() const;
};
bool CTCCreditsUI::HasFrameUpdate() const
{
    return true;
}
