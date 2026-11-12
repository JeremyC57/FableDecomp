// CTCSearchableContainer::IsComponentSerialisable() const -- returns true
// Leaf: ?IsComponentSerialisable@CTCSearchableContainer@@UBE_NXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x004E48D7, thiscall, `ret`.
// Identity: slot 23 of CTCSearchableContainer's vtable 0x012431F4 (defined here: the slot holds this body in 1 vtable(s), all of CTCSearchableContainer or derived); the Xbox 360 Test PDB virtual order (third source) aligned on 16 TLC-named slots of that vtable puts IsComponentSerialisable(void) at this slot; Ego_r/FableWin name set: ?IsComponentSerialisable@CTCSearchableContainer@@UBE_NXZ; `ret` matches the signature.
class CTCSearchableContainer {
public:
    virtual bool IsComponentSerialisable() const;
};
bool CTCSearchableContainer::IsComponentSerialisable() const
{
    return true;
}
