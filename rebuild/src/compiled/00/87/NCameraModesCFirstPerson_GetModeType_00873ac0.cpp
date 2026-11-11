// NCameraModes::CFirstPerson::GetModeType() const -- returns (EMode)4
// Leaf: ?GetModeType@CFirstPerson@NCameraModes@@UBE?AW4EMode@2@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00873AC0, thiscall, `ret`.
// Identity: slot 2 of NCameraModes::CFirstPerson's vtable 0x012776AC (defined here: the slot holds this body in 1 vtable(s), all of NCameraModes::CFirstPerson or derived); the Xbox 360 Test PDB virtual order (third source) aligned on 7 TLC-named slots of that vtable puts GetModeType(void) at this slot; Ego_r/FableWin name set: ?GetModeType@CFirstPerson@NCameraModes@@UBE?AW4EMode@2@XZ; `ret` matches the signature.
namespace NCameraModes { enum EMode { }; }
namespace NCameraModes {
class CFirstPerson {
public:
    virtual NCameraModes::EMode GetModeType() const;
};
NCameraModes::EMode CFirstPerson::GetModeType() const
{
    return (EMode)4;
}
}
