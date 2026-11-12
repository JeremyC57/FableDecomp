// NCreatureMode::CModeJackOfBladesScream::GetType() const -- returns 0x33
// Leaf: ?GetType@CModeJackOfBladesScream@NCreatureMode@@UBEKXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00740500, thiscall, `ret`.
// Identity: slot 1 of NCreatureMode::CModeJackOfBladesScream's vtable 0x01267D5C (defined here: the slot holds this body in 1 vtable(s), all of NCreatureMode::CModeJackOfBladesScream or derived); the Xbox 360 Test PDB virtual order (third source) aligned on 7 TLC-named slots of that vtable puts GetType(void) at this slot; Ego_r/FableWin name set: ?GetType@CModeJackOfBladesScream@NCreatureMode@@UBEKXZ; `ret` matches the signature.
namespace NCreatureMode {
class CModeJackOfBladesScream {
public:
    virtual unsigned long GetType() const;
};
unsigned long CModeJackOfBladesScream::GetType() const
{
    return 0x33;
}
}
