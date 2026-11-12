// NCreatureMode::CModeDragonLanded::GetType() const -- returns 0x4e
// Leaf: ?GetType@CModeDragonLanded@NCreatureMode@@UBEKXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00741960, thiscall, `ret`.
// Identity: slot 1 of NCreatureMode::CModeDragonLanded's vtable 0x012683A8 (defined here: the slot holds this body in 1 vtable(s), all of NCreatureMode::CModeDragonLanded or derived); the Xbox 360 Test PDB virtual order (third source) aligned on 6 TLC-named slots of that vtable puts GetType(void) at this slot; Ego_r/FableWin name set: ?GetType@CModeDragonLanded@NCreatureMode@@UBEKXZ; `ret` matches the signature.
namespace NCreatureMode {
class CModeDragonLanded {
public:
    virtual unsigned long GetType() const;
};
unsigned long CModeDragonLanded::GetType() const
{
    return 0x4e;
}
}
