// CTCCarriedActionUseRead::IsComponentSerialisable() const -- returns true
// Leaf: ?IsComponentSerialisable@CTCCarriedActionUseRead@@UBE_NXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x004DB396, thiscall, `ret`.
// Identity: slot 23 of CTCCarriedActionUseRead's vtable 0x01240834 (defined here: the slot holds this body in 1 vtable(s), all of CTCCarriedActionUseRead or derived); the Xbox 360 Test PDB virtual order (third source) aligned on 14 TLC-named slots of that vtable puts IsComponentSerialisable(void) at this slot; Ego_r/FableWin name set: ?IsComponentSerialisable@CTCCarriedActionUseRead@@UBE_NXZ; `ret` matches the signature.
class CTCCarriedActionUseRead {
public:
    virtual bool IsComponentSerialisable() const;
};
bool CTCCarriedActionUseRead::IsComponentSerialisable() const
{
    return true;
}
