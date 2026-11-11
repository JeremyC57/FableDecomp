// NCreatureMode::CModeStandLeft::GetType() const -- returns 1
// Leaf: ?GetType@CModeStandLeft@NCreatureMode@@UBEKXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x0073E4A0, thiscall, `ret`.
// Identity: slot 1 of NCreatureMode::CModeStandLeft's vtable 0x012674F4 (defined here: the slot holds this body in 1 vtable(s), all of NCreatureMode::CModeStandLeft or derived); the Xbox 360 Test PDB virtual order (third source) aligned on 7 TLC-named slots of that vtable puts GetType(void) at this slot; Ego_r/FableWin name set: ?GetType@CModeStandLeft@NCreatureMode@@UBEKXZ; `ret` matches the signature.
namespace NCreatureMode {
class CModeStandLeft {
public:
    virtual unsigned long GetType() const;
};
unsigned long CModeStandLeft::GetType() const
{
    return 1;
}
}
