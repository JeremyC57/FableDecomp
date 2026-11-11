// CTCHeroCentreDoorMarker::IsComponentSerialisable() const -- returns true
// Leaf: ?IsComponentSerialisable@CTCHeroCentreDoorMarker@@UBE_NXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x0077FCE0, thiscall, `ret`.
// Identity: slot 23 of CTCHeroCentreDoorMarker's vtable 0x0126B044 (defined here: the slot holds this body in 1 vtable(s), all of CTCHeroCentreDoorMarker or derived); the Xbox 360 Test PDB virtual order (third source) aligned on 17 TLC-named slots of that vtable puts IsComponentSerialisable(void) at this slot; Ego_r/FableWin name set: ?IsComponentSerialisable@CTCHeroCentreDoorMarker@@UBE_NXZ; `ret` matches the signature.
class CTCHeroCentreDoorMarker {
public:
    virtual bool IsComponentSerialisable() const;
};
bool CTCHeroCentreDoorMarker::IsComponentSerialisable() const
{
    return true;
}
