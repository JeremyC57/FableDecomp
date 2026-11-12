// NCreatureMode::CModeGuildmasterPowerUpHeal::GetType() const -- returns 0x3c
// Leaf: ?GetType@CModeGuildmasterPowerUpHeal@NCreatureMode@@UBEKXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00742D80, thiscall, `ret`.
// Identity: slot 1 of NCreatureMode::CModeGuildmasterPowerUpHeal's vtable 0x012685BC (defined here: the slot holds this body in 1 vtable(s), all of NCreatureMode::CModeGuildmasterPowerUpHeal or derived); the Xbox 360 Test PDB virtual order (third source) aligned on 7 TLC-named slots of that vtable puts GetType(void) at this slot; Ego_r/FableWin name set: ?GetType@CModeGuildmasterPowerUpHeal@NCreatureMode@@UBEKXZ; `ret` matches the signature.
namespace NCreatureMode {
class CModeGuildmasterPowerUpHeal {
public:
    virtual unsigned long GetType() const;
};
unsigned long CModeGuildmasterPowerUpHeal::GetType() const
{
    return 0x3c;
}
}
