// NCreatureMode::CModeBriarRoseTeleportInvulnerable::GetType() const -- returns 0x40
// Leaf: ?GetType@CModeBriarRoseTeleportInvulnerable@NCreatureMode@@UBEKXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x008C3680, thiscall, `ret`.
// Identity: slot 1 of NCreatureMode::CModeBriarRoseTeleportInvulnerable's vtable 0x01280228 (defined here: the slot holds this body in 1 vtable(s), all of NCreatureMode::CModeBriarRoseTeleportInvulnerable or derived); the Xbox 360 Test PDB virtual order (third source) aligned on 6 TLC-named slots of that vtable puts GetType(void) at this slot; Ego_r/FableWin name set: ?GetType@CModeBriarRoseTeleportInvulnerable@NCreatureMode@@UBEKXZ; `ret` matches the signature.
namespace NCreatureMode {
class CModeBriarRoseTeleportInvulnerable {
public:
    virtual unsigned long GetType() const;
};
unsigned long CModeBriarRoseTeleportInvulnerable::GetType() const
{
    return 0x40;
}
}
