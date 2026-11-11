// NCreatureMode::CModeCrouch::GetType() const -- returns 7
// Leaf: ?GetType@CModeCrouch@NCreatureMode@@UBEKXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x0073FE40, thiscall, `ret`.
// Identity: slot 1 of NCreatureMode::CModeCrouch's vtable 0x01267B34 (defined here: the slot holds this body in 1 vtable(s), all of NCreatureMode::CModeCrouch or derived); the Xbox 360 Test PDB virtual order (third source) aligned on 7 TLC-named slots of that vtable puts GetType(void) at this slot; Ego_r/FableWin name set: ?GetType@CModeCrouch@NCreatureMode@@UBEKXZ; `ret` matches the signature.
namespace NCreatureMode {
class CModeCrouch {
public:
    virtual unsigned long GetType() const;
};
unsigned long CModeCrouch::GetType() const
{
    return 7;
}
}
