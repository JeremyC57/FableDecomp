// NCreatureMode::CModeJackOfBladesDeath::GetType() const -- returns 0x35
// Leaf: ?GetType@CModeJackOfBladesDeath@NCreatureMode@@UBEKXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x007405F0, thiscall, `ret`.
// Identity: slot 1 of NCreatureMode::CModeJackOfBladesDeath's vtable 0x01267DA8 (defined here: the slot holds this body in 1 vtable(s), all of NCreatureMode::CModeJackOfBladesDeath or derived); the Xbox 360 Test PDB virtual order (third source) aligned on 7 TLC-named slots of that vtable puts GetType(void) at this slot; Ego_r/FableWin name set: ?GetType@CModeJackOfBladesDeath@NCreatureMode@@UBEKXZ; `ret` matches the signature.
namespace NCreatureMode {
class CModeJackOfBladesDeath {
public:
    virtual unsigned long GetType() const;
};
unsigned long CModeJackOfBladesDeath::GetType() const
{
    return 0x35;
}
}
