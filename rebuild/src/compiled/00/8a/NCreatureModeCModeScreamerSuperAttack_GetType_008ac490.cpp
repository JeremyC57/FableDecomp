// NCreatureMode::CModeScreamerSuperAttack::GetType() const -- returns 0x5e
// Leaf: ?GetType@CModeScreamerSuperAttack@NCreatureMode@@UBEKXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x008AC490, thiscall, `ret`.
// Identity: slot 1 of NCreatureMode::CModeScreamerSuperAttack's vtable 0x01278338 (defined here: the slot holds this body in 1 vtable(s), all of NCreatureMode::CModeScreamerSuperAttack or derived); the Xbox 360 Test PDB virtual order (third source) aligned on 7 TLC-named slots of that vtable puts GetType(void) at this slot; Ego_r/FableWin name set: ?GetType@CModeScreamerSuperAttack@NCreatureMode@@UBEKXZ; `ret` matches the signature.
namespace NCreatureMode {
class CModeScreamerSuperAttack {
public:
    virtual unsigned long GetType() const;
};
unsigned long CModeScreamerSuperAttack::GetType() const
{
    return 0x5e;
}
}
