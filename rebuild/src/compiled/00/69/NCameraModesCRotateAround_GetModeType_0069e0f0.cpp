// NCameraModes::CRotateAround::GetModeType() const -- returns (EMode)0xa
// Leaf: ?GetModeType@CRotateAround@NCameraModes@@UBE?AW4EMode@2@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x0069E0F0, thiscall, `ret`.
// Identity: slot 2 of NCameraModes::CRotateAround's vtable 0x0125C674 (defined here: the slot holds this body in 1 vtable(s), all of NCameraModes::CRotateAround or derived); the Xbox 360 Test PDB virtual order (third source) aligned on 6 TLC-named slots of that vtable puts GetModeType(void) at this slot; Ego_r/FableWin name set: ?GetModeType@CRotateAround@NCameraModes@@UBE?AW4EMode@2@XZ; `ret` matches the signature.
namespace NCameraModes { enum EMode { }; }
namespace NCameraModes {
class CRotateAround {
public:
    virtual NCameraModes::EMode GetModeType() const;
};
NCameraModes::EMode CRotateAround::GetModeType() const
{
    return (EMode)0xa;
}
}
