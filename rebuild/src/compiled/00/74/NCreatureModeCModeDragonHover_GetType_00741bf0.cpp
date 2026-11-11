// NCreatureMode::CModeDragonHover::GetType() const -- returns 0x47
// Leaf: ?GetType@CModeDragonHover@NCreatureMode@@UBEKXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00741BF0, thiscall, `ret`.
// Identity: slot 1 of NCreatureMode::CModeDragonHover's vtable 0x0126845C (defined here: the slot holds this body in 1 vtable(s), all of NCreatureMode::CModeDragonHover or derived); the Xbox 360 Test PDB virtual order (third source) aligned on 7 TLC-named slots of that vtable puts GetType(void) at this slot; Ego_r/FableWin name set: ?GetType@CModeDragonHover@NCreatureMode@@UBEKXZ; `ret` matches the signature.
namespace NCreatureMode {
class CModeDragonHover {
public:
    virtual unsigned long GetType() const;
};
unsigned long CModeDragonHover::GetType() const
{
    return 0x47;
}
}
