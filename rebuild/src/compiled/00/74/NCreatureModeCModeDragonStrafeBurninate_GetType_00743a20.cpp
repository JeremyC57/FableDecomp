// NCreatureMode::CModeDragonStrafeBurninate::GetType() const -- returns 0x4d
// Leaf: ?GetType@CModeDragonStrafeBurninate@NCreatureMode@@UBEKXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00743A20, thiscall, `ret`.
// Identity: slot 1 of NCreatureMode::CModeDragonStrafeBurninate's vtable 0x01268718 (defined here: the slot holds this body in 1 vtable(s), all of NCreatureMode::CModeDragonStrafeBurninate or derived); the Xbox 360 Test PDB virtual order (third source) aligned on 7 TLC-named slots of that vtable puts GetType(void) at this slot; Ego_r/FableWin name set: ?GetType@CModeDragonStrafeBurninate@NCreatureMode@@UBEKXZ; `ret` matches the signature.
namespace NCreatureMode {
class CModeDragonStrafeBurninate {
public:
    virtual unsigned long GetType() const;
};
unsigned long CModeDragonStrafeBurninate::GetType() const
{
    return 0x4d;
}
}
