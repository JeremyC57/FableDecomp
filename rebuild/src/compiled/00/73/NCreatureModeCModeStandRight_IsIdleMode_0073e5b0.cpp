// NCreatureMode::CModeStandRight::IsIdleMode() const -- returns true
// Leaf: ?IsIdleMode@CModeStandRight@NCreatureMode@@UBE_NXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x0073E5B0, thiscall, `ret`.
// Identity: slot 8 of NCreatureMode::CModeStandRight's vtable 0x01267534 (defined here: the slot holds this body in 1 vtable(s), all of NCreatureMode::CModeStandRight or derived); the Xbox 360 Test PDB virtual order (third source) aligned on 7 TLC-named slots of that vtable puts IsIdleMode(void) at this slot; Ego_r/FableWin name set: ?IsIdleMode@CModeStandRight@NCreatureMode@@UBE_NXZ; `ret` matches the signature.
namespace NCreatureMode {
class CModeStandRight {
public:
    virtual bool IsIdleMode() const;
};
bool CModeStandRight::IsIdleMode() const
{
    return true;
}
}
