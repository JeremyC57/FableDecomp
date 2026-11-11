// NCreatureMode::CModeAttracted::GetType() const -- returns 0x15
// Leaf: ?GetType@CModeAttracted@NCreatureMode@@UBEKXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x0073F1F0, thiscall, `ret`.
// Identity: slot 1 of NCreatureMode::CModeAttracted's vtable 0x01267758 (defined here: the slot holds this body in 1 vtable(s), all of NCreatureMode::CModeAttracted or derived); the Xbox 360 Test PDB virtual order (third source) aligned on 7 TLC-named slots of that vtable puts GetType(void) at this slot; Ego_r/FableWin name set: ?GetType@CModeAttracted@NCreatureMode@@UBEKXZ; `ret` matches the signature.
namespace NCreatureMode {
class CModeAttracted {
public:
    virtual unsigned long GetType() const;
};
unsigned long CModeAttracted::GetType() const
{
    return 0x15;
}
}
