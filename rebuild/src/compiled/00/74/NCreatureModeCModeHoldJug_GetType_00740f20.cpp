// NCreatureMode::CModeHoldJug::GetType() const -- returns 0x29
// Leaf: ?GetType@CModeHoldJug@NCreatureMode@@UBEKXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00740F20, thiscall, `ret`.
// Identity: slot 1 of NCreatureMode::CModeHoldJug's vtable 0x012680A8 (defined here: the slot holds this body in 1 vtable(s), all of NCreatureMode::CModeHoldJug or derived); the Xbox 360 Test PDB virtual order (third source) aligned on 7 TLC-named slots of that vtable puts GetType(void) at this slot; Ego_r/FableWin name set: ?GetType@CModeHoldJug@NCreatureMode@@UBEKXZ; `ret` matches the signature.
namespace NCreatureMode {
class CModeHoldJug {
public:
    virtual unsigned long GetType() const;
};
unsigned long CModeHoldJug::GetType() const
{
    return 0x29;
}
}
