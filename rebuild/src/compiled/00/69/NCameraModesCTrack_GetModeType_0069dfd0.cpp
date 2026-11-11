// NCameraModes::CTrack::GetModeType() const -- returns (EMode)7
// Leaf: ?GetModeType@CTrack@NCameraModes@@UBE?AW4EMode@2@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x0069DFD0, thiscall, `ret`.
// Identity: slot 2 of NCameraModes::CTrack's vtable 0x0125C50C (defined here: the slot holds this body in 1 vtable(s), all of NCameraModes::CTrack or derived); the Xbox 360 Test PDB virtual order (third source) aligned on 8 TLC-named slots of that vtable puts GetModeType(void) at this slot; Ego_r/FableWin name set: ?GetModeType@CTrack@NCameraModes@@UBE?AW4EMode@2@XZ; `ret` matches the signature.
namespace NCameraModes { enum EMode { }; }
namespace NCameraModes {
class CTrack {
public:
    virtual NCameraModes::EMode GetModeType() const;
};
NCameraModes::EMode CTrack::GetModeType() const
{
    return (EMode)7;
}
}
