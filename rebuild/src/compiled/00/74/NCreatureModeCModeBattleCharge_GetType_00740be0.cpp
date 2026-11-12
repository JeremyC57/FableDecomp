// NCreatureMode::CModeBattleCharge::GetType() const -- returns 0x23
// Leaf: ?GetType@CModeBattleCharge@NCreatureMode@@UBEKXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00740BE0, thiscall, `ret`.
// Identity: slot 1 of NCreatureMode::CModeBattleCharge's vtable 0x01267F90 (defined here: the slot holds this body in 1 vtable(s), all of NCreatureMode::CModeBattleCharge or derived); the Xbox 360 Test PDB virtual order (third source) aligned on 7 TLC-named slots of that vtable puts GetType(void) at this slot; Ego_r/FableWin name set: ?GetType@CModeBattleCharge@NCreatureMode@@UBEKXZ; `ret` matches the signature.
namespace NCreatureMode {
class CModeBattleCharge {
public:
    virtual unsigned long GetType() const;
};
unsigned long CModeBattleCharge::GetType() const
{
    return 0x23;
}
}
