// NCreatureMode::CModeCarryingGoodsStand::GetType() const -- returns 6
// Leaf: ?GetType@CModeCarryingGoodsStand@NCreatureMode@@UBEKXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x0073FD60, thiscall, `ret`.
// Identity: slot 1 of NCreatureMode::CModeCarryingGoodsStand's vtable 0x01267AE8 (defined here: the slot holds this body in 1 vtable(s), all of NCreatureMode::CModeCarryingGoodsStand or derived); the Xbox 360 Test PDB virtual order (third source) aligned on 7 TLC-named slots of that vtable puts GetType(void) at this slot; Ego_r/FableWin name set: ?GetType@CModeCarryingGoodsStand@NCreatureMode@@UBEKXZ; `ret` matches the signature.
namespace NCreatureMode {
class CModeCarryingGoodsStand {
public:
    virtual unsigned long GetType() const;
};
unsigned long CModeCarryingGoodsStand::GetType() const
{
    return 6;
}
}
