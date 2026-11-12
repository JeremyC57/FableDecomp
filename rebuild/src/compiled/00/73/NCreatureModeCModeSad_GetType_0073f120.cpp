// NCreatureMode::CModeSad::GetType() const -- returns 0x14
// Leaf: ?GetType@CModeSad@NCreatureMode@@UBEKXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x0073F120, thiscall, `ret`.
// Identity: slot 1 of NCreatureMode::CModeSad's vtable 0x01267720 (defined here: the slot holds this body in 1 vtable(s), all of NCreatureMode::CModeSad or derived); the Xbox 360 Test PDB virtual order (third source) aligned on 7 TLC-named slots of that vtable puts GetType(void) at this slot; Ego_r/FableWin name set: ?GetType@CModeSad@NCreatureMode@@UBEKXZ; `ret` matches the signature.
namespace NCreatureMode {
class CModeSad {
public:
    virtual unsigned long GetType() const;
};
unsigned long CModeSad::GetType() const
{
    return 0x14;
}
}
