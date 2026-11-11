// CTCDiggingSpot::IsComponentSerialisable() const -- returns true
// Leaf: ?IsComponentSerialisable@CTCDiggingSpot@@UBE_NXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x007E25B0, thiscall, `ret`.
// Identity: slot 23 of CTCDiggingSpot's vtable 0x0127017C (defined here: the slot holds this body in 1 vtable(s), all of CTCDiggingSpot or derived); the Xbox 360 Test PDB virtual order (third source) aligned on 16 TLC-named slots of that vtable puts IsComponentSerialisable(void) at this slot; Ego_r/FableWin name set: ?IsComponentSerialisable@CTCDiggingSpot@@UBE_NXZ; `ret` matches the signature.
class CTCDiggingSpot {
public:
    virtual bool IsComponentSerialisable() const;
};
bool CTCDiggingSpot::IsComponentSerialisable() const
{
    return true;
}
