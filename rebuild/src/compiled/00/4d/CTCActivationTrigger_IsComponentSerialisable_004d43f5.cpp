// CTCActivationTrigger::IsComponentSerialisable() const -- returns true
// Leaf: ?IsComponentSerialisable@CTCActivationTrigger@@UBE_NXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x004D43F5, thiscall, `ret`.
// Identity: slot 23 of CTCActivationTrigger's vtable 0x0123BBAC (defined here: the slot holds this body in 1 vtable(s), all of CTCActivationTrigger or derived); the Xbox 360 Test PDB virtual order (third source) aligned on 15 TLC-named slots of that vtable puts IsComponentSerialisable(void) at this slot; Ego_r/FableWin name set: ?IsComponentSerialisable@CTCActivationTrigger@@UBE_NXZ; `ret` matches the signature.
class CTCActivationTrigger {
public:
    virtual bool IsComponentSerialisable() const;
};
bool CTCActivationTrigger::IsComponentSerialisable() const
{
    return true;
}
