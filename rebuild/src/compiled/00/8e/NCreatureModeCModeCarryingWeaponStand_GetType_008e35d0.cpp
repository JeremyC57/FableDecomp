// NCreatureMode::CModeCarryingWeaponStand::GetType() const -- returns 4
// Leaf: ?GetType@CModeCarryingWeaponStand@NCreatureMode@@UBEKXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x008E35D0, thiscall, `ret`.
// Identity: slot 1 of NCreatureMode::CModeCarryingWeaponStand's vtable 0x012856E4 (defined here: the slot holds this body in 1 vtable(s), all of NCreatureMode::CModeCarryingWeaponStand or derived); the Xbox 360 Test PDB virtual order (third source) aligned on 7 TLC-named slots of that vtable puts GetType(void) at this slot; Ego_r/FableWin name set: ?GetType@CModeCarryingWeaponStand@NCreatureMode@@UBEKXZ; `ret` matches the signature.
namespace NCreatureMode {
class CModeCarryingWeaponStand {
public:
    virtual unsigned long GetType() const;
};
unsigned long CModeCarryingWeaponStand::GetType() const
{
    return 4;
}
}
