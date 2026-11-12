// NCreatureMode::CModeStandHappy::GetType() const -- returns 2
// Leaf: ?GetType@CModeStandHappy@NCreatureMode@@UBEKXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x0073ECF0, thiscall, `ret`.
// Identity: slot 1 of NCreatureMode::CModeStandHappy's vtable 0x012675F0 (defined here: the slot holds this body in 1 vtable(s), all of NCreatureMode::CModeStandHappy or derived); the Xbox 360 Test PDB virtual order (third source) aligned on 7 TLC-named slots of that vtable puts GetType(void) at this slot; Ego_r/FableWin name set: ?GetType@CModeStandHappy@NCreatureMode@@UBEKXZ; `ret` matches the signature.
namespace NCreatureMode {
class CModeStandHappy {
public:
    virtual unsigned long GetType() const;
};
unsigned long CModeStandHappy::GetType() const
{
    return 2;
}
}
