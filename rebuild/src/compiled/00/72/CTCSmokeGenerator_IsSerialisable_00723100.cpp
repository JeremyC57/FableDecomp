// CTCSmokeGenerator::IsSerialisable() const -- returns true
// Leaf: ?IsSerialisable@CTCSmokeGenerator@@MBE_NXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00723100, thiscall, `ret`.
// Identity: slot 28 of CTCSmokeGenerator's vtable 0x01265D74 (defined here: the slot holds this body in 1 vtable(s), all of CTCSmokeGenerator or derived); the Xbox 360 Test PDB virtual order (third source) aligned on 16 TLC-named slots of that vtable puts IsSerialisable(void) at this slot; Ego_r/FableWin name set: ?IsSerialisable@CTCSmokeGenerator@@MBE_NXZ; `ret` matches the signature.
class CTCSmokeGenerator {
public:
protected:
    virtual bool IsSerialisable() const;
};
bool CTCSmokeGenerator::IsSerialisable() const
{
    return true;
}
