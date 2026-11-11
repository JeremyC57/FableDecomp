// NCreatureMode::CModeStunned::GetType() const -- returns 0x1d
// Leaf: ?GetType@CModeStunned@NCreatureMode@@UBEKXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00740200, thiscall, `ret`.
// Identity: slot 1 of NCreatureMode::CModeStunned's vtable 0x01267C74 (defined here: the slot holds this body in 1 vtable(s), all of NCreatureMode::CModeStunned or derived); the Xbox 360 Test PDB virtual order (third source) aligned on 9 TLC-named slots of that vtable puts GetType(void) at this slot; Ego_r/FableWin name set: ?GetType@CModeStunned@NCreatureMode@@UBEKXZ; `ret` matches the signature.
namespace NCreatureMode {
class CModeStunned {
public:
    virtual unsigned long GetType() const;
};
unsigned long CModeStunned::GetType() const
{
    return 0x1d;
}
}
