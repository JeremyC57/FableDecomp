// NCameraModes::CSACCombat::GetModeType() const -- returns (EMode)0xe
// Leaf: ?GetModeType@CSACCombat@NCameraModes@@UBE?AW4EMode@2@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00873440, thiscall, `ret`.
// Identity: slot 2 of NCameraModes::CSACCombat's vtable 0x012775B4 (defined here: the slot holds this body in 1 vtable(s), all of NCameraModes::CSACCombat or derived); the Xbox 360 Test PDB virtual order (third source) aligned on 8 TLC-named slots of that vtable puts GetModeType(void) at this slot; Ego_r/FableWin name set: ?GetModeType@CSACCombat@NCameraModes@@UBE?AW4EMode@2@XZ; `ret` matches the signature.
namespace NCameraModes { enum EMode { }; }
namespace NCameraModes {
class CSACCombat {
public:
    virtual NCameraModes::EMode GetModeType() const;
};
NCameraModes::EMode CSACCombat::GetModeType() const
{
    return (EMode)0xe;
}
}
