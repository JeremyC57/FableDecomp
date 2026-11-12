// NCreatureMode::CModeCrouch::IsRemoveOnDeactivate() const -- returns true
// Leaf: ?IsRemoveOnDeactivate@CModeCrouch@NCreatureMode@@UBE_NXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x0073FE60, thiscall, `ret`.
// Identity: slot 11 of NCreatureMode::CModeCrouch's vtable 0x01267B34 (defined here: the slot holds this body in 1 vtable(s), all of NCreatureMode::CModeCrouch or derived); the Xbox 360 Test PDB virtual order (third source) aligned on 7 TLC-named slots of that vtable puts IsRemoveOnDeactivate(void) at this slot; Ego_r/FableWin name set: ?IsRemoveOnDeactivate@CModeCrouch@NCreatureMode@@UBE_NXZ; `ret` matches the signature.
namespace NCreatureMode {
class CModeCrouch {
public:
    virtual bool IsRemoveOnDeactivate() const;
};
bool CModeCrouch::IsRemoveOnDeactivate() const
{
    return true;
}
}
