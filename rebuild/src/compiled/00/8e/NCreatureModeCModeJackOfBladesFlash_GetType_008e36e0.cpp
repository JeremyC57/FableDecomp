// NCreatureMode::CModeJackOfBladesFlash::GetType() const -- returns 0x32
// Leaf: ?GetType@CModeJackOfBladesFlash@NCreatureMode@@UBEKXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x008E36E0, thiscall, `ret`.
// Identity: slot 1 of NCreatureMode::CModeJackOfBladesFlash's vtable 0x012857A8 (defined here: the slot holds this body in 1 vtable(s), all of NCreatureMode::CModeJackOfBladesFlash or derived); the Xbox 360 Test PDB virtual order (third source) aligned on 7 TLC-named slots of that vtable puts GetType(void) at this slot; Ego_r/FableWin name set: ?GetType@CModeJackOfBladesFlash@NCreatureMode@@UBEKXZ; `ret` matches the signature.
namespace NCreatureMode {
class CModeJackOfBladesFlash {
public:
    virtual unsigned long GetType() const;
};
unsigned long CModeJackOfBladesFlash::GetType() const
{
    return 0x32;
}
}
