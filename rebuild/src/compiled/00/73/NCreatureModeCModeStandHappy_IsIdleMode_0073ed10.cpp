// NCreatureMode::CModeStandHappy::IsIdleMode() const -- returns true
// Leaf: ?IsIdleMode@CModeStandHappy@NCreatureMode@@UBE_NXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x0073ED10, thiscall, `ret`.
// Identity: slot 8 of NCreatureMode::CModeStandHappy's vtable 0x012675F0 (defined here: the slot holds this body in 1 vtable(s), all of NCreatureMode::CModeStandHappy or derived); the Xbox 360 Test PDB virtual order (third source) aligned on 7 TLC-named slots of that vtable puts IsIdleMode(void) at this slot; Ego_r/FableWin name set: ?IsIdleMode@CModeStandHappy@NCreatureMode@@UBE_NXZ; `ret` matches the signature.
namespace NCreatureMode {
class CModeStandHappy {
public:
    virtual bool IsIdleMode() const;
};
bool CModeStandHappy::IsIdleMode() const
{
    return true;
}
}
