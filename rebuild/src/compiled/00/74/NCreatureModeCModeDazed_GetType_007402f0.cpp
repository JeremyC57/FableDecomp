// NCreatureMode::CModeDazed::GetType() const -- returns 0x43
// Leaf: ?GetType@CModeDazed@NCreatureMode@@UBEKXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x007402F0, thiscall, `ret`.
// Identity: slot 1 of NCreatureMode::CModeDazed's vtable 0x01267CC4 (defined here: the slot holds this body in 1 vtable(s), all of NCreatureMode::CModeDazed or derived); the Xbox 360 Test PDB virtual order (third source) aligned on 8 TLC-named slots of that vtable puts GetType(void) at this slot; Ego_r/FableWin name set: ?GetType@CModeDazed@NCreatureMode@@UBEKXZ; `ret` matches the signature.
namespace NCreatureMode {
class CModeDazed {
public:
    virtual unsigned long GetType() const;
};
unsigned long CModeDazed::GetType() const
{
    return 0x43;
}
}
