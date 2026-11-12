// NCreatureMode::CModeStandBored::GetType() const -- returns 3
// Leaf: ?GetType@CModeStandBored@NCreatureMode@@UBEKXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x0073EDD0, thiscall, `ret`.
// Identity: slot 1 of NCreatureMode::CModeStandBored's vtable 0x01267630 (defined here: the slot holds this body in 1 vtable(s), all of NCreatureMode::CModeStandBored or derived); the Xbox 360 Test PDB virtual order (third source) aligned on 7 TLC-named slots of that vtable puts GetType(void) at this slot; Ego_r/FableWin name set: ?GetType@CModeStandBored@NCreatureMode@@UBEKXZ; `ret` matches the signature.
namespace NCreatureMode {
class CModeStandBored {
public:
    virtual unsigned long GetType() const;
};
unsigned long CModeStandBored::GetType() const
{
    return 3;
}
}
