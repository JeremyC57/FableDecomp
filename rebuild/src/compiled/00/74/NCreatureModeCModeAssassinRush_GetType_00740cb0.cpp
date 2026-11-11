// NCreatureMode::CModeAssassinRush::GetType() const -- returns 0x25
// Leaf: ?GetType@CModeAssassinRush@NCreatureMode@@UBEKXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00740CB0, thiscall, `ret`.
// Identity: slot 1 of NCreatureMode::CModeAssassinRush's vtable 0x01267FD4 (defined here: the slot holds this body in 1 vtable(s), all of NCreatureMode::CModeAssassinRush or derived); the Xbox 360 Test PDB virtual order (third source) aligned on 7 TLC-named slots of that vtable puts GetType(void) at this slot; Ego_r/FableWin name set: ?GetType@CModeAssassinRush@NCreatureMode@@UBEKXZ; `ret` matches the signature.
namespace NCreatureMode {
class CModeAssassinRush {
public:
    virtual unsigned long GetType() const;
};
unsigned long CModeAssassinRush::GetType() const
{
    return 0x25;
}
}
