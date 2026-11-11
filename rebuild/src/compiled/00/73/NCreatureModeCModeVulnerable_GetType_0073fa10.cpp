// NCreatureMode::CModeVulnerable::GetType() const -- returns 0x53
// Leaf: ?GetType@CModeVulnerable@NCreatureMode@@UBEKXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x0073FA10, thiscall, `ret`.
// Identity: slot 1 of NCreatureMode::CModeVulnerable's vtable 0x012679D4 (defined here: the slot holds this body in 1 vtable(s), all of NCreatureMode::CModeVulnerable or derived); the Xbox 360 Test PDB virtual order (third source) aligned on 7 TLC-named slots of that vtable puts GetType(void) at this slot; Ego_r/FableWin name set: ?GetType@CModeVulnerable@NCreatureMode@@UBEKXZ; `ret` matches the signature.
namespace NCreatureMode {
class CModeVulnerable {
public:
    virtual unsigned long GetType() const;
};
unsigned long CModeVulnerable::GetType() const
{
    return 0x53;
}
}
