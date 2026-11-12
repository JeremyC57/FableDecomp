// NCreatureMode::CModeScreamerAggressive::GetType() const -- returns 0x44
// Leaf: ?GetType@CModeScreamerAggressive@NCreatureMode@@UBEKXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00741350, thiscall, `ret`.
// Identity: slot 1 of NCreatureMode::CModeScreamerAggressive's vtable 0x012681EC (defined here: the slot holds this body in 1 vtable(s), all of NCreatureMode::CModeScreamerAggressive or derived); the Xbox 360 Test PDB virtual order (third source) aligned on 7 TLC-named slots of that vtable puts GetType(void) at this slot; Ego_r/FableWin name set: ?GetType@CModeScreamerAggressive@NCreatureMode@@UBEKXZ; `ret` matches the signature.
namespace NCreatureMode {
class CModeScreamerAggressive {
public:
    virtual unsigned long GetType() const;
};
unsigned long CModeScreamerAggressive::GetType() const
{
    return 0x44;
}
}
