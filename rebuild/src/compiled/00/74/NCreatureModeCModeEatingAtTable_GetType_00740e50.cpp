// NCreatureMode::CModeEatingAtTable::GetType() const -- returns 0x28
// Leaf: ?GetType@CModeEatingAtTable@NCreatureMode@@UBEKXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00740E50, thiscall, `ret`.
// Identity: slot 1 of NCreatureMode::CModeEatingAtTable's vtable 0x01268064 (defined here: the slot holds this body in 1 vtable(s), all of NCreatureMode::CModeEatingAtTable or derived); the Xbox 360 Test PDB virtual order (third source) aligned on 7 TLC-named slots of that vtable puts GetType(void) at this slot; Ego_r/FableWin name set: ?GetType@CModeEatingAtTable@NCreatureMode@@UBEKXZ; `ret` matches the signature.
namespace NCreatureMode {
class CModeEatingAtTable {
public:
    virtual unsigned long GetType() const;
};
unsigned long CModeEatingAtTable::GetType() const
{
    return 0x28;
}
}
