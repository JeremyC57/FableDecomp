// NCreatureMode::CModeJackOfBladesFloat::GetType() const -- returns 0x31
// Leaf: ?GetType@CModeJackOfBladesFloat@NCreatureMode@@UBEKXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x007403E0, thiscall, `ret`.
// Identity: slot 1 of NCreatureMode::CModeJackOfBladesFloat's vtable 0x01267D10 (defined here: the slot holds this body in 1 vtable(s), all of NCreatureMode::CModeJackOfBladesFloat or derived); the Xbox 360 Test PDB virtual order (third source) aligned on 7 TLC-named slots of that vtable puts GetType(void) at this slot; Ego_r/FableWin name set: ?GetType@CModeJackOfBladesFloat@NCreatureMode@@UBEKXZ; `ret` matches the signature.
namespace NCreatureMode {
class CModeJackOfBladesFloat {
public:
    virtual unsigned long GetType() const;
};
unsigned long CModeJackOfBladesFloat::GetType() const
{
    return 0x31;
}
}
