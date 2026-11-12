// NCreatureMode::CModeBerserk::GetType() const -- returns 0x24
// Leaf: ?GetType@CModeBerserk@NCreatureMode@@UBEKXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00740A40, thiscall, `ret`.
// Identity: slot 1 of NCreatureMode::CModeBerserk's vtable 0x01267F00 (defined here: the slot holds this body in 1 vtable(s), all of NCreatureMode::CModeBerserk or derived); the Xbox 360 Test PDB virtual order (third source) aligned on 6 TLC-named slots of that vtable puts GetType(void) at this slot; Ego_r/FableWin name set: ?GetType@CModeBerserk@NCreatureMode@@UBEKXZ; `ret` matches the signature.
namespace NCreatureMode {
class CModeBerserk {
public:
    virtual unsigned long GetType() const;
};
unsigned long CModeBerserk::GetType() const
{
    return 0x24;
}
}
