// NCameraModes::COrbitGame::GetModeType() const -- returns (EMode)0x10
// Leaf: ?GetModeType@COrbitGame@NCameraModes@@UBE?AW4EMode@2@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x0069E160, thiscall, `ret`.
// Identity: slot 2 of NCameraModes::COrbitGame's vtable 0x0125C6DC (defined here: the slot holds this body in 1 vtable(s), all of NCameraModes::COrbitGame or derived); the Xbox 360 Test PDB virtual order (third source) aligned on 7 TLC-named slots of that vtable puts GetModeType(void) at this slot; Ego_r/FableWin name set: ?GetModeType@COrbitGame@NCameraModes@@UBE?AW4EMode@2@XZ; `ret` matches the signature.
namespace NCameraModes { enum EMode { }; }
namespace NCameraModes {
class COrbitGame {
public:
    virtual NCameraModes::EMode GetModeType() const;
};
NCameraModes::EMode COrbitGame::GetModeType() const
{
    return (EMode)0x10;
}
}
