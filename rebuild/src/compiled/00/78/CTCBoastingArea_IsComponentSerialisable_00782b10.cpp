// CTCBoastingArea::IsComponentSerialisable() const -- returns true
// Leaf: ?IsComponentSerialisable@CTCBoastingArea@@UBE_NXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00782B10, thiscall, `ret`.
// Identity: slot 23 of CTCBoastingArea's vtable 0x0126B314 (defined here: the slot holds this body in 1 vtable(s), all of CTCBoastingArea or derived); the Xbox 360 Test PDB virtual order (third source) aligned on 16 TLC-named slots of that vtable puts IsComponentSerialisable(void) at this slot; Ego_r/FableWin name set: ?IsComponentSerialisable@CTCBoastingArea@@UBE_NXZ; `ret` matches the signature.
class CTCBoastingArea {
public:
    virtual bool IsComponentSerialisable() const;
};
bool CTCBoastingArea::IsComponentSerialisable() const
{
    return true;
}
