// NCreatureMode::CModeArmsOffTable::GetType() const -- returns 0x2d
// Leaf: ?GetType@CModeArmsOffTable@NCreatureMode@@UBEKXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00741260, thiscall, `ret`.
// Identity: slot 1 of NCreatureMode::CModeArmsOffTable's vtable 0x012681A8 (defined here: the slot holds this body in 1 vtable(s), all of NCreatureMode::CModeArmsOffTable or derived); the Xbox 360 Test PDB virtual order (third source) aligned on 7 TLC-named slots of that vtable puts GetType(void) at this slot; Ego_r/FableWin name set: ?GetType@CModeArmsOffTable@NCreatureMode@@UBEKXZ; `ret` matches the signature.
namespace NCreatureMode {
class CModeArmsOffTable {
public:
    virtual unsigned long GetType() const;
};
unsigned long CModeArmsOffTable::GetType() const
{
    return 0x2d;
}
}
