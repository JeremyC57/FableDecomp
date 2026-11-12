// CTCDRegionEntrance::IsComponentSerialisable() const -- returns true
// Leaf: ?IsComponentSerialisable@CTCDRegionEntrance@@UBE_NXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x004D42EF, thiscall, `ret`.
// Identity: slot 23 of CTCDRegionEntrance's vtable 0x0123BA74 (defined here: the slot holds this body in 1 vtable(s), all of CTCDRegionEntrance or derived); the Xbox 360 Test PDB virtual order (third source) aligned on 16 TLC-named slots of that vtable puts IsComponentSerialisable(void) at this slot; Ego_r/FableWin name set: ?IsComponentSerialisable@CTCDRegionEntrance@@UBE_NXZ; `ret` matches the signature.
class CTCDRegionEntrance {
public:
    virtual bool IsComponentSerialisable() const;
};
bool CTCDRegionEntrance::IsComponentSerialisable() const
{
    return true;
}
