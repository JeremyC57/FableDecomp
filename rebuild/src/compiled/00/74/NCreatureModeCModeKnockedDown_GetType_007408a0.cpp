// NCreatureMode::CModeKnockedDown::GetType() const -- returns 0x1f
// Leaf: ?GetType@CModeKnockedDown@NCreatureMode@@UBEKXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x007408A0, thiscall, `ret`.
// Identity: slot 1 of NCreatureMode::CModeKnockedDown's vtable 0x01267E78 (defined here: the slot holds this body in 1 vtable(s), all of NCreatureMode::CModeKnockedDown or derived); the Xbox 360 Test PDB virtual order (third source) aligned on 7 TLC-named slots of that vtable puts GetType(void) at this slot; Ego_r/FableWin name set: ?GetType@CModeKnockedDown@NCreatureMode@@UBEKXZ; `ret` matches the signature.
namespace NCreatureMode {
class CModeKnockedDown {
public:
    virtual unsigned long GetType() const;
};
unsigned long CModeKnockedDown::GetType() const
{
    return 0x1f;
}
}
