// NCreatureMode::CModeBriarRoseTeleportIntangible::GetType() const -- returns 0x41
// Leaf: ?GetType@CModeBriarRoseTeleportIntangible@NCreatureMode@@UBEKXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x008C3790, thiscall, `ret`.
// Identity: slot 1 of NCreatureMode::CModeBriarRoseTeleportIntangible's vtable 0x0128026C (defined here: the slot holds this body in 1 vtable(s), all of NCreatureMode::CModeBriarRoseTeleportIntangible or derived); the Xbox 360 Test PDB virtual order (third source) aligned on 7 TLC-named slots of that vtable puts GetType(void) at this slot; Ego_r/FableWin name set: ?GetType@CModeBriarRoseTeleportIntangible@NCreatureMode@@UBEKXZ; `ret` matches the signature.
namespace NCreatureMode {
class CModeBriarRoseTeleportIntangible {
public:
    virtual unsigned long GetType() const;
};
unsigned long CModeBriarRoseTeleportIntangible::GetType() const
{
    return 0x41;
}
}
