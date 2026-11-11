// NCreatureMode::CModeStandBored::IsIdleMode() const -- returns true
// Leaf: ?IsIdleMode@CModeStandBored@NCreatureMode@@UBE_NXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x0073EDF0, thiscall, `ret`.
// Identity: slot 8 of NCreatureMode::CModeStandBored's vtable 0x01267630 (defined here: the slot holds this body in 1 vtable(s), all of NCreatureMode::CModeStandBored or derived); the Xbox 360 Test PDB virtual order (third source) aligned on 7 TLC-named slots of that vtable puts IsIdleMode(void) at this slot; Ego_r/FableWin name set: ?IsIdleMode@CModeStandBored@NCreatureMode@@UBE_NXZ; `ret` matches the signature.
namespace NCreatureMode {
class CModeStandBored {
public:
    virtual bool IsIdleMode() const;
};
bool CModeStandBored::IsIdleMode() const
{
    return true;
}
}
