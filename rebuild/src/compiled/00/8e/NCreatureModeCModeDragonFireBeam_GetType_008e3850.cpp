// NCreatureMode::CModeDragonFireBeam::GetType() const -- returns 0x4b
// Leaf: ?GetType@CModeDragonFireBeam@NCreatureMode@@UBEKXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x008E3850, thiscall, `ret`.
// Identity: slot 1 of NCreatureMode::CModeDragonFireBeam's vtable 0x01285900 (defined here: the slot holds this body in 1 vtable(s), all of NCreatureMode::CModeDragonFireBeam or derived); the Xbox 360 Test PDB virtual order (third source) aligned on 7 TLC-named slots of that vtable puts GetType(void) at this slot; Ego_r/FableWin name set: ?GetType@CModeDragonFireBeam@NCreatureMode@@UBEKXZ; `ret` matches the signature.
namespace NCreatureMode {
class CModeDragonFireBeam {
public:
    virtual unsigned long GetType() const;
};
unsigned long CModeDragonFireBeam::GetType() const
{
    return 0x4b;
}
}
