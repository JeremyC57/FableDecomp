// NCreatureMode::CModeStandFront::IsIdleMode() const -- returns true
// Leaf: ?IsIdleMode@CModeStandFront@NCreatureMode@@UBE_NXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x0073E3D0, thiscall, `ret`.
// Identity: slot 8 of NCreatureMode::CModeStandFront's vtable 0x012674AC (defined here: the slot holds this body in 1 vtable(s), all of NCreatureMode::CModeStandFront or derived); the Xbox 360 Test PDB virtual order (third source) aligned on 7 TLC-named slots of that vtable puts IsIdleMode(void) at this slot; Ego_r/FableWin name set: ?IsIdleMode@CModeStandFront@NCreatureMode@@UBE_NXZ; `ret` matches the signature.
namespace NCreatureMode {
class CModeStandFront {
public:
    virtual bool IsIdleMode() const;
};
bool CModeStandFront::IsIdleMode() const
{
    return true;
}
}
