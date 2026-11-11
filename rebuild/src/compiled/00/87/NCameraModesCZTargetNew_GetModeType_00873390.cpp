// NCameraModes::CZTargetNew::GetModeType() const -- returns (EMode)0xb
// Leaf: ?GetModeType@CZTargetNew@NCameraModes@@UBE?AW4EMode@2@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00873390, thiscall, `ret`.
// Identity: slot 2 of NCameraModes::CZTargetNew's vtable 0x0127754C (defined here: the slot holds this body in 1 vtable(s), all of NCameraModes::CZTargetNew or derived); the Xbox 360 Test PDB virtual order (third source) aligned on 8 TLC-named slots of that vtable puts GetModeType(void) at this slot; Ego_r/FableWin name set: ?GetModeType@CZTargetNew@NCameraModes@@UBE?AW4EMode@2@XZ; `ret` matches the signature.
namespace NCameraModes { enum EMode { }; }
namespace NCameraModes {
class CZTargetNew {
public:
    virtual NCameraModes::EMode GetModeType() const;
};
NCameraModes::EMode CZTargetNew::GetModeType() const
{
    return (EMode)0xb;
}
}
