// NCreatureMode::CModeSummonerDie::GetType() const -- returns 0x5c
// Leaf: ?GetType@CModeSummonerDie@NCreatureMode@@UBEKXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x007416E0, thiscall, `ret`.
// Identity: slot 1 of NCreatureMode::CModeSummonerDie's vtable 0x012682EC (defined here: the slot holds this body in 1 vtable(s), all of NCreatureMode::CModeSummonerDie or derived); the Xbox 360 Test PDB virtual order (third source) aligned on 6 TLC-named slots of that vtable puts GetType(void) at this slot; Ego_r/FableWin name set: ?GetType@CModeSummonerDie@NCreatureMode@@UBEKXZ; `ret` matches the signature.
namespace NCreatureMode {
class CModeSummonerDie {
public:
    virtual unsigned long GetType() const;
};
unsigned long CModeSummonerDie::GetType() const
{
    return 0x5c;
}
}
