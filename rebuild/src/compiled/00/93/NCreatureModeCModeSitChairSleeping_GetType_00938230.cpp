// NCreatureMode::CModeSitChairSleeping::GetType() const -- returns 8
// Leaf: ?GetType@CModeSitChairSleeping@NCreatureMode@@UBEKXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00938230, thiscall, `ret`.
// Identity: slot 1 of NCreatureMode::CModeSitChairSleeping's vtable 0x01292398 (defined here: the slot holds this body in 1 vtable(s), all of NCreatureMode::CModeSitChairSleeping or derived); the Xbox 360 Test PDB virtual order (third source) aligned on 7 TLC-named slots of that vtable puts GetType(void) at this slot; Ego_r/FableWin name set: ?GetType@CModeSitChairSleeping@NCreatureMode@@UBEKXZ; `ret` matches the signature.
namespace NCreatureMode {
class CModeSitChairSleeping {
public:
    virtual unsigned long GetType() const;
};
unsigned long CModeSitChairSleeping::GetType() const
{
    return 8;
}
}
