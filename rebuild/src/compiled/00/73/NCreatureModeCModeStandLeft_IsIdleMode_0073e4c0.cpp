// NCreatureMode::CModeStandLeft::IsIdleMode() const -- returns true
// Leaf: ?IsIdleMode@CModeStandLeft@NCreatureMode@@UBE_NXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x0073E4C0, thiscall, `ret`.
// Identity: slot 8 of NCreatureMode::CModeStandLeft's vtable 0x012674F4 (defined here: the slot holds this body in 1 vtable(s), all of NCreatureMode::CModeStandLeft or derived); the Xbox 360 Test PDB virtual order (third source) aligned on 7 TLC-named slots of that vtable puts IsIdleMode(void) at this slot; Ego_r/FableWin name set: ?IsIdleMode@CModeStandLeft@NCreatureMode@@UBE_NXZ; `ret` matches the signature.
namespace NCreatureMode {
class CModeStandLeft {
public:
    virtual bool IsIdleMode() const;
};
bool CModeStandLeft::IsIdleMode() const
{
    return true;
}
}
