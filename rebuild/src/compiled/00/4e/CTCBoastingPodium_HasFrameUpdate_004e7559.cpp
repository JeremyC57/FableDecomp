// CTCBoastingPodium::HasFrameUpdate() const -- returns true
// Leaf: ?HasFrameUpdate@CTCBoastingPodium@@UBE_NXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x004E7559, thiscall, `ret`.
// Identity: slot 8 of CTCBoastingPodium's vtable 0x0124346C (defined here: the slot holds this body in 1 vtable(s), all of CTCBoastingPodium or derived); the Xbox 360 Test PDB virtual order (third source) aligned on 16 TLC-named slots of that vtable puts HasFrameUpdate(void) at this slot; Ego_r/FableWin name set: ?HasFrameUpdate@CTCBoastingPodium@@UBE_NXZ; `ret` matches the signature.
class CTCBoastingPodium {
public:
    virtual bool HasFrameUpdate() const;
};
bool CTCBoastingPodium::HasFrameUpdate() const
{
    return true;
}
