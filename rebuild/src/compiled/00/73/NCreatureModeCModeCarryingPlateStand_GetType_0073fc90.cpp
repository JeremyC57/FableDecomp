// NCreatureMode::CModeCarryingPlateStand::GetType() const -- returns 5
// Leaf: ?GetType@CModeCarryingPlateStand@NCreatureMode@@UBEKXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x0073FC90, thiscall, `ret`.
// Identity: slot 1 of NCreatureMode::CModeCarryingPlateStand's vtable 0x01267AA0 (defined here: the slot holds this body in 1 vtable(s), all of NCreatureMode::CModeCarryingPlateStand or derived); the Xbox 360 Test PDB virtual order (third source) aligned on 8 TLC-named slots of that vtable puts GetType(void) at this slot; Ego_r/FableWin name set: ?GetType@CModeCarryingPlateStand@NCreatureMode@@UBEKXZ; `ret` matches the signature.
namespace NCreatureMode {
class CModeCarryingPlateStand {
public:
    virtual unsigned long GetType() const;
};
unsigned long CModeCarryingPlateStand::GetType() const
{
    return 5;
}
}
