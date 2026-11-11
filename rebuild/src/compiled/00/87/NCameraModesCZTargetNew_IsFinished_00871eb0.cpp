// NCameraModes::CZTargetNew::IsFinished() const -- returns false
// Leaf: ?IsFinished@CZTargetNew@NCameraModes@@UBE_NXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00871EB0, thiscall, `ret`.
// Identity: slot 13 of NCameraModes::CZTargetNew's vtable 0x0127754C (defined here: the slot holds this body in 1 vtable(s), all of NCameraModes::CZTargetNew or derived); the Xbox 360 Test PDB virtual order (third source) aligned on 8 TLC-named slots of that vtable puts IsFinished(void) at this slot; Ego_r/FableWin name set: ?IsFinished@CZTargetNew@NCameraModes@@UBE_NXZ; `ret` matches the signature.
namespace NCameraModes {
class CZTargetNew {
public:
    virtual bool IsFinished() const;
};
bool CZTargetNew::IsFinished() const
{
    return false;
}
}
