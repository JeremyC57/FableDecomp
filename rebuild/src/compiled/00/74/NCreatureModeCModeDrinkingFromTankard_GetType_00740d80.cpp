// NCreatureMode::CModeDrinkingFromTankard::GetType() const -- returns 0x27
// Leaf: ?GetType@CModeDrinkingFromTankard@NCreatureMode@@UBEKXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00740D80, thiscall, `ret`.
// Identity: slot 1 of NCreatureMode::CModeDrinkingFromTankard's vtable 0x01268018 (defined here: the slot holds this body in 1 vtable(s), all of NCreatureMode::CModeDrinkingFromTankard or derived); the Xbox 360 Test PDB virtual order (third source) aligned on 7 TLC-named slots of that vtable puts GetType(void) at this slot; Ego_r/FableWin name set: ?GetType@CModeDrinkingFromTankard@NCreatureMode@@UBEKXZ; `ret` matches the signature.
namespace NCreatureMode {
class CModeDrinkingFromTankard {
public:
    virtual unsigned long GetType() const;
};
unsigned long CModeDrinkingFromTankard::GetType() const
{
    return 0x27;
}
}
