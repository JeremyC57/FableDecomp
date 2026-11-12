// NCameraModes::CPCMouseLook::GetModeType() const -- returns (EMode)0x11
// Leaf: ?GetModeType@CPCMouseLook@NCameraModes@@UBE?AW4EMode@2@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00874E50, thiscall, `ret`.
// Identity: slot 2 of NCameraModes::CPCMouseLook's vtable 0x01277784 (defined here: the slot holds this body in 1 vtable(s), all of NCameraModes::CPCMouseLook or derived); the Xbox 360 Test PDB virtual order (third source) aligned on 9 TLC-named slots of that vtable puts GetModeType(void) at this slot; Ego_r/FableWin name set: ?GetModeType@CPCMouseLook@NCameraModes@@UBE?AW4EMode@2@XZ; `ret` matches the signature.
namespace NCameraModes { enum EMode { }; }
namespace NCameraModes {
class CPCMouseLook {
public:
    virtual NCameraModes::EMode GetModeType() const;
};
NCameraModes::EMode CPCMouseLook::GetModeType() const
{
    return (EMode)0x11;
}
}
