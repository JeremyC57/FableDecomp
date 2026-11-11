// NCreatureMode::CModeSummonerLightningOrb::GetType() const -- returns 0x5a
// Leaf: ?GetType@CModeSummonerLightningOrb@NCreatureMode@@UBEKXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x008CC240, thiscall, `ret`.
// Identity: slot 1 of NCreatureMode::CModeSummonerLightningOrb's vtable 0x01282BDC (defined here: the slot holds this body in 1 vtable(s), all of NCreatureMode::CModeSummonerLightningOrb or derived); the Xbox 360 Test PDB virtual order (third source) aligned on 6 TLC-named slots of that vtable puts GetType(void) at this slot; Ego_r/FableWin name set: ?GetType@CModeSummonerLightningOrb@NCreatureMode@@UBEKXZ; `ret` matches the signature.
namespace NCreatureMode {
class CModeSummonerLightningOrb {
public:
    virtual unsigned long GetType() const;
};
unsigned long CModeSummonerLightningOrb::GetType() const
{
    return 0x5a;
}
}
