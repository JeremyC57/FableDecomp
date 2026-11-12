// NCreatureMode::CModeStrafe::GetType() const -- returns 0x19
// Leaf: ?GetType@CModeStrafe@NCreatureMode@@UBEKXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x007420A0, thiscall, `ret`.
// Identity: slot 1 of NCreatureMode::CModeStrafe's vtable 0x01268588 (defined here: the slot holds this body in 1 vtable(s), all of NCreatureMode::CModeStrafe or derived); the Xbox 360 Test PDB virtual order (third source) aligned on 7 TLC-named slots of that vtable puts GetType(void) at this slot; Ego_r/FableWin name set: ?GetType@CModeStrafe@NCreatureMode@@UBEKXZ; `ret` matches the signature.
namespace NCreatureMode {
class CModeStrafe {
public:
    virtual unsigned long GetType() const;
};
unsigned long CModeStrafe::GetType() const
{
    return 0x19;
}
}
