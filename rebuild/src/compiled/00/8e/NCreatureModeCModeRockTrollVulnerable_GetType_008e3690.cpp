// NCreatureMode::CModeRockTrollVulnerable::GetType() const -- returns 0x3a
// Leaf: ?GetType@CModeRockTrollVulnerable@NCreatureMode@@UBEKXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x008E3690, thiscall, `ret`.
// Identity: slot 1 of NCreatureMode::CModeRockTrollVulnerable's vtable 0x01285774 (defined here: the slot holds this body in 1 vtable(s), all of NCreatureMode::CModeRockTrollVulnerable or derived); the Xbox 360 Test PDB virtual order (third source) aligned on 6 TLC-named slots of that vtable puts GetType(void) at this slot; Ego_r/FableWin name set: ?GetType@CModeRockTrollVulnerable@NCreatureMode@@UBEKXZ; `ret` matches the signature.
namespace NCreatureMode {
class CModeRockTrollVulnerable {
public:
    virtual unsigned long GetType() const;
};
unsigned long CModeRockTrollVulnerable::GetType() const
{
    return 0x3a;
}
}
