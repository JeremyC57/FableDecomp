// NCreatureMode::CModeSummonerSummon::GetType() const -- returns 0x59
// Leaf: ?GetType@CModeSummonerSummon@NCreatureMode@@UBEKXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x008CC160, thiscall, `ret`.
// Identity: slot 1 of NCreatureMode::CModeSummonerSummon's vtable 0x01282BA0 (defined here: the slot holds this body in 1 vtable(s), all of NCreatureMode::CModeSummonerSummon or derived); the Xbox 360 Test PDB virtual order (third source) aligned on 6 TLC-named slots of that vtable puts GetType(void) at this slot; Ego_r/FableWin name set: ?GetType@CModeSummonerSummon@NCreatureMode@@UBEKXZ; `ret` matches the signature.
namespace NCreatureMode {
class CModeSummonerSummon {
public:
    virtual unsigned long GetType() const;
};
unsigned long CModeSummonerSummon::GetType() const
{
    return 0x59;
}
}
