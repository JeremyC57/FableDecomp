// NCreatureMode::CModeHoverNormal::GetType() const -- returns 0x1a
// Leaf: ?GetType@CModeHoverNormal@NCreatureMode@@UBEKXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x007440C0, thiscall, `ret`.
// Identity: slot 1 of NCreatureMode::CModeHoverNormal's vtable 0x012687FC (defined here: the slot holds this body in 1 vtable(s), all of NCreatureMode::CModeHoverNormal or derived); the Xbox 360 Test PDB virtual order (third source) aligned on 8 TLC-named slots of that vtable puts GetType(void) at this slot; Ego_r/FableWin name set: ?GetType@CModeHoverNormal@NCreatureMode@@UBEKXZ; `ret` matches the signature.
namespace NCreatureMode {
class CModeHoverNormal {
public:
    virtual unsigned long GetType() const;
};
unsigned long CModeHoverNormal::GetType() const
{
    return 0x1a;
}
}
