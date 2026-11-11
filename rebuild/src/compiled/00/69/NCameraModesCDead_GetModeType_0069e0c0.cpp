// NCameraModes::CDead::GetModeType() const -- returns (EMode)9
// Leaf: ?GetModeType@CDead@NCameraModes@@UBE?AW4EMode@2@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x0069E0C0, thiscall, `ret`.
// Identity: slot 2 of NCameraModes::CDead's vtable 0x0125C5F4 (defined here: the slot holds this body in 1 vtable(s), all of NCameraModes::CDead or derived); the Xbox 360 Test PDB virtual order (third source) aligned on 7 TLC-named slots of that vtable puts GetModeType(void) at this slot; Ego_r/FableWin name set: ?GetModeType@CDead@NCameraModes@@UBE?AW4EMode@2@XZ; `ret` matches the signature.
namespace NCameraModes { enum EMode { }; }
namespace NCameraModes {
class CDead {
public:
    virtual NCameraModes::EMode GetModeType() const;
};
NCameraModes::EMode CDead::GetModeType() const
{
    return (EMode)9;
}
}
