// NCreatureMode::CModeForcePushed::GetType() const -- returns 0x20
// Leaf: ?GetType@CModeForcePushed@NCreatureMode@@UBEKXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00740970, thiscall, `ret`.
// Identity: slot 1 of NCreatureMode::CModeForcePushed's vtable 0x01267EBC (defined here: the slot holds this body in 1 vtable(s), all of NCreatureMode::CModeForcePushed or derived); the Xbox 360 Test PDB virtual order (third source) aligned on 7 TLC-named slots of that vtable puts GetType(void) at this slot; Ego_r/FableWin name set: ?GetType@CModeForcePushed@NCreatureMode@@UBEKXZ; `ret` matches the signature.
namespace NCreatureMode {
class CModeForcePushed {
public:
    virtual unsigned long GetType() const;
};
unsigned long CModeForcePushed::GetType() const
{
    return 0x20;
}
}
