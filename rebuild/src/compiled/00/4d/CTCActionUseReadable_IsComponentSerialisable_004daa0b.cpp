// CTCActionUseReadable::IsComponentSerialisable() const -- returns true
// Leaf: ?IsComponentSerialisable@CTCActionUseReadable@@UBE_NXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x004DAA0B, thiscall, `ret`.
// Identity: slot 23 of CTCActionUseReadable's vtable 0x0123F234 (defined here: the slot holds this body in 1 vtable(s), all of CTCActionUseReadable or derived); the Xbox 360 Test PDB virtual order (third source) aligned on 16 TLC-named slots of that vtable puts IsComponentSerialisable(void) at this slot; Ego_r/FableWin name set: ?IsComponentSerialisable@CTCActionUseReadable@@UBE_NXZ; `ret` matches the signature.
class CTCActionUseReadable {
public:
    virtual bool IsComponentSerialisable() const;
};
bool CTCActionUseReadable::IsComponentSerialisable() const
{
    return true;
}
