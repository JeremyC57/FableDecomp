// NCreatureMode::CModeIceTrollGroundSmashIcicleAssault::GetType() const -- returns 0x5d
// Leaf: ?GetType@CModeIceTrollGroundSmashIcicleAssault@NCreatureMode@@UBEKXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00743C90, thiscall, `ret`.
// Identity: slot 1 of NCreatureMode::CModeIceTrollGroundSmashIcicleAssault's vtable 0x01268774 (defined here: the slot holds this body in 1 vtable(s), all of NCreatureMode::CModeIceTrollGroundSmashIcicleAssault or derived); the Xbox 360 Test PDB virtual order (third source) aligned on 7 TLC-named slots of that vtable puts GetType(void) at this slot; Ego_r/FableWin name set: ?GetType@CModeIceTrollGroundSmashIcicleAssault@NCreatureMode@@UBEKXZ; `ret` matches the signature.
namespace NCreatureMode {
class CModeIceTrollGroundSmashIcicleAssault {
public:
    virtual unsigned long GetType() const;
};
unsigned long CModeIceTrollGroundSmashIcicleAssault::GetType() const
{
    return 0x5d;
}
}
