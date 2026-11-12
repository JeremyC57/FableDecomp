// NCreatureMode::CModeHitByEpicSpell::GetType() const -- returns 0x5f
// Leaf: ?GetType@CModeHitByEpicSpell@NCreatureMode@@UBEKXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00741CC0, thiscall, `ret`.
// Identity: slot 1 of NCreatureMode::CModeHitByEpicSpell's vtable 0x01268498 (defined here: the slot holds this body in 1 vtable(s), all of NCreatureMode::CModeHitByEpicSpell or derived); the Xbox 360 Test PDB virtual order (third source) aligned on 6 TLC-named slots of that vtable puts GetType(void) at this slot; Ego_r/FableWin name set: ?GetType@CModeHitByEpicSpell@NCreatureMode@@UBEKXZ; `ret` matches the signature.
namespace NCreatureMode {
class CModeHitByEpicSpell {
public:
    virtual unsigned long GetType() const;
};
unsigned long CModeHitByEpicSpell::GetType() const
{
    return 0x5f;
}
}
