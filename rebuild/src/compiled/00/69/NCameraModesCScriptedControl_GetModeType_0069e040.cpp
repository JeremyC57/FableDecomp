// NCameraModes::CScriptedControl::GetModeType() const -- returns (EMode)8
// Leaf: ?GetModeType@CScriptedControl@NCameraModes@@UBE?AW4EMode@2@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x0069E040, thiscall, `ret`.
// Identity: slot 2 of NCameraModes::CScriptedControl's vtable 0x0125C58C (defined here: the slot holds this body in 1 vtable(s), all of NCameraModes::CScriptedControl or derived); the Xbox 360 Test PDB virtual order (third source) aligned on 9 TLC-named slots of that vtable puts GetModeType(void) at this slot; Ego_r/FableWin name set: ?GetModeType@CScriptedControl@NCameraModes@@UBE?AW4EMode@2@XZ; `ret` matches the signature.
namespace NCameraModes { enum EMode { }; }
namespace NCameraModes {
class CScriptedControl {
public:
    virtual NCameraModes::EMode GetModeType() const;
};
NCameraModes::EMode CScriptedControl::GetModeType() const
{
    return (EMode)8;
}
}
