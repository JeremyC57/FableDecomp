// NCreatureMode::CModeHeroImitation::GetType() const -- returns 0x16
// Leaf: ?GetType@CModeHeroImitation@NCreatureMode@@UBEKXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x0073F530, thiscall, `ret`.
// Identity: slot 1 of NCreatureMode::CModeHeroImitation's vtable 0x0126784C (defined here: the slot holds this body in 1 vtable(s), all of NCreatureMode::CModeHeroImitation or derived); the Xbox 360 Test PDB virtual order (third source) aligned on 7 TLC-named slots of that vtable puts GetType(void) at this slot; Ego_r/FableWin name set: ?GetType@CModeHeroImitation@NCreatureMode@@UBEKXZ; `ret` matches the signature.
namespace NCreatureMode {
class CModeHeroImitation {
public:
    virtual unsigned long GetType() const;
};
unsigned long CModeHeroImitation::GetType() const
{
    return 0x16;
}
}
