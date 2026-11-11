// CTCOnHeroPush::HasFrameUpdate() const -- returns true
// Leaf: ?HasFrameUpdate@CTCOnHeroPush@@UBE_NXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x004D536F, thiscall, `ret`.
// Identity: slot 8 of CTCOnHeroPush's vtable 0x0123CBC4 (defined here: the slot holds this body in 1 vtable(s), all of CTCOnHeroPush or derived); the Xbox 360 Test PDB virtual order (third source) aligned on 15 TLC-named slots of that vtable puts HasFrameUpdate(void) at this slot; Ego_r/FableWin name set: ?HasFrameUpdate@CTCOnHeroPush@@UBE_NXZ; `ret` matches the signature.
class CTCOnHeroPush {
public:
    virtual bool HasFrameUpdate() const;
};
bool CTCOnHeroPush::HasFrameUpdate() const
{
    return true;
}
