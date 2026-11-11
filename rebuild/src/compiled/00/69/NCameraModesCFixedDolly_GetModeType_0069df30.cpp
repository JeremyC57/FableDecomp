// NCameraModes::CFixedDolly::GetModeType() const -- returns (EMode)5
// Leaf: ?GetModeType@CFixedDolly@NCameraModes@@UBE?AW4EMode@2@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x0069DF30, thiscall, `ret`.
// Identity: slot 2 of NCameraModes::CFixedDolly's vtable 0x0125C48C (defined here: the slot holds this body in 1 vtable(s), all of NCameraModes::CFixedDolly or derived); the Xbox 360 Test PDB virtual order (third source) aligned on 9 TLC-named slots of that vtable puts GetModeType(void) at this slot; Ego_r/FableWin name set: ?GetModeType@CFixedDolly@NCameraModes@@UBE?AW4EMode@2@XZ; `ret` matches the signature.
namespace NCameraModes { enum EMode { }; }
namespace NCameraModes {
class CFixedDolly {
public:
    virtual NCameraModes::EMode GetModeType() const;
};
NCameraModes::EMode CFixedDolly::GetModeType() const
{
    return (EMode)5;
}
}
