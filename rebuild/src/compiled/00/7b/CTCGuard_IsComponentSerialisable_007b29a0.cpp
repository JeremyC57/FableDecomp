// CTCGuard::IsComponentSerialisable() const -- returns true
// Leaf: ?IsComponentSerialisable@CTCGuard@@UBE_NXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x007B29A0, thiscall, `ret`.
// Identity: slot 23 of CTCGuard's vtable 0x0126D80C (defined here: the slot holds this body in 1 vtable(s), all of CTCGuard or derived); the Xbox 360 Test PDB virtual order (third source) aligned on 16 TLC-named slots of that vtable puts IsComponentSerialisable(void) at this slot; Ego_r/FableWin name set: ?IsComponentSerialisable@CTCGuard@@UBE_NXZ; `ret` matches the signature.
class CTCGuard {
public:
    virtual bool IsComponentSerialisable() const;
};
bool CTCGuard::IsComponentSerialisable() const
{
    return true;
}
