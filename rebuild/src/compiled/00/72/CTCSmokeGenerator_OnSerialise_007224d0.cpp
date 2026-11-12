// CTCSmokeGenerator::OnSerialise(CPersistContext&, bool) -- does nothing
// Leaf: ?OnSerialise@CTCSmokeGenerator@@UAEXAAVCPersistContext@@_N@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x007224D0, thiscall, `ret 8`.
// Identity: slot 1 of CTCSmokeGenerator's vtable 0x01265D74 (defined here: the slot holds this body in 1 vtable(s), all of CTCSmokeGenerator or derived); the Xbox 360 Test PDB virtual order (third source) aligned on 16 TLC-named slots of that vtable puts OnSerialise(CPersistContext &, bool) at this slot; Ego_r/FableWin name set: ?OnSerialise@CTCSmokeGenerator@@UAEXAAVCPersistContext@@_N@Z; `ret` matches the signature.
class CPersistContext;
class CTCSmokeGenerator {
public:
    virtual void OnSerialise(CPersistContext&, bool);
};
void CTCSmokeGenerator::OnSerialise(CPersistContext&, bool)
{
}
