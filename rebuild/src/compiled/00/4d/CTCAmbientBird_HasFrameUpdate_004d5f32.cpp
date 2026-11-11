// CTCAmbientBird::HasFrameUpdate() const -- returns true
// Leaf: ?HasFrameUpdate@CTCAmbientBird@@UBE_NXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x004D5F32, thiscall, `ret`.
// Identity: slot 8 of CTCAmbientBird's vtable 0x0123DC0C (defined here: the slot holds this body in 1 vtable(s), all of CTCAmbientBird or derived); the Xbox 360 Test PDB virtual order (third source) aligned on 16 TLC-named slots of that vtable puts HasFrameUpdate(void) at this slot; Ego_r/FableWin name set: ?HasFrameUpdate@CTCAmbientBird@@UBE_NXZ; `ret` matches the signature.
class CTCAmbientBird {
public:
    virtual bool HasFrameUpdate() const;
};
bool CTCAmbientBird::HasFrameUpdate() const
{
    return true;
}
