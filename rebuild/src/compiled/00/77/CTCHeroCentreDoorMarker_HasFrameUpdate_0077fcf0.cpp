// CTCHeroCentreDoorMarker::HasFrameUpdate() const -- returns true
// Leaf: ?HasFrameUpdate@CTCHeroCentreDoorMarker@@UBE_NXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x0077FCF0, thiscall, `ret`.
// Identity: slot 8 of CTCHeroCentreDoorMarker's vtable 0x0126B044 (defined here: the slot holds this body in 1 vtable(s), all of CTCHeroCentreDoorMarker or derived); the Xbox 360 Test PDB virtual order (third source) aligned on 17 TLC-named slots of that vtable puts HasFrameUpdate(void) at this slot; Ego_r/FableWin name set: ?HasFrameUpdate@CTCHeroCentreDoorMarker@@UBE_NXZ; `ret` matches the signature.
class CTCHeroCentreDoorMarker {
public:
    virtual bool HasFrameUpdate() const;
};
bool CTCHeroCentreDoorMarker::HasFrameUpdate() const
{
    return true;
}
