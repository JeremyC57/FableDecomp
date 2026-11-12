// NCreatureMode::CModeDragonFlying::GetType() const -- returns 0x50
// Leaf: ?GetType@CModeDragonFlying@NCreatureMode@@UBEKXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00743860, thiscall, `ret`.
// Identity: slot 1 of NCreatureMode::CModeDragonFlying's vtable 0x0126868C (defined here: the slot holds this body in 1 vtable(s), all of NCreatureMode::CModeDragonFlying or derived); the Xbox 360 Test PDB virtual order (third source) aligned on 7 TLC-named slots of that vtable puts GetType(void) at this slot; Ego_r/FableWin name set: ?GetType@CModeDragonFlying@NCreatureMode@@UBEKXZ; `ret` matches the signature.
namespace NCreatureMode {
class CModeDragonFlying {
public:
    virtual unsigned long GetType() const;
};
unsigned long CModeDragonFlying::GetType() const
{
    return 0x50;
}
}
