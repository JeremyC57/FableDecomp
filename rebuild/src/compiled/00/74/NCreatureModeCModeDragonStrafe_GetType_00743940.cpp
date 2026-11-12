// NCreatureMode::CModeDragonStrafe::GetType() const -- returns 0x4c
// Leaf: ?GetType@CModeDragonStrafe@NCreatureMode@@UBEKXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00743940, thiscall, `ret`.
// Identity: slot 1 of NCreatureMode::CModeDragonStrafe's vtable 0x012686C8 (defined here: the slot holds this body in 1 vtable(s), all of NCreatureMode::CModeDragonStrafe or derived); the Xbox 360 Test PDB virtual order (third source) aligned on 7 TLC-named slots of that vtable puts GetType(void) at this slot; Ego_r/FableWin name set: ?GetType@CModeDragonStrafe@NCreatureMode@@UBEKXZ; `ret` matches the signature.
namespace NCreatureMode {
class CModeDragonStrafe {
public:
    virtual unsigned long GetType() const;
};
unsigned long CModeDragonStrafe::GetType() const
{
    return 0x4c;
}
}
