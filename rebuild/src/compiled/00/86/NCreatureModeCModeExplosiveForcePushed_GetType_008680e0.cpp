// NCreatureMode::CModeExplosiveForcePushed::GetType() const -- returns 0x42
// Leaf: ?GetType@CModeExplosiveForcePushed@NCreatureMode@@UBEKXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x008680E0, thiscall, `ret`.
// Identity: slot 1 of NCreatureMode::CModeExplosiveForcePushed's vtable 0x012770B4 (defined here: the slot holds this body in 1 vtable(s), all of NCreatureMode::CModeExplosiveForcePushed or derived); the Xbox 360 Test PDB virtual order (third source) aligned on 6 TLC-named slots of that vtable puts GetType(void) at this slot; Ego_r/FableWin name set: ?GetType@CModeExplosiveForcePushed@NCreatureMode@@UBEKXZ; `ret` matches the signature.
namespace NCreatureMode {
class CModeExplosiveForcePushed {
public:
    virtual unsigned long GetType() const;
};
unsigned long CModeExplosiveForcePushed::GetType() const
{
    return 0x42;
}
}
